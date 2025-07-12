// GameSession.cpp
#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ServerPacketHandler.h"
#include "Player.h"
#include "Room.h"
#include "Protocol.pb.h"

void GameSession::OnConnected()
{
	// TODO 여기서 넘버링같은거 해줄 수 없나?
	cout<< "new Player Connected. Here is server\r\n";

	// 근데 이 패킷 목적 자체가 직업 선택 창 띄우려고 하는건데 의미가 없지 않은가?
	Protocol::S_Connected connectedPkt;
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(connectedPkt);
	Send(sendBuffer);
}

void GameSession::OnDisconnected()
{
	if (player.load() == nullptr)
		return;

	std::shared_ptr<Player> p = player.load();

	GRoom->DoAsync(&Room::HandleLeavePlayer, p);

	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	// TODO : packetId 대역 체크
	ServerPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(int32 len)
{

}