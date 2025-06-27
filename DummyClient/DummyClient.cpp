#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "SendBuffer.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"
#include <random>

class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected() override
	{
		Protocol::C_ClassChoice pkt;
		pkt.set_classtype(static_cast<Protocol::ClassType>(1));
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		Send(sendBuffer);
	}

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		PacketSessionRef session = GetPacketSessionRef();
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

		// TODO : packetId 대역 체크
		ClientPacketHandler::HandlePacket(session, buffer, len);
	}

	virtual void OnSend(int32 len) override
	{
		//cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		//cout << "Disconnected" << endl;
	}
};

int main()
{
	ClientPacketHandler::Init();

	this_thread::sleep_for(1s);

	SOCKADDR_IN sock;
	sock.sin_family = AF_INET;
	sock.sin_addr.s_addr = htonl(0x7F000001); // 127.0.0.1
	sock.sin_port = ::htons(7777);

	ClientServiceRef service = make_shared<ClientService>(
		sock,
		make_shared<IocpCore>(),
		[]() { return make_shared<ServerSession>(); },
		1); // N명 

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 1; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	Protocol::C_PlayerMove movePkt;

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> dist(-5.0f, 5.0f); // -5.0 ~ 5.0 사이

	while (true)
	{
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(movePkt);
		movePkt.set_positionx(dist(rng));
		service->Broadcast(sendBuffer);

		this_thread::sleep_for(100ms);
	}


	GThreadManager->Join();
}