#include "pch.h"
#include "Listener.h"
//#include "Socket.h"
#include "SocketUtils.h"
#include "Session.h"
#include "IocpCore.h"
#include "Service.h"

/*--------------
	Listener
---------------*/

Listener::~Listener()
{
	::closesocket(_socket);

	for (AcceptEvent* acceptEvent : _acceptEvents)
	{
		// TODO

		delete(acceptEvent);
	}
}

// 클라이언트 연결 수락 시작
//bool Listener::StartAccept(SOCKADDR_IN sockaddr)
bool Listener::StartAccept(ServerServiceRef service)
{
	_service = service;
	if (_service == nullptr)
		return false;

	_socket = SocketUtils::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;

	if (_service->GetIocpCore()->Register(shared_from_this()) == false)
		return false;

	if (SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;

	if (SocketUtils::SetLinger(_socket, 0, 0) == false)
		return false;

	if (SocketUtils::Bind(_socket, _service->GetNetAddress()) == false)
		return false;

	if (SocketUtils::Listen(_socket) == false)
		return false;

	// 수락할 연결의 개수만큼 AcceptEvent를 등록
	const int32 acceptCount = 1;
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = new AcceptEvent();
		acceptEvent->owner = shared_from_this(); // iocpEvent의 주인이 누군지 지정(자기 자신)
		_acceptEvents.push_back(acceptEvent); // 이벤트 리스트에 추가 
		RegisterAccept(acceptEvent); // 수락 등록
	}

	return false;
}

void Listener::CloseSocket()
{
	::closesocket(_socket);
}

// 자기 자신의 소켓(리스너)를 반환하는 함수
HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

// IOCP 이벤트가 완료되었을 때 콜백 함수
void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent); // 연결 수락 처리
}

// 낚시대 
// 새로운 세션 생성 (IOCP에 등록)
void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	//Session* session = new Session();
	//SessionRef session = std::make_shared<Session>();
	SessionRef session = _service->CreateSession(); // Register IOCP

	acceptEvent->Init();
	//acceptEvent->SetSession(session);
	acceptEvent->session = session;
	DWORD bytesReceived = 0;
	// https://learn.microsoft.com/ko-kr/windows/win32/api/_winsock/
	// ★★★ 실제 연결이 되는 부분 AcceptEx()
	// ★★★ AcceptEx는 비동기 함수로써, 일단 접속을 대기하다가 
	//        LPOVERLAPPED 구조체인 acceptEvent에 완료 결과가 담긴다.
	// ★★★ acceptEvent는 IOCP 큐에 담기고, GetQueuedCompletionStatus()가 꺼내서 워커 스레드가 처리.
	if (false == SocketUtils::AcceptEx(_socket, session->GetSocket(),
		session->_recvBuffer.WritePos(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// 일단 다시 Accept 걸어준다
			RegisterAccept(acceptEvent);
		}
	}
}

// 실제 연결이 확정되고 세션을 등록하는 부분
void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	//Session* session = acceptEvent->GetSession();
	SessionRef session = acceptEvent->session;

	// 세션의 소켓 업데이트
	// 리스너의 소켓이랑 옵션을 똑같이 맞춰줌(SetUpdateAcceptSocket())
	if (false == SetSockOpt(session->GetSocket(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, _socket))
	{
		RegisterAccept(acceptEvent); // 실패 시 다시 수락 등록
		return;
	}

	// 클라이언트 주소 가져오기
	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(),
		OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	// 세션의 네트워크 주소 설정
	session->SetNetAddress(sockAddress);
	session->ProcessConnect();
	RegisterAccept(acceptEvent);
}