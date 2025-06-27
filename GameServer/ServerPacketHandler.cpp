#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "Protocol.pb.h"
#include "Room.h"
#include "ObjectUtils.h"
#include "Player.h"
#include "GameSession.h"
#include "GameSessionManager.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_C_CLASS_CHOICE(PacketSessionRef& session, Protocol::C_ClassChoice& pkt)
{
    // 원래 GameSession에서 OnConnected 할 때 등록 할 때를 이렇게 변형한건데 문법 맞겠지?
    // GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));
    GSessionManager.Add(static_pointer_cast<GameSession>(session));

    // 플레이어 생성 및 기본 정보 대입
    PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));
    player->playerInfo->set_positionx(1.111f);
    player->playerInfo->set_positiony(1.111f);

    Protocol::PlayerStatInfo* playerStatInfo = new Protocol::PlayerStatInfo();
    playerStatInfo->set_classtype(pkt.classtype());
    player->playerInfo->set_allocated_statinfo(playerStatInfo);

    // 방에 입장
    GRoom->DoAsync(&Room::HandleEnterPlayer, player);

    cout << "The Player " << player->playerInfo->playerid() << " Enter the Room\n";
	return true;
}

bool Handle_C_PLAYER_MOVE(PacketSessionRef& session, Protocol::C_PlayerMove& pkt)
{
    auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleMove, player, pkt);
	//room->HandleMove(pkt);

	return true;
}

bool Handle_C_HIT_MONSTER(PacketSessionRef& session, Protocol::C_HitMonster& pkt)
{
    cout << "Player Hit Monster\n";

    // auto 안 쓰기 연습
    shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);

    PlayerRef player = gameSession->player.load();
    if (player == nullptr)
        return false;

    RoomRef room = player->room.load().lock();
    if (room == nullptr)
        return false;

    vector<int> damageAmounts;
    for(auto playerAttackPower : pkt.playerattackpowers())
        damageAmounts.push_back(playerAttackPower);

    room->DoAsync(&Room::MonsterHitAndSetTarget, player, pkt.monsterid(), damageAmounts);

    return true;
}

bool Handle_C_PLAYER_DAMAGED(PacketSessionRef& session, Protocol::C_PlayerDamaged& pkt)
{
    cout << "Player Damaged\r\n";
    return false;
}

#pragma region 미구현

bool Handle_C_PLAYER_DIE(PacketSessionRef& session, Protocol::C_PlayerDie& pkt)
{
    // 예정 없음
    return false;
}

bool Handle_C_PLAYER_SKILL(PacketSessionRef& session, Protocol::C_PlayerSkill& pkt)
{
    // 아마도 미사용
    return false;
}

bool Handle_C_CHANGE_MAP(PacketSessionRef& session, Protocol::C_ChangeMap& pkt)
{
    // 예정 없음
    return false;
}

bool Handle_C_BOSS_REGISTER(PacketSessionRef& session, Protocol::C_BossRegister& pkt)
{
    // 예정 없음
    return false;
}

bool Handle_C_BOSS_CANCLE(PacketSessionRef& session, Protocol::C_BossCancle& pkt)
{
    // 예정 없음
    return false;
}

bool Handle_C_LOOT_ITEM(PacketSessionRef& session, Protocol::C_LootItem& pkt)
{
    // 예정 없음
    return false;
}
#pragma endregion