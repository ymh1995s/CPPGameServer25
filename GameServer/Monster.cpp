#include "pch.h"
#include "Monster.h"

Monster::Monster()
{
	monsterInfo = new Protocol::MonsterInfo();
}

Monster::~Monster()
{
	delete monsterInfo;
}