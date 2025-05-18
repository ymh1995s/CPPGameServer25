#include "pch.h"
#include "Session.h"
#include "Socket.h"

/*--------------
	Session
---------------*/

Session::Session()
{
	_socket = GSocketManager->CreateSocket();
}

Session::~Session()
{
	closesocket(_socket);
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	// TODO
}