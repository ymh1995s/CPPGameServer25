#pragma once
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	S_CONNECTED = 0,
	C_CLASS_CHOICE = 1,
	S_ENTER_GAME = 2,
	S_PLAYER_SPAWN = 3,
	S_MONSTER_SPAWN = 4,
	C_PLAYER_MOVE = 5,
	S_PLAYER_MOVE = 6,
	S_MONSTER_MOVE = 7,
	C_PLAYER_DIE = 8,
	S_LEAVE_GAME = 9,
	S_PLAYER_DESPAWN = 10,
	S_MONSTER_DESPAWN = 11,
	S_ITEM_SPAWN = 12,
	C_PLAYER_SKILL = 13,
	S_PLAYER_SKILL = 14,
	S_MONSTER_SKILL = 15,
	C_HIT_MONSTER = 16,
	S_HIT_MONSTER = 17,
	C_PLAYER_DAMAGED = 18,
	S_PLAYER_DAMAGED = 19,
	C_CHANGE_MAP = 20,
	C_BOSS_REGISTER = 21,
	S_BOSS_REGISTER_DENY = 22,
	S_BOSS_WAITING = 23,
	C_BOSS_CANCLE = 24,
	S_GAME_CLEAR = 25,
	S_GET_EXP = 26,
	C_LOOT_ITEM = 27,
	S_LOOT_ITEM = 28,
	S_ITEM_DESPAWN = 29
};

// Custom Handlers
bool Handle_S_CONNECTED(PacketSessionRef& session, Protocol::S_Connected& pkt);
bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_EnterGame& pkt);
bool Handle_S_PLAYER_SPAWN(PacketSessionRef& session, Protocol::S_PlayerSpawn& pkt);
bool Handle_S_MONSTER_SPAWN(PacketSessionRef& session, Protocol::S_MonsterSpawn& pkt);
bool Handle_S_PLAYER_MOVE(PacketSessionRef& session, Protocol::S_PlayerMove& pkt);
bool Handle_S_MONSTER_MOVE(PacketSessionRef& session, Protocol::S_MonsterMove& pkt);
bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LeaveGame& pkt);
bool Handle_S_PLAYER_DESPAWN(PacketSessionRef& session, Protocol::S_PlayerDespawn& pkt);
bool Handle_S_MONSTER_DESPAWN(PacketSessionRef& session, Protocol::S_MonsterDespawn& pkt);
bool Handle_S_ITEM_SPAWN(PacketSessionRef& session, Protocol::S_ItemSpawn& pkt);
bool Handle_S_PLAYER_SKILL(PacketSessionRef& session, Protocol::S_PlayerSkill& pkt);
bool Handle_S_MONSTER_SKILL(PacketSessionRef& session, Protocol::S_MonsterSkill& pkt);
bool Handle_S_HIT_MONSTER(PacketSessionRef& session, Protocol::S_HitMonster& pkt);
bool Handle_S_PLAYER_DAMAGED(PacketSessionRef& session, Protocol::S_PlayerDamaged& pkt);
bool Handle_S_BOSS_REGISTER_DENY(PacketSessionRef& session, Protocol::S_BossRegisterDeny& pkt);
bool Handle_S_BOSS_WAITING(PacketSessionRef& session, Protocol::S_BossWaiting& pkt);
bool Handle_S_GAME_CLEAR(PacketSessionRef& session, Protocol::S_GameClear& pkt);
bool Handle_S_GET_EXP(PacketSessionRef& session, Protocol::S_GetExp& pkt);
bool Handle_S_LOOT_ITEM(PacketSessionRef& session, Protocol::S_LootItem& pkt);
bool Handle_S_ITEM_DESPAWN(PacketSessionRef& session, Protocol::S_ItemDespawn& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		GPacketHandler[S_CONNECTED] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_Connected>(Handle_S_CONNECTED, session, buffer, len); };
		GPacketHandler[S_ENTER_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_EnterGame>(Handle_S_ENTER_GAME, session, buffer, len); };
		GPacketHandler[S_PLAYER_SPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PlayerSpawn>(Handle_S_PLAYER_SPAWN, session, buffer, len); };
		GPacketHandler[S_MONSTER_SPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MonsterSpawn>(Handle_S_MONSTER_SPAWN, session, buffer, len); };
		GPacketHandler[S_PLAYER_MOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PlayerMove>(Handle_S_PLAYER_MOVE, session, buffer, len); };
		GPacketHandler[S_MONSTER_MOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MonsterMove>(Handle_S_MONSTER_MOVE, session, buffer, len); };
		GPacketHandler[S_LEAVE_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LeaveGame>(Handle_S_LEAVE_GAME, session, buffer, len); };
		GPacketHandler[S_PLAYER_DESPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PlayerDespawn>(Handle_S_PLAYER_DESPAWN, session, buffer, len); };
		GPacketHandler[S_MONSTER_DESPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MonsterDespawn>(Handle_S_MONSTER_DESPAWN, session, buffer, len); };
		GPacketHandler[S_ITEM_SPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ItemSpawn>(Handle_S_ITEM_SPAWN, session, buffer, len); };
		GPacketHandler[S_PLAYER_SKILL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PlayerSkill>(Handle_S_PLAYER_SKILL, session, buffer, len); };
		GPacketHandler[S_MONSTER_SKILL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MonsterSkill>(Handle_S_MONSTER_SKILL, session, buffer, len); };
		GPacketHandler[S_HIT_MONSTER] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_HitMonster>(Handle_S_HIT_MONSTER, session, buffer, len); };
		GPacketHandler[S_PLAYER_DAMAGED] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PlayerDamaged>(Handle_S_PLAYER_DAMAGED, session, buffer, len); };
		GPacketHandler[S_BOSS_REGISTER_DENY] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_BossRegisterDeny>(Handle_S_BOSS_REGISTER_DENY, session, buffer, len); };
		GPacketHandler[S_BOSS_WAITING] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_BossWaiting>(Handle_S_BOSS_WAITING, session, buffer, len); };
		GPacketHandler[S_GAME_CLEAR] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_GameClear>(Handle_S_GAME_CLEAR, session, buffer, len); };
		GPacketHandler[S_GET_EXP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_GetExp>(Handle_S_GET_EXP, session, buffer, len); };
		GPacketHandler[S_LOOT_ITEM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LootItem>(Handle_S_LOOT_ITEM, session, buffer, len); };
		GPacketHandler[S_ITEM_DESPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ItemDespawn>(Handle_S_ITEM_DESPAWN, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

	static SendBufferRef MakeSendBuffer(Protocol::C_ClassChoice& pkt) { return MakeSendBuffer(pkt, C_CLASS_CHOICE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_PlayerMove& pkt) { return MakeSendBuffer(pkt, C_PLAYER_MOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_PlayerDie& pkt) { return MakeSendBuffer(pkt, C_PLAYER_DIE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_PlayerSkill& pkt) { return MakeSendBuffer(pkt, C_PLAYER_SKILL); }
	static SendBufferRef MakeSendBuffer(Protocol::C_HitMonster& pkt) { return MakeSendBuffer(pkt, C_HIT_MONSTER); }
	static SendBufferRef MakeSendBuffer(Protocol::C_PlayerDamaged& pkt) { return MakeSendBuffer(pkt, C_PLAYER_DAMAGED); }
	static SendBufferRef MakeSendBuffer(Protocol::C_ChangeMap& pkt) { return MakeSendBuffer(pkt, C_CHANGE_MAP); }
	static SendBufferRef MakeSendBuffer(Protocol::C_BossRegister& pkt) { return MakeSendBuffer(pkt, C_BOSS_REGISTER); }
	static SendBufferRef MakeSendBuffer(Protocol::C_BossCancle& pkt) { return MakeSendBuffer(pkt, C_BOSS_CANCLE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_LootItem& pkt) { return MakeSendBuffer(pkt, C_LOOT_ITEM); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};