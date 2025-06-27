#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 직접 컨텐츠 작업자

bool Handle_S_CONNECTED(PacketSessionRef& session, Protocol::S_Connected& pkt)
{
	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_EnterGame& pkt)
{
	return true;
}

bool Handle_S_PLAYER_SPAWN(PacketSessionRef& session, Protocol::S_PlayerSpawn& pkt)
{
	return true;
}

bool Handle_S_MONSTER_SPAWN(PacketSessionRef& session, Protocol::S_MonsterSpawn& pkt)
{
	return true;
}

bool Handle_S_PLAYER_MOVE(PacketSessionRef& session, Protocol::S_PlayerMove& pkt)
{
	return true;
}

bool Handle_S_MONSTER_MOVE(PacketSessionRef& session, Protocol::S_MonsterMove& pkt)
{
	return true;
}

bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LeaveGame& pkt)
{
	return true;
}

bool Handle_S_PLAYER_DESPAWN(PacketSessionRef& session, Protocol::S_PlayerDespawn& pkt)
{
	return true;
}

bool Handle_S_MONSTER_DESPAWN(PacketSessionRef& session, Protocol::S_MonsterDespawn& pkt)
{
	return true;
}

bool Handle_S_ITEM_SPAWN(PacketSessionRef& session, Protocol::S_ItemSpawn& pkt)
{
	return true;
}

bool Handle_S_PLAYER_SKILL(PacketSessionRef& session, Protocol::S_PlayerSkill& pkt)
{
	return true;
}

bool Handle_S_MONSTER_SKILL(PacketSessionRef& session, Protocol::S_MonsterSkill& pkt)
{
	return true;
}

bool Handle_S_HIT_MONSTER(PacketSessionRef& session, Protocol::S_HitMonster& pkt)
{
	return true;
}

bool Handle_S_PLAYER_DAMAGED(PacketSessionRef& session, Protocol::S_PlayerDamaged& pkt)
{
	return true;
}

bool Handle_S_BOSS_REGISTER_DENY(PacketSessionRef& session, Protocol::S_BossRegisterDeny& pkt)
{
	return true;
}

bool Handle_S_BOSS_WAITING(PacketSessionRef& session, Protocol::S_BossWaiting& pkt)
{
	return true;
}

bool Handle_S_GAME_CLEAR(PacketSessionRef& session, Protocol::S_GameClear& pkt)
{
	return true;
}

bool Handle_S_GET_EXP(PacketSessionRef& session, Protocol::S_GetExp& pkt)
{
	return true;
}

bool Handle_S_LOOT_ITEM(PacketSessionRef& session, Protocol::S_LootItem& pkt)
{
	return true;
}

bool Handle_S_ITEM_DESPAWN(PacketSessionRef& session, Protocol::S_ItemDespawn& pkt)
{
	return true;
}
