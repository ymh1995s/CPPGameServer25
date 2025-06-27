#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	Room();
	virtual ~Room();

public:
	void Init();
	bool EnterRoom(ObjectRef object);
	bool LeaveRoom(ObjectRef object);

	bool HandleEnterPlayer(PlayerRef player);
	bool HandleLeavePlayer(PlayerRef player);
	void HandleDamaged(PlayerRef player, Protocol::C_PlayerDamaged pkt);
	void HandleMove(PlayerRef player, Protocol::C_PlayerMove pkt);

public:
	void UpdateTick();

	RoomRef GetRoomRef();

private:
	bool AddObject(ObjectRef object);
	bool RemoveObject(uint64 objectId);

public:
	void Broadcast(SendBufferRef sendBuffer, int64 exceptId = 0);

public:
	// 몬스터 관련
	void MonsterInit();
	void MonsterSpawn();
	void MonsterUpdate();
	bool AddMonsterObject(MonsterRef monster);
	void MonsterEnterRoom(MonsterRef monster);
	void LeaveMonster(int objectId);
	void MonsterHitAndSetTarget(PlayerRef player, int monsterId, vector<int> damageAmounts);
	void RemoveMonster(int id);

public:
	// 몬스터랑 플레이어 나눴던가? 확인
	unordered_map<uint64, PlayerRef> _players;
	unordered_map<uint64, MonsterRef> _monsters;
};

extern RoomRef GRoom;