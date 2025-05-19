#include "pch.h"
#include "IocpCore.h"

/*--------------
	IocpCore
---------------*/

// TEMP
IocpCore GIocpCore;

IocpCore::IocpCore()
{
	// 생성자에서 CP 생성
	_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	// CP 해제
	::CloseHandle(_iocpHandle);
}

// IOCP 객체(소켓 등)를 IOCP에 등록
//bool IocpCore::Register(IocpObject* iocpObject)
bool IocpCore::Register(IocpObjectRef iocpObject)
{
	//return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, /*key*/reinterpret_cast<ULONG_PTR>(iocpObject), 0);
	// key는 고유한 식별 값이고, 고유 값을 넣기 위해 해당 객체의 포인터를 넣음

	// 이젠 키값 대신 share_ptr로 관리할 것이기 떄문에 키 관련은 0또는 더미로 밀어준다.
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, /*key*/0, 0);
}

/*
	IOCP 이벤트 처리:
	- `GetQueuedCompletionStatus`를 호출하여 완료된 IO 이벤트를 가져온다.
	- 이벤트가 있으면 해당 `IocpEvent` 객체를 찾고, 연결된 `IocpObject`의 `Dispatch` 함수를 호출하여 이벤트 처리.
	- 에러 발생 시 에러 코드에 따라 처리 (예: 타임아웃 발생 시 false 반환).

	IOCP 이벤트 : connect diconnect recv send 등
*/
bool IocpCore::Dispatch(uint32 timeoutMs)
{
	DWORD numOfBytes = 0;
	//IocpObject* iocpObject = nullptr;
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr;

	// 무한 대기 하다 완료된 IO 이벤트 가져오기
	// OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent) 인자로 완료된 iocp 이벤트를 가져온다.
	// iocpObject, iocpEvent에 데이터 복원
	// iocpObject (3번째 파라미터) : 등록할 때 넣은 키 값
	// iocpEvent (4번째 파라미터) : OVERLAPPED 구조체 포인터 => IocpEvent는 OVERLAPPED 구조체를 상속 받음
	if (::GetQueuedCompletionStatus(_iocpHandle, OUT & numOfBytes,
		//OUT reinterpret_cast<PULONG_PTR>(&iocpObject),
		OUT & key,
		OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		IocpObjectRef iocpObject = iocpEvent->owner; // 여기서 iocpEvent의 주인을 가져옴 
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = ::WSAGetLastError();
		switch (errCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// TODO : 로그 찍기
			IocpObjectRef iocpObject = iocpEvent->owner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}

/*--------------
	IocpEvent
---------------*/
IocpEvent::IocpEvent(EventType type) : eventType(type)
{
	Init(); // OVERLAPPED 구조체 초기화
}

void IocpEvent::Init()
{
	OVERLAPPED::hEvent = 0;
	OVERLAPPED::Internal = 0;
	OVERLAPPED::InternalHigh = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;
}
