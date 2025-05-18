#include "pch.h"
#include "Socket.h"


Socket::Socket()
{
	// 0. 윈속 초기화
	SocketStart();
	// 1. 확장 포인터 바인딩 (런타임에)
	SOCKET dummySocket = CreateSocket();
	BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx));
	BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx));
	BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx));
	CloseSocket(dummySocket);
}

void Socket::SocketStart()
{
	// 0. 윈속 초기화
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), OUT & wsaData);
}

Socket::~Socket()
{
	WSACleanup(); // 원칙상 WSAStartup의 개수와 맞아 떨어져야 함
}

SOCKET Socket::CreateSocket()
{
	// 기존에 socket() 함수로 SOCKET을 리턴했던 반면,
	// 윈도우 전용확장 함수인 WSASocket()으로 대체됨 -> 세부 옵션 설정 가능
	// return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKET sock = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
	{
		int errCode = WSAGetLastError();
		std::cout << "[CreateSocket] 실패: " << errCode << std::endl;
	}
	return sock;
}

bool Socket::BindSocket(SOCKET socket, SOCKADDR_IN sockaddr)
{
	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&sockaddr), sizeof(sockaddr));
}

bool Socket::BindSocketAnyAddress(SOCKET socket, uint16 port)
{
	SOCKADDR_IN myAddress;
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddress.sin_port = ::htons(port);

	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&myAddress), sizeof(myAddress));
}

bool Socket::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != ::listen(socket, backlog);
}

void Socket::CloseSocket(SOCKET& socket)
{
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}

bool Socket::BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
{
	DWORD bytes = 0;
	// 첫 번째 파라미터 socket 은 더미 소켓을 넣어준다.
	// Connect Disconnect Accept를 런타임에 바인디 해준다라고만 알면 됨
	return SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}
