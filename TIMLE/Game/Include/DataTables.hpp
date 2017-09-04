#ifndef DATATABLES_HPP
#define DATATABLES_HPP

#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>


class Player;
class Enemy;

namespace Type
{
	const size_t HeroCount = 3;
	const size_t EnemyCount = 11;
	const size_t ProjectilesCount = 3;

	enum ID
	{
		Archer,
		Swordsman,
		Mage,

		Ghost,
		Golem,
		DarkSoldier,
		Goblin,
		MinotaurMage,
		Dwarf,
		DwarfArcher,
		DwarvenCommander,
		Shadow,
		GolemDark,
		Tentacle,

		AlliedBullet,
		EnemyBullet,
		FireBall,

		MovingPlatform,
		Rock,
		OpeningGate,
		ClosingGate,
		OpenClosingGate,

		HealthBar,
		ManaBar,
		ShadowBossBar,
		GolemDarkBossBar,
		Oswald,
		Heinrich,
		DeadJuggernaut,
		DeadDwarf,
		TypeCount
	};
}

struct Direction
{
	Direction(float angle, float distance)
	: angle(angle)
	, distance(distance)
	{
	}

	float angle;
	float distance;
};

struct TeleportPoint
{
	TeleportPoint(float x, float y)
	: x(x)
	, y(y)
	{
	}

	float		x;
	float		y;
};

struct EntityData
{
	int								hitpoints;
	int								damage;
	float							speed;
	Textures::ID					texture;
	sf::Time						fireInterval;
	std::vector<Direction>			directions;
};

struct ShadowData
{
	int								hitpoints;
	int								damage;
	float							speed;
	Textures::ID					texture;
	sf::Time						timeInterval;
	std::vector<TeleportPoint>		teleportPoints;
};

struct GolemDarkData
{
	int								hitpoints;
	int								damage;
	float							speed;
	Textures::ID					texture;
	sf::Time						timeInterval;
};

struct ProjectileData
{
	int								damage;
	float							speed;
	Textures::ID					texture;
};

struct PickupData
{
	std::function<void(Player&)>	action;
	Textures::ID					texture;
};


std::vector<EntityData>		initializeHeroData();
std::vector<EntityData>		initializeEnemyData();
ShadowData					initializeShadowData();
GolemDarkData				initializeGolemDarkData();
std::vector<ProjectileData>	initializeProjectileData();
std::vector<PickupData>		initializePickupData();

#endif // DATATABLES_HPP
