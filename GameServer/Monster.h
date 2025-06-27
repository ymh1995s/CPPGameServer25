#pragma once
#include "Object.h"
#include <random>

using namespace std::chrono;

enum class MonsterState
{
	Idle = 0,
	Move = 1,
	Stun = 2,
	Skill = 3,
	Dead = 4,
};

struct Vector2
{
	float x = 0;
	float y = 0;

	Vector2() = default;
	Vector2(float _x, float _y) : x(_x), y(_y) {}
};

class Monster : public Object
{
public:
	Monster();
	virtual ~Monster();
public:
	void Update();
	void UpdateThink();
	void UpdateTarget();
	void UpdateIdle();
	void UpdateMove();
	void UpdateStun();
	void UpdateSkill();
	void UpdateDead();
	void UpdateInfo();
	void BroadcastMove();
	void Think();
	void TakeDamage(int playerId, vector<int> damageAmounts, bool forceDamage=false);
	void SetTarget(PlayerRef newTarget);

public:
	Protocol::MonsterInfo* monsterInfo;
	MonsterState _currentState;

private:
	// 랜덤 관련 변수
	std::mt19937 rng{ std::random_device{}() }; // 멤버 변수 선언 시 직접 초기화
	std::uniform_int_distribution<int> dist{ 0, 1 }; // 정수 범위 분포

private:
	weak_ptr<Player> _target;

	Vector2 _destinationPos{ 0.0f, 0.75f };
	bool _isRight = false;

	// 이동 가능한 범위
	float _minX = -3;
	float _maxX = 8;

	// 랜덤한 Action을 선택하는 Think 함수에서 사용될 변수
	steady_clock::time_point _lastThinkTime;
	float _thinkInterval;

	// Stun
	steady_clock::time_point _stunStartTime;
	float _stunDuration = 2.0f;

	// Skill
	steady_clock::time_point _skillStartTime;
	float _skillDuration = 3.5f;
};

