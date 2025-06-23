#pragma once
#include "Protocol.pb.h"

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
#include "S1.h"
#endif

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
bool Handle_C_CLASS_CHOICE(PacketSessionRef& session, Protocol::C_ClassChoice& pkt);
bool Handle_C_PLAYER_MOVE(PacketSessionRef& session, Protocol::C_PlayerMove& pkt);
bool Handle_C_PLAYER_DIE(PacketSessionRef& session, Protocol::C_PlayerDie& pkt);
bool Handle_C_PLAYER_SKILL(PacketSessionRef& session, Protocol::C_PlayerSkill& pkt);
bool Handle_C_HIT_MONSTER(PacketSessionRef& session, Protocol::C_HitMonster& pkt);
bool Handle_C_PLAYER_DAMAGED(PacketSessionRef& session, Protocol::C_PlayerDamaged& pkt);
bool Handle_C_CHANGE_MAP(PacketSessionRef& session, Protocol::C_ChangeMap& pkt);
bool Handle_C_BOSS_REGISTER(PacketSessionRef& session, Protocol::C_BossRegister& pkt);
bool Handle_C_BOSS_CANCLE(PacketSessionRef& session, Protocol::C_BossCancle& pkt);
bool Handle_C_LOOT_ITEM(PacketSessionRef& session, Protocol::C_LootItem& pkt);

class ServerPacketHandler
{
public:
	static void Init()
	{
		GPacketHandler[C_CLASS_CHOICE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ClassChoice>(Handle_C_CLASS_CHOICE, session, buffer, len); };
		GPacketHandler[C_PLAYER_MOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_PlayerMove>(Handle_C_PLAYER_MOVE, session, buffer, len); };
		GPacketHandler[C_PLAYER_DIE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_PlayerDie>(Handle_C_PLAYER_DIE, session, buffer, len); };
		GPacketHandler[C_PLAYER_SKILL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_PlayerSkill>(Handle_C_PLAYER_SKILL, session, buffer, len); };
		GPacketHandler[C_HIT_MONSTER] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_HitMonster>(Handle_C_HIT_MONSTER, session, buffer, len); };
		GPacketHandler[C_PLAYER_DAMAGED] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_PlayerDamaged>(Handle_C_PLAYER_DAMAGED, session, buffer, len); };
		GPacketHandler[C_CHANGE_MAP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ChangeMap>(Handle_C_CHANGE_MAP, session, buffer, len); };
		GPacketHandler[C_BOSS_REGISTER] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_BossRegister>(Handle_C_BOSS_REGISTER, session, buffer, len); };
		GPacketHandler[C_BOSS_CANCLE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_BossCancle>(Handle_C_BOSS_CANCLE, session, buffer, len); };
		GPacketHandler[C_LOOT_ITEM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_LootItem>(Handle_C_LOOT_ITEM, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

	static SendBufferRef MakeSendBuffer(Protocol::S_Connected& pkt) { return MakeSendBuffer(pkt, S_CONNECTED); }
	static SendBufferRef MakeSendBuffer(Protocol::S_EnterGame& pkt) { return MakeSendBuffer(pkt, S_ENTER_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PlayerSpawn& pkt) { return MakeSendBuffer(pkt, S_PLAYER_SPAWN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_MonsterSpawn& pkt) { return MakeSendBuffer(pkt, S_MONSTER_SPAWN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PlayerMove& pkt) { return MakeSendBuffer(pkt, S_PLAYER_MOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_MonsterMove& pkt) { return MakeSendBuffer(pkt, S_MONSTER_MOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LeaveGame& pkt) { return MakeSendBuffer(pkt, S_LEAVE_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PlayerDespawn& pkt) { return MakeSendBuffer(pkt, S_PLAYER_DESPAWN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_MonsterDespawn& pkt) { return MakeSendBuffer(pkt, S_MONSTER_DESPAWN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ItemSpawn& pkt) { return MakeSendBuffer(pkt, S_ITEM_SPAWN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PlayerSkill& pkt) { return MakeSendBuffer(pkt, S_PLAYER_SKILL); }
	static SendBufferRef MakeSendBuffer(Protocol::S_MonsterSkill& pkt) { return MakeSendBuffer(pkt, S_MONSTER_SKILL); }
	static SendBufferRef MakeSendBuffer(Protocol::S_HitMonster& pkt) { return MakeSendBuffer(pkt, S_HIT_MONSTER); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PlayerDamaged& pkt) { return MakeSendBuffer(pkt, S_PLAYER_DAMAGED); }
	static SendBufferRef MakeSendBuffer(Protocol::S_BossRegisterDeny& pkt) { return MakeSendBuffer(pkt, S_BOSS_REGISTER_DENY); }
	static SendBufferRef MakeSendBuffer(Protocol::S_BossWaiting& pkt) { return MakeSendBuffer(pkt, S_BOSS_WAITING); }
	static SendBufferRef MakeSendBuffer(Protocol::S_GameClear& pkt) { return MakeSendBuffer(pkt, S_GAME_CLEAR); }
	static SendBufferRef MakeSendBuffer(Protocol::S_GetExp& pkt) { return MakeSendBuffer(pkt, S_GET_EXP); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LootItem& pkt) { return MakeSendBuffer(pkt, S_LOOT_ITEM); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ItemDespawn& pkt) { return MakeSendBuffer(pkt, S_ITEM_DESPAWN); }

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

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
		SendBufferRef sendBuffer = MakeShared<SendBuffer>(packetSize);
#else
		SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);
#endif

		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		pkt.SerializeToArray(&header[1], dataSize);
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};