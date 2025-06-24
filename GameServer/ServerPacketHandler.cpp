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
    cout << " 클래스 선택 패킷 수신!\n";

    // 원래 GameSession에서 OnConnected 할 때 등록 할 때를 이렇게 변형한건데 문법 맞겠지?
    // GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));
    GSessionManager.Add(static_pointer_cast<GameSession>(session));

    // C#에선 여기에 해당 클라이언트에 정보를 설정해주는데 여기 없어도 괜찮은가?
    // 이름, 세션, 직업 정보 등 
    // => 이름은 여기선 필요 없고
    // => 세션은 CreatePlayer()에서
    // => 직업 정보는 아래에 내가 추가로 코딩

    // 플레이어 생성 및 기본 정보 대입
    PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));
    player->playerInfo->set_positionx(1.111f);
    player->playerInfo->set_positiony(1.111f);

    Protocol::PlayerStatInfo* playerStatInfo = new Protocol::PlayerStatInfo();
    playerStatInfo->set_classtype(pkt.classtype());
    player->playerInfo->set_allocated_statinfo(playerStatInfo);

    // 방에 입장
    GRoom->DoAsync(&Room::HandleEnterPlayer, player);

	return true;
}

bool Handle_C_PLAYER_MOVE(PacketSessionRef& session, Protocol::C_PlayerMove& pkt)
{
    return false;
}

bool Handle_C_PLAYER_DIE(PacketSessionRef& session, Protocol::C_PlayerDie& pkt)
{
    return false;
}

bool Handle_C_PLAYER_SKILL(PacketSessionRef& session, Protocol::C_PlayerSkill& pkt)
{
    return false;
}

bool Handle_C_HIT_MONSTER(PacketSessionRef& session, Protocol::C_HitMonster& pkt)
{
    return false;
}

bool Handle_C_PLAYER_DAMAGED(PacketSessionRef& session, Protocol::C_PlayerDamaged& pkt)
{
    return false;
}

bool Handle_C_CHANGE_MAP(PacketSessionRef& session, Protocol::C_ChangeMap& pkt)
{
    return false;
}

bool Handle_C_BOSS_REGISTER(PacketSessionRef& session, Protocol::C_BossRegister& pkt)
{
    return false;
}

bool Handle_C_BOSS_CANCLE(PacketSessionRef& session, Protocol::C_BossCancle& pkt)
{
    return false;
}

bool Handle_C_LOOT_ITEM(PacketSessionRef& session, Protocol::C_LootItem& pkt)
{
    return false;
}

//bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
//{
//	// 플레이어 생성
//	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));
//
//	// 방에 입장
//	GRoom->DoAsync(&Room::HandleEnterPlayer, player);
//	//GRoom->HandleEnterPlayerLocked(player);
//
//	return true;
//}
//
//bool Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
//{
//	auto gameSession = static_pointer_cast<GameSession>(session);
//
//	PlayerRef player = gameSession->player.load();
//	if (player == nullptr)
//		return false;
//
//	RoomRef room = player->room.load().lock();
//	if (room == nullptr)
//		return false;
//
//	room->HandleLeavePlayer(player);
//
//	return true;
//}
//
//bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
//{
//	auto gameSession = static_pointer_cast<GameSession>(session);
//
//	PlayerRef player = gameSession->player.load();
//	if (player == nullptr)
//		return false;
//
//	RoomRef room = player->room.load().lock();
//	if (room == nullptr)
//		return false;
//
//	room->DoAsync(&Room::HandleMove, pkt);
//	//room->HandleMove(pkt);
//
//	return true;
//}
//
//bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
//{
//	return true;
//}
