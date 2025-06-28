﻿#pragma once
#include "IocpCore.h"
#include "RecvBuffer.h"

/*--------------
	Session
---------------*/
class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;

	enum
	{
		BUFFER_SIZE = 0x10000, // 64KB
	};

public:
	Session();
	virtual ~Session();

public:
	/* 외부에서 사용 */
	//void				Send(BYTE* buffer, int32 len);
	void				Send(SendBufferRef sendBuffer);
	bool				Connect();
	void				Disconnect(const WCHAR* cause);

	shared_ptr<Service>	GetService() { return _service.lock(); }
	void				SetService(shared_ptr<Service> service) { _service = service; }

public:
	/* 정보 관련 */
	void				SetNetAddress(SOCKADDR_IN address) { _netAddress = address; }
	SOCKADDR_IN			GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	bool				IsConnected() { return _connected; }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

private:
	/* 인터페이스 구현 */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* 전송 관련 */
	bool				RegisterConnect();
	bool				RegisterDisconnect();
	void				RegisterRecv();
	//void				RegisterSend(SendEvent* sendEvent);
	void				RegisterSend();

	void				ProcessConnect();
	void				ProcessDisconnect();
	void				ProcessRecv(int32 numOfBytes);
	//void				ProcessSend(SendEvent* sendEvent, int32 numOfBytes);
	void				ProcessSend(int32 numOfBytes);

	void				HandleError(int32 errorCode);

protected:
	/* 컨텐츠 코드에서 오버로딩 => 서비스 영역*/ 
	virtual void		OnConnected() {}
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) {}
	virtual void		OnDisconnected() {}

private:
	weak_ptr<Service>	_service;
	SOCKET				_socket = INVALID_SOCKET;
	SOCKADDR_IN			_netAddress = {};
	Atomic<bool>		_connected = false;

private:
	USE_LOCK;

	/* 수신 관련 */
	RecvBuffer			_recvBuffer;

	/* 송신 관련 */
	queue<SendBufferRef>	_sendQueue;
	Atomic<bool>			_sendRegistered = false;

private:
	/* IocpEvent 재사용 */
	ConnectEvent		_connectEvent;
	DisconnectEvent		_disconnectEvent;
	RecvEvent			_recvEvent; // 한 번만 선언하고 재사용 (낚시대)
	SendEvent			_sendEvent; // 한 번만 선언하고 재사용
};
