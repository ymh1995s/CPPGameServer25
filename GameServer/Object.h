#pragma once


class Object : public enable_shared_from_this<Object>
{
public:
	Object();
	virtual ~Object();

	bool IsPlayer() { return _isPlayer; }


public:
	// 왜 아토믹이지?
	atomic<weak_ptr<Room>> room;

public:
	int id;

protected:
	bool _isPlayer = false;
};

