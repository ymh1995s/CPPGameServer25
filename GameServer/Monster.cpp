#include "pch.h"
#include "Monster.h"
#include <chrono>
#include <algorithm>
#include "Room.h"
#include "Player.h"

Monster::Monster()
{
	monsterInfo = new Protocol::MonsterInfo();
    _currentState = MonsterState::Idle;
}

Monster::~Monster()
{
	delete monsterInfo;
}

void Monster::Update()
{
    // 클라이언트와 주고 받은 패킷을 기반으로 State 결정
    UpdateThink();
    UpdateTarget();

    switch (_currentState)
    {
    case MonsterState::Idle:
        UpdateIdle();
        break;
    case MonsterState::Move:
        UpdateMove();
        break;
    case MonsterState::Stun:
        UpdateStun();
        break;
    case MonsterState::Skill:
        UpdateSkill();
        break;
    case MonsterState::Dead:
        UpdateDead();
        break;
    }

    UpdateInfo();
    BroadcastMove();
}

void Monster::UpdateThink()
{
    if (_currentState == MonsterState::Dead) return;

    // 현재 시간을 바탕으로 경과 시간 계산
    steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    float elapsedTime = duration_cast<std::chrono::duration<float>>(currentTime - _lastThinkTime).count();

    // 스턴 상태일 경우
    if (_currentState == MonsterState::Stun)
    {
        // 스턴 지속 시간이 경과했는지 확인
        if ((float)(currentTime - _stunStartTime).count() >= _stunDuration)
        {
            // 스턴이 끝난 후 Think를 호출
            Think();

            _thinkInterval = std::uniform_real_distribution<float>{ 3.0f, 5.0f }(rng); // 3초에서 5초
            _lastThinkTime = currentTime;
        }
    }
    else if (_currentState == MonsterState::Skill)
    {
        if ((float)(currentTime - _skillStartTime).count() >= _skillDuration)
        {
            Think();

            _thinkInterval = std::uniform_real_distribution<float>{ 3.0f, 5.0f }(rng);; // 3초에서 5초
            _lastThinkTime = currentTime;
        }
    }

    else // 스턴 상태, 스킬 사용 상태가 아닐 경우
    {
        if (elapsedTime >= _thinkInterval)
        {
            Think();

            _thinkInterval = std::uniform_real_distribution<float>{ 3.0f, 5.0f }(rng); // 3초에서 5초
            _lastThinkTime = currentTime;
        }
    }
}

void Monster::UpdateTarget()
{
    // TODO
}

void Monster::UpdateIdle()
{
    _currentState = MonsterState::Idle;
}

void Monster::UpdateMove()
{
    _currentState = MonsterState::Move;

    _destinationPos.x += (_isRight ? monsterInfo->statinfo().speed() : -monsterInfo->statinfo().speed());

    if (auto target = _target.lock())
    {
        float distance = abs(target->playerInfo->positionx() - _destinationPos.x);

        if (distance >= 1.0f) // 거리 차이가 1.0 이상일 때만 변경
        {
            _isRight = target->playerInfo->positionx() > _destinationPos.x;
        }
    }

    _destinationPos.x = clamp(_destinationPos.x, _minX, _maxX);
}

void Monster::UpdateStun()
{
    if (_currentState != MonsterState::Stun)
        _stunStartTime = std::chrono::steady_clock::now(); // 스턴 상태로 전환할 때 시작 시간 기록.

    _currentState = MonsterState::Stun;
}

void Monster::UpdateSkill()
{
    // TODO: 일반 몬스터는 아직 개발 계획 없음.
}

void Monster::UpdateDead()
{
    _currentState = MonsterState::Dead;
}

void Monster::UpdateInfo()
{
    // Update Info and Stat
    monsterInfo->set_destinationx(std::clamp(_destinationPos.x, _minX, _maxX));
    monsterInfo->set_destinationy(_destinationPos.y);
    monsterInfo->set_creaturestate(static_cast<Protocol::MonsterState>(_currentState));
    //monsterInfo->set_monsterid(Id);
    //monsterInfo->set_allocated_statinfo(Stat);
}

void Monster::BroadcastMove()
{            
    // 다른 플레이어한테도 알려준다.
    Protocol::S_MonsterMove smmPkt;
    smmPkt.set_state(static_cast<Protocol::MonsterState>(_currentState));
    smmPkt.set_monsterid(id);
    smmPkt.set_destinationx(std::clamp(_destinationPos.x, _minX, _maxX));
    smmPkt.set_destinationy(_destinationPos.y);
    smmPkt.set_isright(_isRight);

    SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(smmPkt);
    std::shared_ptr<Room> roomPtr = room.load().lock();
    if (roomPtr)
    {
        roomPtr->Broadcast(sendBuffer,-1);
    }
}

void Monster::Think()
{            
    // 타겟이 설정되지 않은 경우 Idle, Move 중 random Selection
    if (_target.expired())
    {
        _currentState = static_cast<MonsterState>(dist(rng));

        // 방향 랜덤 Selection
        _isRight = std::uniform_int_distribution<int>{ 0, 1 }(rng) == 0;
    }
    // 타겟이 설정된 경우 Move, Skill 중 random Selection
    else
    {
        _currentState = MonsterState::Move;
    }
}
