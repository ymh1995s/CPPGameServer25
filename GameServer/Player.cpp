#include "pch.h"
#include "Player.h"

Player::Player()
{
	playerInfo = new Protocol::PlayerInfo();
	_isPlayer = true;
}

Player::~Player()
{
	delete playerInfo;
}
