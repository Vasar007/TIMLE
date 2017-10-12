#ifndef DATATABLES_HPP
#define DATATABLES_HPP

#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>


// Forward declaration.

class Player;
class Enemy;

/**
 * \brief Additional namespace for convenience.
 */
namespace Type
{
	/**
	 * \brief Constant for counting hero types.
	 */
	const size_t HeroCount = 3;

	/**
	 * \brief Constant for counting enemy types.
	 */
	const size_t EnemyCount = 11;

	/**
	 * \brief Constant for counting projectiles types.
	 */
	const size_t ProjectilesCount = 4;

	/**
	 * \brief Identifiers of all entities.
	 */
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
		Flamestrike,
		Fireball,

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

/**
 * \brief Additional structure to work with direction of the some entities.
 */
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

/**
 * \brief Additional structure to work with teleporting entities.
 */
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

/**
 * \brief Additional structure to work with creating entities.
 */
struct EntityData
{
	size_t							hitpoints;
	size_t							damage;
	float							speed;
	Textures::ID					texture;
	sf::Time						fireInterval;
	std::vector<Direction>			directions;
};

/**
 * \brief Additional structure to work with boss of the first level.
 */
struct ShadowData
{
	size_t							hitpoints;
	size_t							damage;
	float							speed;
	Textures::ID					texture;
	sf::Time						timeInterval;
	std::vector<TeleportPoint>		teleportPoints;
};

/**
 * \brief Additional structure to work with mini-boss of the first level.
 */
struct GolemDarkData
{
	size_t							hitpoints;
	size_t							damage;
	float							speed;
	Textures::ID					texture;
	sf::Time						timeInterval;
};

/**
 * \brief Additional structure to work with projectiles' behaviour.
 */
struct ProjectileData
{
	size_t							damage;
	float							speed;
	Textures::ID					texture;
};

/**
 * \brief Additional structure to work with collectable items.
 */
struct PickupData
{
	std::function<void(Player&)>	action;
	Textures::ID					texture;
};

// Initialization block.

std::vector<EntityData>		initializeHeroData();
std::vector<EntityData>		initializeEnemyData();
ShadowData					initializeShadowData();
GolemDarkData				initializeGolemDarkData();
std::vector<ProjectileData>	initializeProjectileData();
std::vector<PickupData>		initializePickupData();

#endif // DATATABLES_HPP
