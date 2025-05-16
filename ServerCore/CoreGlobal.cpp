#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Socket.h"

ThreadManager*		GThreadManager = nullptr;
Socket*				GSocketManager = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();

		// 강의에선 static을 사용해서 Init()따위로 초기화 하지만
		// 난 연습겸 동적할당으로 바꿈
		GSocketManager = new Socket();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		delete GSocketManager;
	}
} GCoreGlobal;