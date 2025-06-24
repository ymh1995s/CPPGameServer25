#include "pch.h"
#include "Object.h"

Object::Object()
{
	playerInfo = new Protocol::PlayerInfo();
}

Object::~Object()
{
	delete playerInfo;
}
