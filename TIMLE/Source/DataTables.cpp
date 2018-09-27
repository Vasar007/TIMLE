#include "../Include/DataTables.hpp"


/*
 * Constant values of width and height for every object:
 *        – Ghost:                    w=51,        h=36
 *        – Golem:                    w=50,        h=38
 *        – DarkSoldier:              w=20,        h=30
 *        – Goblin:                   w=25,        h=28
 *        – MinotaurMage:             w=40,        h=40
 *        – Dwarf (all):              w=40,        h=27
 *        – Tentacle:                 w=13,        h=45
 *        – DarkArcher:               w=50,        h=50
 *        – Rock:                     w=type*16,   h=16
 *        – Gate (all):               w=16,        h=type*16
 *        – DialogPerson (Oswald):    w=16,        h=20
 *        – DialogPerson (Heinrich):  w=29,        h=32
 *        – MovingPlatform:           w=48,        h=11
 *        – DeadMan (DeadJuggernaut): w=35,        h=15
 *        – DeadMan (DeadDwarf):      w=25,        h=15
 */

// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<EntityData> initializeHeroData()
{
    std::vector<EntityData> data(Type::HERO_COUNT);

    data.at(Type::ID::Archer).mHitpoints = 1000;
    data.at(Type::ID::Archer).mDamage = 0;
    data.at(Type::ID::Archer).mSpeed = 200.f;
    data.at(Type::ID::Archer).mFireInterval = sf::seconds(1);
    data.at(Type::ID::Archer).mTexture = Textures::ID::Archer;
    data.at(Type::ID::Archer).mCalcBodyRect = 
        [](float left, float top, float width, float height, float)
    {
        return sf::FloatRect(left, top, width, height);
    };

    return data;
}

std::vector<EntityData> initializeEnemyData()
{
    std::vector<EntityData> data(Type::ENEMY_COUNT);

    data.at(Type::ID::Ghost - Type::HERO_COUNT).mHitpoints = 50;
    data.at(Type::ID::Ghost - Type::HERO_COUNT).mDamage = 50;
    data.at(Type::ID::Ghost - Type::HERO_COUNT).mSpeed = 80.f;
    data.at(Type::ID::Ghost - Type::HERO_COUNT).mTexture = Textures::ID::Ghost;
    data.at(Type::ID::Ghost - Type::HERO_COUNT).mFireInterval = sf::Time::Zero;
    data.at(Type::ID::Ghost - Type::HERO_COUNT).mCalcBodyRect = 
        [](float left, float top, float width, float height, float direction)
    {
        if (direction > 0.f)
        {
            return sf::FloatRect(left + 7.f, top + 7.f, width - 32.f, height - 10.f);
        }
        else
        {
            return sf::FloatRect(left + 25.f, top + 7.f, width - 32.f, height - 10.f);
        }
    };

    data.at(Type::ID::Golem - Type::HERO_COUNT).mHitpoints = 125;
    data.at(Type::ID::Golem - Type::HERO_COUNT).mDamage = 50;
    data.at(Type::ID::Golem - Type::HERO_COUNT).mSpeed = 50.f;
    data.at(Type::ID::Golem - Type::HERO_COUNT).mTexture = Textures::ID::Golem;
    data.at(Type::ID::Golem - Type::HERO_COUNT).mFireInterval = sf::Time::Zero;
    data.at(Type::ID::Golem - Type::HERO_COUNT).mCalcBodyRect = 
        [](float left, float top, float width, float height, float direction)
    {
        if (direction > 0.f)
        {
            return sf::FloatRect(left + 15.f, top + 4.f, width - 28.f, height - 4.f);
        }
        else
        {
            return sf::FloatRect(left + 15.f, top + 4.f, width - 28.f, height - 4.f);
        }
    };


    data.at(Type::ID::DarkSoldier - Type::HERO_COUNT).mHitpoints = 1000;
    data.at(Type::ID::DarkSoldier - Type::HERO_COUNT).mDamage = 0;
    data.at(Type::ID::DarkSoldier - Type::HERO_COUNT).mSpeed = 50.f;
    data.at(Type::ID::DarkSoldier - Type::HERO_COUNT).mTexture = Textures::ID::DarkSoldier;
    data.at(Type::ID::DarkSoldier - Type::HERO_COUNT).mFireInterval = sf::Time::Zero;
    data.at(Type::ID::DarkSoldier - Type::HERO_COUNT).mCalcBodyRect = 
        [](float left, float top, float width, float height, float)
    {
        return sf::FloatRect(left + 5.f, top + 5.f, width - 10.f, height - 5.f);
    };


    data.at(Type::ID::Goblin - Type::HERO_COUNT).mHitpoints = 50;
    data.at(Type::ID::Goblin - Type::HERO_COUNT).mDamage = 30;
    data.at(Type::ID::Goblin - Type::HERO_COUNT).mSpeed = 50.f;
    data.at(Type::ID::Goblin - Type::HERO_COUNT).mTexture = Textures::ID::Goblin;
    data.at(Type::ID::Goblin - Type::HERO_COUNT).mFireInterval = sf::Time::Zero;
    data.at(Type::ID::Goblin - Type::HERO_COUNT).mCalcBodyRect = 
        [](float left, float top, float width, float height, float direction)
    {
        if (direction > 0.f)
        {
            return sf::FloatRect(left + 4.f, top + 5.f, width - 13.f, height - 5.f);
        }
        else
        {
            return sf::FloatRect(left + 9.f, top + 5.f, width - 13.f, height - 5.f);
        }

    };


    data.at(Type::ID::MinotaurMage - Type::HERO_COUNT).mHitpoints = 150;
    data.at(Type::ID::MinotaurMage - Type::HERO_COUNT).mDamage = 50;
    data.at(Type::ID::MinotaurMage - Type::HERO_COUNT).mSpeed = 50.f;
    data.at(Type::ID::MinotaurMage - Type::HERO_COUNT).mTexture = Textures::ID::MinotaurMage;
    data.at(Type::ID::MinotaurMage - Type::HERO_COUNT).mFireInterval = sf::seconds(2);
    data.at(Type::ID::MinotaurMage - Type::HERO_COUNT).mCalcBodyRect = 
        [](float left, float top, float width, float height, float)
    {
        return sf::FloatRect(left + 10.f, top + 10.f, width - 20.f, height - 10.f);
    };


    data.at(Type::ID::Dwarf - Type::HERO_COUNT).mHitpoints = 75;
    data.at(Type::ID::Dwarf - Type::HERO_COUNT).mDamage = 20;
    data.at(Type::ID::Dwarf - Type::HERO_COUNT).mSpeed = 50.f;
    data.at(Type::ID::Dwarf - Type::HERO_COUNT).mTexture = Textures::ID::Dwarf;
    data.at(Type::ID::Dwarf - Type::HERO_COUNT).mFireInterval = sf::Time::Zero;
    data.at(Type::ID::Dwarf - Type::HERO_COUNT).mCalcBodyRect = 
        [](float left, float top, float width, float height, float direction)
    {
        if (direction > 0.f)
        {
            return sf::FloatRect(left + 3.f, top + 1.f, width - 20.f, height - 1.f);
        }
        else
        {
            return sf::FloatRect(left + 17.f, top + 1.f, width - 20.f, height - 1.f);
        }
    };


    data.at(Type::ID::DwarfArcher - Type::HERO_COUNT).mHitpoints = 75;
    data.at(Type::ID::DwarfArcher - Type::HERO_COUNT).mDamage = 10;
    data.at(Type::ID::DwarfArcher - Type::HERO_COUNT).mSpeed = 50.f;
    data.at(Type::ID::DwarfArcher - Type::HERO_COUNT).mTexture = Textures::ID::Dwarf;
    data.at(Type::ID::DwarfArcher - Type::HERO_COUNT).mFireInterval = sf::seconds(2);
    data.at(Type::ID::DwarfArcher - Type::HERO_COUNT).mCalcBodyRect = 
        [](float left, float top, float width, float height, float direction)
    {
        if (direction > 0.f)
        {
            return sf::FloatRect(left + 3.f, top + 1.f, width - 20.f, height - 1.f);
        }
        else
        {
            return sf::FloatRect(left + 17.f, top + 1.f, width - 20.f, height - 1.f);
        }
    };


    data.at(Type::ID::DwarvenCommander - Type::HERO_COUNT).mHitpoints = 100;
    data.at(Type::ID::DwarvenCommander - Type::HERO_COUNT).mDamage = 30;
    data.at(Type::ID::DwarvenCommander - Type::HERO_COUNT).mSpeed = 50.f;
    data.at(Type::ID::DwarvenCommander - Type::HERO_COUNT).mTexture = Textures::ID::Dwarf;
    data.at(Type::ID::DwarvenCommander - Type::HERO_COUNT).mFireInterval = sf::Time::Zero;
    data.at(Type::ID::DwarvenCommander - Type::HERO_COUNT).mCalcBodyRect = 
        [](float left, float top, float width, float height, float direction)
    {
        if (direction > 0.f)
        {
            return sf::FloatRect(left + 3.f, top + 1.f, width - 20.f, height - 1.f);
        }
        else
        {
            return sf::FloatRect(left + 17.f, top + 1.f, width - 20.f, height - 1.f);
        }
    };

    data.at(Type::ID::Tentacle - Type::HERO_COUNT).mHitpoints = 100;
    data.at(Type::ID::Tentacle - Type::HERO_COUNT).mDamage = 10;
    data.at(Type::ID::Tentacle - Type::HERO_COUNT).mSpeed = 50.f;
    data.at(Type::ID::Tentacle - Type::HERO_COUNT).mTexture = Textures::ID::Tentacle;
    data.at(Type::ID::Tentacle - Type::HERO_COUNT).mFireInterval = sf::Time::Zero;
    data.at(Type::ID::Tentacle - Type::HERO_COUNT).mCalcBodyRect = 
        [](float left, float top, float width, float height, float)
    {
        return sf::FloatRect(left, top, width, height);
    };

    data.at(Type::ID::DarkArcher - Type::HERO_COUNT).mHitpoints = 150;
    data.at(Type::ID::DarkArcher - Type::HERO_COUNT).mDamage = 25;
    data.at(Type::ID::DarkArcher - Type::HERO_COUNT).mSpeed = 50.f;
    data.at(Type::ID::DarkArcher - Type::HERO_COUNT).mTexture = Textures::ID::DarkArcher;
    data.at(Type::ID::DarkArcher - Type::HERO_COUNT).mFireInterval = sf::seconds(2.f);
    data.at(Type::ID::DarkArcher - Type::HERO_COUNT).mCalcBodyRect =
        [](float left, float top, float width, float height, float)
    {
        return sf::FloatRect(left + 15.f, top + 5.f, width - 30.f, height - 5.f);
    };


    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(Type::PROJECTILES_COUNT);

    data.at(Type::ID::AlliedBullet - Type::HERO_COUNT - Type::ENEMY_COUNT).mDamage = 25;
    data.at(Type::ID::AlliedBullet - Type::HERO_COUNT - Type::ENEMY_COUNT).mSpeed = 10.f;
    data.at(Type::ID::AlliedBullet - Type::HERO_COUNT - Type::ENEMY_COUNT).mTexture = 
        Textures::ID::Bullet;

    data.at(Type::ID::EnemyBullet - Type::HERO_COUNT - Type::ENEMY_COUNT).mDamage = 10;
    data.at(Type::ID::EnemyBullet - Type::HERO_COUNT - Type::ENEMY_COUNT).mSpeed = 10.f;
    data.at(Type::ID::EnemyBullet - Type::HERO_COUNT - Type::ENEMY_COUNT).mTexture = 
        Textures::ID::Bullet;

    data.at(Type::ID::Flamestrike - Type::HERO_COUNT - Type::ENEMY_COUNT).mDamage = 15;
    data.at(Type::ID::Flamestrike - Type::HERO_COUNT - Type::ENEMY_COUNT).mSpeed = 0.f;
    data.at(Type::ID::Flamestrike - Type::HERO_COUNT - Type::ENEMY_COUNT).mTexture = 
        Textures::ID::Fire;

    data.at(Type::ID::Fireball - Type::HERO_COUNT - Type::ENEMY_COUNT).mDamage = 25;
    data.at(Type::ID::Fireball - Type::HERO_COUNT - Type::ENEMY_COUNT).mSpeed = 5.f;
    data.at(Type::ID::Fireball - Type::HERO_COUNT - Type::ENEMY_COUNT).mTexture = Textures::ID::Fire;

    data.at(Type::ID::MagicArrow - Type::HERO_COUNT - Type::ENEMY_COUNT).mDamage = 15;
    data.at(Type::ID::MagicArrow - Type::HERO_COUNT - Type::ENEMY_COUNT).mSpeed = 4.f;
    data.at(Type::ID::MagicArrow - Type::HERO_COUNT - Type::ENEMY_COUNT).mTexture = 
        Textures::ID::MagicArrow;

    return data;
}

std::vector<PickupData> initializePickupData()
{
    /*
    std::vector<PickupData> data(Pickup::Type::TypeCount); 
    
    data[Pickup::HealthRefill].texture = Textures::HealthRefill;
    data[Pickup::HealthRefill].action = [](Player& a) { a.repair(25); };
    
    data[Pickup::MissileRefill].texture = Textures::MissileRefill;
    data[Pickup::MissileRefill].action = std::bind(&Player::collectMissiles, _1, 3);
    
    data[Pickup::FireSpread].texture = Textures::FireSpread;
    data[Pickup::FireSpread].action = std::bind(&Player::increaseSpread, _1);
    
    data[Pickup::FireRate].texture = Textures::FireRate;
    data[Pickup::FireRate].action = std::bind(&Player::increaseFireRate, _1);
    
    return data;
    */
    return {};
}

ShadowData initializeShadowData()
{
    ShadowData data;

    data.mHitpoints = 150;
    data.mDamage = 2;
    data.mSpeed = 50.f;
    data.mTexture = Textures::ID::Shadow;
    data.mTimeInterval = sf::Time::Zero;
    data.mTeleportPoints.emplace_back(12420.f, 1164.f);
    data.mTeleportPoints.emplace_back(11941.f, 1164.f);
    data.mTeleportPoints.emplace_back(12180.f, 1164.f);
    data.mTeleportPoints.emplace_back(12180.f, 956.f);

    return data;
}

GolemDarkData initializeGolemDarkData()
{
    GolemDarkData data;

    data.mHitpoints = 400;
    data.mDamage = 150;
    data.mSpeed = 50.f;
    data.mTexture = Textures::ID::Golem;
    data.mTimeInterval = sf::Time::Zero;

    return data;
}
