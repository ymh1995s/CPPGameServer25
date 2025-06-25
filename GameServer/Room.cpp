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

		for (auto& item : _objects)
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
	if (_objects.find(playerId) == _objects.end())
		return;

	// 서버에서 관리하기 위해 데이터 반영
	PlayerRef p = dynamic_pointer_cast<Player>(_objects[playerId]);
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
	cout << "Update Room" << endl;

	DoTimer(100, &Room::UpdateTick);
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

	_objects.insert(make_pair(player->playerInfo->playerid(), object));

	object->room.store(GetRoomRef());

	return true;
}

bool Room::RemoveObject(uint64 objectId)
{
	// 없다면 문제가 있다.
	if (_objects.find(objectId) == _objects.end())
		return false;

	ObjectRef object = _objects[objectId];
	PlayerRef player = dynamic_pointer_cast<Player>(object);
	if (player)
		player->room.store(weak_ptr<Room>());

	_objects.erase(objectId);

	return true;
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{
	for (auto& item : _objects)
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