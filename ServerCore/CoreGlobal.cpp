#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
//#include "Socket.h"
#include "SocketUtils.h"
#include "SendBuffer.h"
#include "GlobalQueue.h"
#include "JobTimer.h"

ThreadManager*		GThreadManager = nullptr;
GlobalQueue* GGlobalQueue = nullptr;
//Socket*				GSocketManager = nullptr;
JobTimer* GJobTimer = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GGlobalQueue = new GlobalQueue();
		GJobTimer = new JobTimer();

		// 강의에선 static을 사용해서 Init()따위로 초기화 하지만
		// 난 연습겸 동적할당으로 바꿈
		//GSocketManager = new Socket();
		SocketUtils::Init();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		delete GGlobalQueue;
		delete GJobTimer;
		SocketUtils::Clear();
	}
} GCoreGlobal;