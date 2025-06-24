#include "pch.h"
#include "Object.h"

Object::Object()
{
	playerInfo = new Protocol::PlayerInfo();
	posInfo = new Protocol::PosInfo();
}

Object::~Object()
{
	//delete objectInfo;
}
