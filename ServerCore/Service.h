#pragma once
#include "IocpCore.h"
#include "Listener.h"
#include <functional>

// 이 서비스는 서버인가? 클라이언트인가?
enum class ServiceType : uint8
{
	Server,
	Client
};

/*-------------
	Service
--------------*/

using SessionFactory = function<SessionRef(void)>;

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ServiceType type, SOCKADDR_IN address, IocpCoreRef core,
		SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~Service();

	virtual bool		Start() abstract;
	bool				CanStart() { return _sessionFactory != nullptr; }

	virtual void		CloseService();
	void				SetSessionFactory(SessionFactory func) { _sessionFactory = func; }

	SessionRef			CreateSession();
	void				AddSession(SessionRef session);
	void				ReleaseSession(SessionRef session);
	int32				GetCurrentSessionCount() { return _sessionCount; }
	int32				GetMaxSessionCount() { return _maxSessionCount; }

public:
	ServiceType			GetServiceType() { return _type; }
	SOCKADDR_IN			GetNetAddress() { return _netAddress; }
	IocpCoreRef& GetIocpCore() { return _iocpCore; }

protected:
	USE_LOCK;
	ServiceType			_type;
	SOCKADDR_IN			_netAddress = {};
	IocpCoreRef			_iocpCore;

	set<SessionRef>		_sessions;
	int32				_sessionCount = 0;
	int32				_maxSessionCount = 0;
	SessionFactory		_sessionFactory; // 함수 포인터
};

/*-----------------
	ClientService
------------------*/

class ClientService : public Service
{
public:
	ClientService(SOCKADDR_IN targetAddress, IocpCoreRef core, SessionFactory factory,
		int32 maxSessionCount = 1);
	virtual ~ClientService() {}

	virtual bool	Start() override;
};


/*-----------------
	ServerService
------------------*/

class ServerService : public Service
{
public:
	ServerService(SOCKADDR_IN targetAddress, IocpCoreRef core, SessionFactory factory,
		int32 maxSessionCount = 1);
	virtual ~ServerService() {}

	virtual bool	Start() override;
	virtual void	CloseService() override;

private:
	ListenerRef		_listener = nullptr;
};