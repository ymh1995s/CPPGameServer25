#pragma once
#include "Object.h"

class GameSession;
class Room;

class Player : public Object
{
public:
	Player();
	virtual ~Player();

public:
	Protocol::PlayerInfo* playerInfo;

public:
	weak_ptr<GameSession> session;
};

