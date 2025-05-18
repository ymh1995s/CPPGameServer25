#pragma once

class Socket
{

public:
	Socket();
	~Socket();

	// 소켓 관련
public:
	void SocketStart();

	SOCKET CreateSocket();
	bool BindSocket(SOCKET socket, SOCKADDR_IN sockaddr);
	bool BindSocketAnyAddress(SOCKET socket, uint16 port);
	bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);

	bool Listen(SOCKET socket, int32 backlog = SOMAXCONN);
	void CloseSocket(SOCKET& socket);

	// 확장 포인터
	// 비동기 Connect / DisConnect / Accept 사용 전 런타임에서 주소 얻어오기
	// 강의에선 static을 붙였는데 난 연습겸 생성자에서 초기화 해주자
	LPFN_CONNECTEX		ConnectEx;
	LPFN_DISCONNECTEX	DisconnectEx;
	LPFN_ACCEPTEX		AcceptEx;
};

// setsockopt();
// 첫 번째 파라미터 : 설정할 소켓
// 두 번째 파라미터
	// 옵션을 해석하고 처리할 주체?
	// 소켓 코드 -> SOL_SOCKET
	// IPv4 -> IPPROTO_IP
	// TCP 프로토콜 -> IPPROTO_TCP
// 세 번째 파라미터 
	// SO_KEEPALIVE = 주기적으로 연결 상태 확인 여부 (TCP ONLY)
		// 상대방이 조용히 연결을 끊는다면?
		// 주기적으로 TCP 프로토콜 연결 상태 확인 -> 끊어진 연결 탐지 

template<typename T>
static inline bool SetSockOpt(SOCKET socket, int32 level, int32 optName, T optVal)
{
	return SOCKET_ERROR != ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T));
}