#pragma once
#include "IocpCore.h"

/*--------------
	Session
---------------*/

class Session : public IocpObject
{
public:
	Session();
	virtual ~Session();

public:
	/* 정보 관련 */
	void		SetNetAddress(SOCKADDR_IN address) { _netAddress = address; }
	SOCKADDR_IN	GetAddress() { return _netAddress; }
	SOCKET		GetSocket() { return _socket; }

public:
	/* 인터페이스 구현 */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

public:
	// TEMP
	char _recvBuffer[1000];

//private:
public: //temp
	SOCKET			_socket = INVALID_SOCKET;
	SOCKADDR_IN		_netAddress = {};
	Atomic<bool>	_connected = false;
};