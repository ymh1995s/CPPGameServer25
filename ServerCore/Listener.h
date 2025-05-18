#pragma once
#include "IocpCore.h"

class AcceptEvent;

/*--------------
	Listener
---------------*/
// Listener는 Server Service의 일부로 동작하는 클래스
class Listener : public IocpObject
{
public:
	Listener() = default; // Listener 기본 생성자 호출 
	~Listener();

public:
	/* 외부에서 사용 */
	bool StartAccept(SOCKADDR_IN sockaddr);
	void CloseSocket();

public:
	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* 수신 관련 */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent); // 콜백 함수 

protected:
	SOCKET _socket = INVALID_SOCKET;
	vector<AcceptEvent*> _acceptEvents;
};