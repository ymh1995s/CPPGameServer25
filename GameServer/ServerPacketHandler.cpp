#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "Protocol.pb.h"
#include "Room.h"
#include "ObjectUtils.h"
#include "Player.h"
#include "GameSession.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_C_CLASS_CHOICE(PacketSessionRef& session, Protocol::C_ClassChoice& pkt)
{

    Protocol::C_ClassChoice enterPkt;
    cout << " 클래스 선택 패킷 수신!\n";
    /*
    C_ClassChoice pkt = packet as C_ClassChoice;
    ClientSession clientSession = session as ClientSession;

    // TODO : 오브젝트 매니저에 추가
    clientSession.MyPlayer = ObjectManager.Instance.Add<Player>();
    {
        clientSession.MyPlayer.Info.Name = $"Player_{clientSession.MyPlayer.Info.PlayerId}";
        clientSession.MyPlayer.Session = clientSession;
    }
    clientSession.MyPlayer.Info.StatInfo.ClassType = pkt.ClassType;

    GameRoom room = RoomManager.Instance.Find((int)MapName.Tutorial);
    if (room == null)
    {
        Console.WriteLine($"{(int)MapName.Tutorial}번 방이 존재하지 않습니다. 클라이언트 접속 종료");
        clientSession.Disconnect();
        return;
    }

    room.Push(room.PlayerEnterGame, clientSession.MyPlayer, 0);



	Protocol::S_EnterGame enterPkt;

	for (int32 i = 0; i < 3; i++)
	{
		Protocol::ObjectInfo* player = enterPkt.add_players();
		Protocol::PosInfo* posInfo = player->mutable_pos_info();
		posInfo->set_x(Utils::GetRandom(0.f, 100.f));
		posInfo->set_y(Utils::GetRandom(0.f, 100.f));
		posInfo->set_z(Utils::GetRandom(0.f, 100.f));
		posInfo->set_yaw(Utils::GetRandom(0.f, 45.f));
	}

	enterPkt.set_success(true);

	SEND_PACKET(enterPkt);

    */
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
