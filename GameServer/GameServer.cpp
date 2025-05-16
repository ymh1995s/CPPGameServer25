#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include "Socket.h"


#include "Memory.h"

int main()
{
	// 1. 윈속 초기화
	GSocketManager->SocketStart();

	// 2. 리스너 소켓 생성 및 바인딩
	SOCKET socket = GSocketManager->CreateSocket();
	GSocketManager->BindSocketAnyAddress(socket, 7777);

	// 3. 클라이언트 소켓 생성
	SOCKET clientSocket = accept(socket, nullptr, nullptr);

	cout << "Client Connected!" << endl;
	while (true)
	{

	}

	GThreadManager->Join();
}