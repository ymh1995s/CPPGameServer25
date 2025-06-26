#include "pch.h"
#include "ObjectUtils.h"
#include "Player.h"
#include "Monster.h"
#include "GameSession.h"

atomic<int64> ObjectUtils::s_idGenerator = 1;

PlayerRef ObjectUtils::CreatePlayer(GameSessionRef session)
{
	// ID 생성기
	const int64 newId = s_idGenerator.fetch_add(1);

	PlayerRef player = make_shared<Player>();
	player->playerInfo->set_playerid(newId);

	player->id = newId;
	player->session = session;
	session->player.store(player);

	return player;
}

MonsterRef ObjectUtils::CreateMonster()
{
	// ID 생성기
	const int64 newId = s_idGenerator.fetch_add(1);

	MonsterRef monster = make_shared<Monster>();
	monster->id = newId;
	monster->monsterInfo->set_monsterid(newId);

	return monster;
}
