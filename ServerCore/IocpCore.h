#pragma once

/*----------------
	IocpObject
-----------------*/

// 모든 IOCP 객체들은 이 클래스를 상속받아야 함 
// Listener.cpp에서 쉐어드 ptr을 쓰기 위한 상속 추가
// 자기 자신에 대한 스마트포인터를 사용 가능하게 함 
class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	// IOCP에 등록할 때 사용하는 소켓 핸들 반환 (예: 리스너 소켓 등)
	virtual HANDLE GetHandle() abstract;
	// IOCP 이벤트가 완료되었을 때 콜백 함수
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

/*--------------
	IocpCore
---------------*/

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	// IOCP 핸들 반환
	HANDLE		GetHandle() { return _iocpHandle; }

	// IOCP 객체 등록 (소켓 또는 파일 핸들 등)
	//bool		Register(class IocpObject* iocpObject);
	bool		Register(IocpObjectRef iocpObject);
	// IOCP 이벤트를 가져와 처리 (timeoutMs: 최대 대기 시간)
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		_iocpHandle; // CP를 가르키는 핸들이 됨
};

// TEMP 전역으로 사용 
extern IocpCore GIocpCore;


/*--------------
	IocpEvent
---------------*/

class Session;

enum class EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	Recv,
	Send
};

/*--------------
	IocpEvent
---------------*/

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);
	// 여기선 소멸자를 가상함수로 만들면 안된다.
	// 상속받았던 OVERLAPPED 메모리가 다른 정보로 채워질 수 있기 때문에 

	void		Init();
	EventType	GetType() { return eventType; }

public:
	EventType	eventType; // 이벤트 타입 (예: Connect, Disconnect, 등)
	IocpObjectRef	owner; // REFCount 위해 주인이 누구인지 기억용
};

/*----------------
	ConnectEvent
-----------------*/

class ConnectEvent : public IocpEvent
{
public:
	// 기본 생성자는 IocpEvent(EventType::Connect) 호출
	ConnectEvent() : IocpEvent(EventType::Connect) {}
};

/*--------------------
	DisconnectEvent
----------------------*/

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(EventType::Disconnect) {}
};

/*----------------
	AcceptEvent
-----------------*/

class AcceptEvent : public IocpEvent
{
public:
	// 기본 생성자는 IocpEvent(EventType::Accept) 호출
	AcceptEvent() : IocpEvent(EventType::Accept) {}

	//void		SetSession(Session* session) { session = session; }
	//Session* GetSession() { return session; }

public:
	//Session* _session = nullptr; // 연결된 세션(클라이언트)을 나타내는 참조
	SessionRef session = nullptr; // 연결된 세션(클라이언트)을 나타내는 참조
};

/*----------------
	RecvEvent
-----------------*/

class RecvEvent : public IocpEvent
{
public:
	// 기본 생성자는 IocpEvent(EventType::Recv) 호출
	RecvEvent() : IocpEvent(EventType::Recv) {}
};

/*----------------
	SendEvent
-----------------*/
class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) {}

	// TEMP
	vector<BYTE> buffer;
};