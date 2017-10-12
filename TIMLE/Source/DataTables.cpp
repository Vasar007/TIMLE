#include "../Include/DataTables.hpp"
#include "../Include/Player.hpp"
#include "../Include/Enemy.hpp"
#include "../Include/Projectile.hpp"
#include "../Include/Pickup.hpp"


/*
 * Constant values of width and height for every object:
 *		– Ghost:					w=51,		h=36
 *		– Golem:					w=45,		h=38
 *		– DarkSoldier:				w=20,		h=30
 *		– Goblin:					w=25,		h=28
 *		– MinotaurMage:				w=40,		h=40
 *		– Dwarf (all):				w=40,		h=27
 *		– Tentacle:					w=13,		h=45
 *		– Rock:						w=type*16,	h=16
 *		– Gate (all):				w=16,		h=type*16
 *		– DialogPerson (Oswald):	w=16,		h=20
 *		– DialogPerson (Heinrich):	w=29,		h=32
 *		– MovingPlatform:			w=48,		h=11
 *		– DeadMan (DeadJuggernaut):	w=35,		h=15
 *		– DeadMan (DeadDwarf):		w=25,		h=15
 */

// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<EntityData> initializeHeroData()
{
	std::vector<EntityData> data(Type::HeroCount);

	data[Type::Archer].hitpoints = 1000;
	data[Type::Archer].damage = 0;
	data[Type::Archer].speed = 200.f;
	data[Type::Archer].fireInterval = sf::seconds(1);
	data[Type::Archer].texture = Textures::Archer;

	return data;
}

std::vector<EntityData> initializeEnemyData()
{
	std::vector<EntityData> data(Type::EnemyCount);

	data[Type::Ghost - Type::HeroCount].hitpoints = 50;
	data[Type::Ghost - Type::HeroCount].damage = 50;
	data[Type::Ghost - Type::HeroCount].speed = 80.f;
	data[Type::Ghost - Type::HeroCount].texture = Textures::Ghost;
	data[Type::Ghost - Type::HeroCount].fireInterval = sf::Time::Zero;

	data[Type::Golem - Type::HeroCount].hitpoints = 125;
	data[Type::Golem - Type::HeroCount].damage = 50;
	data[Type::Golem - Type::HeroCount].speed = 50.f;
	data[Type::Golem - Type::HeroCount].texture = Textures::Golem;
	data[Type::Golem - Type::HeroCount].fireInterval = sf::Time::Zero;

	data[Type::DarkSoldier - Type::HeroCount].hitpoints = 1000;
	data[Type::DarkSoldier - Type::HeroCount].damage = 0;
	data[Type::DarkSoldier - Type::HeroCount].speed = 50.f;
	data[Type::DarkSoldier - Type::HeroCount].texture = Textures::DarkSoldier;
	data[Type::DarkSoldier - Type::HeroCount].fireInterval = sf::Time::Zero;

	data[Type::Goblin - Type::HeroCount].hitpoints = 50;
	data[Type::Goblin - Type::HeroCount].damage = 30;
	data[Type::Goblin - Type::HeroCount].speed = 50.f;
	data[Type::Goblin - Type::HeroCount].texture = Textures::Goblin;
	data[Type::Goblin - Type::HeroCount].fireInterval = sf::Time::Zero;

	data[Type::MinotaurMage - Type::HeroCount].hitpoints = 150;
	data[Type::MinotaurMage - Type::HeroCount].damage = 50;
	data[Type::MinotaurMage - Type::HeroCount].speed = 50.f;
	data[Type::MinotaurMage - Type::HeroCount].texture = Textures::MinotaurMage;
	data[Type::MinotaurMage - Type::HeroCount].fireInterval = sf::seconds(2);

	data[Type::Dwarf - Type::HeroCount].hitpoints = 75;
	data[Type::Dwarf - Type::HeroCount].damage = 20;
	data[Type::Dwarf - Type::HeroCount].speed = 50.f;
	data[Type::Dwarf - Type::HeroCount].texture = Textures::Dwarf;
	data[Type::Dwarf - Type::HeroCount].fireInterval = sf::Time::Zero;

	data[Type::DwarfArcher - Type::HeroCount].hitpoints = 75;
	data[Type::DwarfArcher - Type::HeroCount].damage = 20;
	data[Type::DwarfArcher - Type::HeroCount].speed = 50.f;
	data[Type::DwarfArcher - Type::HeroCount].texture = Textures::Dwarf;
	data[Type::DwarfArcher - Type::HeroCount].fireInterval = sf::seconds(2);

	data[Type::DwarvenCommander - Type::HeroCount].hitpoints = 100;
	data[Type::DwarvenCommander - Type::HeroCount].damage = 20;
	data[Type::DwarvenCommander - Type::HeroCount].speed = 50.f;
	data[Type::DwarvenCommander - Type::HeroCount].texture = Textures::Dwarf;
	data[Type::DwarvenCommander - Type::HeroCount].fireInterval = sf::Time::Zero;

	data[Type::Tentacle - Type::HeroCount].hitpoints = 100;
	data[Type::Tentacle - Type::HeroCount].damage = 10;
	data[Type::Tentacle - Type::HeroCount].speed = 50.f;
	data[Type::Tentacle - Type::HeroCount].texture = Textures::Tentacle;
	data[Type::Tentacle - Type::HeroCount].fireInterval = sf::Time::Zero;

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Type::ProjectilesCount);

	data[Type::AlliedBullet - Type::HeroCount - Type::EnemyCount].damage = 25;
	data[Type::AlliedBullet - Type::HeroCount - Type::EnemyCount].speed = 300.f;
	data[Type::AlliedBullet - Type::HeroCount - Type::EnemyCount].texture = Textures::Bullet;

	data[Type::EnemyBullet - Type::HeroCount - Type::EnemyCount].damage = 10;
	data[Type::EnemyBullet - Type::HeroCount - Type::EnemyCount].speed = 300.f;
	data[Type::EnemyBullet - Type::HeroCount - Type::EnemyCount].texture = Textures::Bullet;

	data[Type::Flamestrike - Type::HeroCount - Type::EnemyCount].damage = 10;
	data[Type::Flamestrike - Type::HeroCount - Type::EnemyCount].speed = 300.f;
	data[Type::Flamestrike - Type::HeroCount - Type::EnemyCount].texture = Textures::Fire;

	data[Type::Fireball - Type::HeroCount - Type::EnemyCount].damage = 50;
	data[Type::Fireball - Type::HeroCount - Type::EnemyCount].speed = 300.f;
	data[Type::Fireball - Type::HeroCount - Type::EnemyCount].texture = Textures::Fire;

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);
	 
	/*
	data[Pickup::HealthRefill].texture = Textures::HealthRefill;
	data[Pickup::HealthRefill].action = [] (Player& a) { a.repair(25); };
	
	data[Pickup::MissileRefill].texture = Textures::MissileRefill;
	data[Pickup::MissileRefill].action = std::bind(&Player::collectMissiles, _1, 3);
	
	data[Pickup::FireSpread].texture = Textures::FireSpread;
	data[Pickup::FireSpread].action = std::bind(&Player::increaseSpread, _1);
	
	data[Pickup::FireRate].texture = Textures::FireRate;
	data[Pickup::FireRate].action = std::bind(&Player::increaseFireRate, _1);
	*/
	return data;
}

ShadowData initializeShadowData()
{
	ShadowData data;

	data.hitpoints = 150;
	data.damage = 2;
	data.speed = 50.f;
	data.texture = Textures::Shadow;
	data.timeInterval = sf::Time::Zero;
	data.teleportPoints.push_back(*new TeleportPoint(12420.f, 1164.f));
	data.teleportPoints.push_back(*new TeleportPoint(11941.f, 1164.f));
	data.teleportPoints.push_back(*new TeleportPoint(12180.f, 1164.f));
	data.teleportPoints.push_back(*new TeleportPoint(12180.f, 956.f));

	return data;
}

GolemDarkData initializeGolemDarkData()
{
	GolemDarkData data;

	data.hitpoints = 400;
	data.damage = 150;
	data.speed = 50.f;
	data.texture = Textures::Golem;
	data.timeInterval = sf::Time::Zero;

	return data;
}
