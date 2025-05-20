#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "Service.h"
#include "Session.h"

class GameSession : public Session
{
public:
	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		// Echo
		cout << "OnRecv Len = " << len << endl;
		Send(buffer, len);
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}
};

int main()
{
	// 난 NetAddr 클래스 안 만들어서 대신 SOCKADDR 하나 생성했음
	SOCKADDR_IN listenSock;
	listenSock.sin_family = AF_INET;
	listenSock.sin_addr.s_addr = htonl(0x7F000001); // 127.0.0.1
	listenSock.sin_port = ::htons(7777);

	//Listener listener;
	//listener.StartAccept(listenSock);

	ServerServiceRef service = make_shared<ServerService>(
		listenSock,
		make_shared<IocpCore>(),
		[]() { return make_shared<GameSession>();  },
		100
	);

	service->Start();

	for (int32 i = 0; i < 4; i++) // 코어개수 ~ (코어개수 * 1.5) 가 적당
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					GIocpCore.Dispatch();
				}
			});
	}

	GThreadManager->Join();
}