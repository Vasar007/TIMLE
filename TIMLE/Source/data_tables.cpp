#include "data_tables.hpp"


/*
 * Constant values of width and height for every object:
 *        - Ghost:                    w=51,        h=36
 *        - Golem:                    w=50,        h=38
 *        - Shadow:                   w=40,        h=35
 *        - GolemDark:                w=70,        h=60
 *        - DarkSoldier:              w=20,        h=30
 *        - Goblin:                   w=25,        h=28
 *        - MinotaurMage:             w=40,        h=40
 *        - Dwarf (all):              w=40,        h=27
 *        - Tentacle:                 w=13,        h=45
 *        - DarkArcher:               w=50,        h=50
 *        - Rock:                     w=type*16,   h=16
 *        - Gate (all):               w=16,        h=type*16
 *        - DialogPerson (Oswald):    w=16,        h=20
 *        - DialogPerson (Heinrich):  w=29,        h=32
 *        - MovingPlatform:           w=48,        h=11
 *        - DeadMan (DeadJuggernaut): w=35,        h=15
 *        - DeadMan (DeadDwarf):      w=25,        h=15
 *        - Projectile (Arrow):       w=7,         h=7
 *        - Projectile (MagicArrow):  w=13,        h=9
 */

// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::map<Type::ID, EntityData> initializeHeroData()
{
    std::map<Type::ID, EntityData> data;

    EntityData hero;
    hero.mHitpoints = 1000;
    hero.mDamage = 0;
    hero.mSpeed = 200.f;
    hero.mFireInterval = sf::seconds(1);
    hero.mTexture = Textures::ID::Archer;
    hero.mCalcBodyRect = 
        [](float left, float top, float width, float height, float)
    {
        return sf::FloatRect(left, top, width, height);
    };

    data.emplace(Type::ID::Archer, std::move(hero));
    return data;
}

std::map<Type::ID, EntityData> initializeEnemyData()
{
    std::map<Type::ID, EntityData> data;

    EntityData enemy;

    enemy.mHitpoints = 50;
    enemy.mDamage = 50;
    enemy.mSpeed = 80.f;
    enemy.mTexture = Textures::ID::Ghost;
    enemy.mFireInterval = sf::Time::Zero;
    enemy.mCalcBodyRect = 
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
    data.emplace(Type::ID::Ghost, std::move(enemy));


    enemy = EntityData();
    enemy.mHitpoints = 125;
    enemy.mDamage = 50;
    enemy.mSpeed = 50.f;
    enemy.mTexture = Textures::ID::Golem;
    enemy.mFireInterval = sf::Time::Zero;
    enemy.mCalcBodyRect = 
        [](float left, float top, float width, float height, float)
    {
        return sf::FloatRect(left + 15.f, top + 4.f, width - 28.f, height - 4.f);
    };
    data.emplace(Type::ID::Golem, std::move(enemy));


    enemy = EntityData();
    enemy.mHitpoints = 200;
    enemy.mDamage = 0;
    enemy.mSpeed = 50.f;
    enemy.mTexture = Textures::ID::DarkSoldier;
    enemy.mFireInterval = sf::Time::Zero;
    enemy.mCalcBodyRect = 
        [](float left, float top, float width, float height, float)
    {
        return sf::FloatRect(left + 5.f, top + 5.f, width - 10.f, height - 5.f);
    };
    data.emplace(Type::ID::DarkSoldier, std::move(enemy));


    enemy = EntityData();
    enemy.mHitpoints = 50;
    enemy.mDamage = 30;
    enemy.mSpeed = 50.f;
    enemy.mTexture = Textures::ID::Goblin;
    enemy.mFireInterval = sf::Time::Zero;
    enemy.mCalcBodyRect = 
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
    data.emplace(Type::ID::Goblin, std::move(enemy));


    enemy = EntityData();
    enemy.mHitpoints = 150;
    enemy.mDamage = 50;
    enemy.mSpeed = 50.f;
    enemy.mTexture = Textures::ID::MinotaurMage;
    enemy.mFireInterval = sf::seconds(2);
    enemy.mCalcBodyRect = 
        [](float left, float top, float width, float height, float)
    {
        return sf::FloatRect(left + 10.f, top + 10.f, width - 20.f, height - 10.f);
    };
    data.emplace(Type::ID::MinotaurMage, std::move(enemy));


    enemy = EntityData();
    enemy.mHitpoints = 75;
    enemy.mDamage = 20;
    enemy.mSpeed = 50.f;
    enemy.mTexture = Textures::ID::Dwarf;
    enemy.mFireInterval = sf::Time::Zero;
    enemy.mCalcBodyRect = 
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
    data.emplace(Type::ID::Dwarf, std::move(enemy));


    enemy = EntityData();
    enemy.mHitpoints = 75;
    enemy.mDamage = 10;
    enemy.mSpeed = 50.f;
    enemy.mTexture = Textures::ID::Dwarf;
    enemy.mFireInterval = sf::seconds(2);
    enemy.mCalcBodyRect = 
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
    data.emplace(Type::ID::DwarfArcher, std::move(enemy));


    enemy = EntityData();
    enemy.mHitpoints = 100;
    enemy.mDamage = 30;
    enemy.mSpeed = 50.f;
    enemy.mTexture = Textures::ID::Dwarf;
    enemy.mFireInterval = sf::Time::Zero;
    enemy.mCalcBodyRect = 
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
    data.emplace(Type::ID::DwarvenCommander, std::move(enemy));


    enemy = EntityData();
    enemy.mHitpoints = 100;
    enemy.mDamage = 10;
    enemy.mSpeed = 50.f;
    enemy.mTexture = Textures::ID::Tentacle;
    enemy.mFireInterval = sf::Time::Zero;
    enemy.mCalcBodyRect = 
        [](float left, float top, float width, float height, float)
    {
        return sf::FloatRect(left, top, width, height);
    };
    data.emplace(Type::ID::Tentacle, std::move(enemy));


    enemy = EntityData();
    enemy.mHitpoints = 100;
    enemy.mDamage = 25;
    enemy.mSpeed = 50.f;
    enemy.mTexture = Textures::ID::DarkArcher;
    enemy.mFireInterval = sf::seconds(2.f);
    enemy.mCalcBodyRect =
        [](float left, float top, float width, float height, float)
    {
        return sf::FloatRect(left + 15.f, top + 5.f, width - 30.f, height - 5.f);
    };
    data.emplace(Type::ID::DarkArcher, std::move(enemy));


    return data;
}

std::map<Type::ID, ProjectileData> initializeProjectileData()
{
    std::map<Type::ID, ProjectileData> data;

    ProjectileData projectile{};

    projectile.mDamage = 25;
    projectile.mSpeed = 10.f;
    projectile.mTexture = Textures::ID::Bullet;
    data.emplace(Type::ID::AlliedBullet, projectile);


    projectile = ProjectileData();
    projectile.mDamage = 10;
    projectile.mSpeed = 10.f;
    projectile.mTexture = Textures::ID::Bullet;
    data.emplace(Type::ID::EnemyBullet, projectile);


    projectile = ProjectileData();
    projectile.mDamage = 15;
    projectile.mSpeed = 0.f;
    projectile.mTexture = Textures::ID::Fire;
    data.emplace(Type::ID::Flamestrike, projectile);


    projectile = ProjectileData();
    projectile.mDamage = 25;
    projectile.mSpeed = 5.f;
    projectile.mTexture = Textures::ID::Fire;
    data.emplace(Type::ID::Fireball, projectile);


    projectile = ProjectileData();
    projectile.mDamage = 15;
    projectile.mSpeed = 4.f;
    projectile.mTexture = Textures::ID::MagicArrow;
    data.emplace(Type::ID::MagicArrow, projectile);

    return data;
}

std::map<Type::ID, PickupData> initializePickupData()
{
    /*
    std::map<Type::ID, ProjectileData> data; 
    
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
