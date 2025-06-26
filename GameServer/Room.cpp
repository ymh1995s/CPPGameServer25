#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "Monster.h"
#include "ObjectUtils.h"

RoomRef GRoom = make_shared<Room>();

Room::Room()
{
}

Room::~Room()
{

}

void Room::Init()
{
	MonsterInit();
}

bool Room::EnterRoom(ObjectRef object)
{
	bool success = AddObject(object);
	shared_ptr<Player> player = dynamic_pointer_cast<Player>(object);

	// 입장 사실을 신입 플레이어에게 알린다
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		// S_Enter : 자기 자신의 캐릭터 
		Protocol::S_EnterGame enterGamePkt;

		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(*player->playerInfo);
		enterGamePkt.set_allocated_playerinfo(playerInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 입장 사실을 다른 플레이어에게 알린다
	{
		// S_Spawn : 다른 사람의 캐릭터
		Protocol::S_PlayerSpawn spawnPkt;

		Protocol::PlayerInfo* playerInfo = spawnPkt.add_playerinfos();
		playerInfo->CopyFrom(*player->playerInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, player->playerInfo->playerid());
	}

	// 기존 입장한 플레이어 목록을 신입 플레이어한테 전송해준다
	if (PlayerRef player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_PlayerSpawn spawnPkt;

		for (auto& item : _players)
		{
			shared_ptr<Player> p = dynamic_pointer_cast<Player>(item.second);

			if (item.second->IsPlayer() == false)
				continue;

			// 자기 자신은 생략
			if (player->playerInfo->playerid() == p->playerInfo->playerid())
				continue;

			Protocol::PlayerInfo* playerInfo = spawnPkt.add_playerinfos();
			playerInfo->CopyFrom(*p->playerInfo);
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 현재 있는 map의 몬스터 동기화
	Protocol::S_MonsterSpawn msPkt;
	for (auto m : _monsters)
	{
		msPkt.add_monsterinfos()->CopyFrom(*m.second->monsterInfo);
	}
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(msPkt);
	if (auto session = player->session.lock())
		session->Send(sendBuffer);

	return success;
}

bool Room::LeaveRoom(ObjectRef object)
{
	if (object == nullptr)
		return false;

	shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(object);
	if (player == nullptr) return false;

	uint64 objectId = player->playerInfo->playerid();

	bool success = RemoveObject(objectId);

	// 퇴장 사실을 퇴장하는 플레이어에게 알린다
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_LeaveGame leaveGamePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 퇴장 사실을 알린다
	{
		Protocol::S_PlayerDespawn despawnPkt;
		despawnPkt.add_playerids(objectId);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);

		if (auto player = dynamic_pointer_cast<Player>(object))
			if (auto session = player->session.lock())
				session->Send(sendBuffer);
	}

	return success;
}

bool Room::HandleEnterPlayer(PlayerRef player)
{
	return EnterRoom(player);
}

bool Room::HandleLeavePlayer(PlayerRef player)
{
	return LeaveRoom(player);
}

void Room::HandleMove(PlayerRef player, Protocol::C_PlayerMove pkt)
{
	const uint64 playerId = player->playerInfo->playerid();
	if (_players.find(playerId) == _players.end())
		return;

	// 서버에서 관리하기 위해 데이터 반영
	PlayerRef p = dynamic_pointer_cast<Player>(_players[playerId]);
	p->playerInfo->set_positionx(pkt.positionx());
	p->playerInfo->set_positiony(pkt.positiony());
	p->playerInfo->set_creaturestate(pkt.state());

	// 이동 사실을 알린다
	// 왜 여기선 동적 할당 안했지?
	Protocol::S_PlayerMove movePkt;
	movePkt.set_playerid(playerId);
	movePkt.set_state(pkt.state());
	movePkt.set_positionx(pkt.positionx());
	movePkt.set_positiony(pkt.positiony());
	movePkt.set_isright(pkt.isright());

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
	Broadcast(sendBuffer);
}

void Room::UpdateTick()
{
	DoTimer(100, &Room::UpdateTick);

	// 몬스터 업데이트
	MonsterUpdate();
}

RoomRef Room::GetRoomRef()
{
	return static_pointer_cast<Room>(shared_from_this());
}

bool Room::AddObject(ObjectRef object)
{
	// 있다면 문제가 있다.
	//if (_objects.find(object->objectInfo->object_id()) != _objects.end())
	//	return false;

	shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(object);
	if (player == nullptr) return false;

	_players.insert(make_pair(player->playerInfo->playerid(), player));

	object->room.store(GetRoomRef());

	return true;
}

bool Room::RemoveObject(uint64 objectId)
{
	// 없다면 문제가 있다.
	if (_players.find(objectId) == _players.end())
		return false;

	ObjectRef object = _players[objectId];
	PlayerRef player = dynamic_pointer_cast<Player>(object);
	if (player)
		player->room.store(weak_ptr<Room>());

	_players.erase(objectId);

	return true;
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{
	for (auto& item : _players)
	{
		PlayerRef player = dynamic_pointer_cast<Player>(item.second);
		if (player == nullptr)
			continue;
		if (player->playerInfo->playerid() == exceptId)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
	}
}


///////////////////////////////////////////
///////////// 몬스터 //////////////////////
///////////////////////////////////////////
void Room::MonsterInit()
{
	int count = 5;
	// 5마리 정도만 뿌려볼까
	for (int i = 0; i < count; i++)
	{
		Protocol::S_MonsterSpawn* monsterSpawnPkt = new Protocol::S_MonsterSpawn();

		MonsterRef monster = ObjectUtils::CreateMonster();
		cout << "Monster No."<<monster->id << " Spawned\r\n";
		monster->monsterInfo->set_destinationx(0);
		monster->monsterInfo->set_destinationy(0.75f);
		monster->monsterInfo->set_name(std::string(reinterpret_cast<const char*>(u8"달팽이")));

		// 몬스터의 스펙을 임의로 설정
		Protocol::MonsterStatInfo* ms = new Protocol::MonsterStatInfo();
		ms->set_attackpower(1);
		ms->set_hp(50);
		ms->set_maxhp(50);
		ms->set_speed(0.1f);
		ms->set_exp(10);
		monster->monsterInfo->set_allocated_statinfo(ms);

		MonsterEnterRoom(monster);
	}
}

void Room::MonsterUpdate()
{
	for (auto m : _monsters)
	{
		m.second->Update();

		// 음.. 얘넨 필요한가?
		/*_normalMonsters[normalMonster.Id].Info = normalMonster.Info;
		_normalMonsters[normalMonster.Id].Stat = normalMonster.Stat;*/
	}

}

bool Room::AddMonsterObject(MonsterRef monster)
{
	_monsters.insert(make_pair(monster->monsterInfo->monsterid(), monster));

	monster->room.store(GetRoomRef());

	return true;
}


void Room::MonsterEnterRoom(MonsterRef monster)
{
	AddMonsterObject(monster);

	Protocol::S_MonsterSpawn mSpawnPkt;
	mSpawnPkt.add_monsterinfos()->CopyFrom(*monster->monsterInfo);
\
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(mSpawnPkt);
	Broadcast(sendBuffer, -1); // -1 : 제외 없이 모든 플레이어에게 보내겠다.
}

void Room::LeaveMonster(int objectId)
{
}

void Room::MonsterHitAndSetTarget(PlayerRef player, int monsterId)
{
}

bool Room::IsPlayerInRoom(int id)
{
	return false;
}

void Room::RemoveMonster(int id)
{
}

void Room::GameClear()
{
}

