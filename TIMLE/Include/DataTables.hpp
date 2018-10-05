#ifndef DATATABLES_HPP
#define DATATABLES_HPP

#include <vector>
#include <functional>
#include <map>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "ResourceIdentifiers.hpp"


// Forward declaration.

class Player;
class Enemy;

/**
 * \brief Additional namespace for convenience.
 */
namespace Type
{
    /**
     * \brief Identifiers of all entities.
     */
    enum class ID
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
        DarkArcher,

        AlliedBullet,
        EnemyBullet,
        Flamestrike,
        Fireball,
        MagicArrow,

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

        Bloodsplat,

        TypeCount
    };
}

/**
 * \brief Additional structure to work with direction of the some entities.
 */
struct Direction
{
    Direction(const float angle, const float distance)
    : mAngle(angle)
    , mDistance(distance)
    {
    }

    float mAngle;
    float mDistance;
};

/**
 * \brief Additional structure to work with teleporting entities.
 */
struct TeleportPoint
{
    TeleportPoint(const float x, const float y)
    : x(x)
    , y(y)
    {
    }

    float x;
    float y;
};

/**
 * \brief Additional structure to work with creating entities.
 */
struct EntityData
{
    std::size_t            mHitpoints{};
    std::size_t            mDamage{};
    float                  mSpeed{};
    Textures::ID           mTexture;
    sf::Time               mFireInterval;
    std::vector<Direction> mDirections;

    std::function<sf::FloatRect(float top, float left, 
                                float width, float height, float direction)> mCalcBodyRect;

};

/**
 * \brief Additional structure to work with boss of the first level.
 */
struct ShadowData
{
    std::size_t                mHitpoints{};
    std::size_t                mDamage{};
    float                      mSpeed{};
    Textures::ID               mTexture;
    sf::Time                   mTimeInterval;
    std::vector<TeleportPoint> mTeleportPoints;
};

/**
 * \brief Additional structure to work with mini-boss of the first level.
 */
struct GolemDarkData
{
    std::size_t  mHitpoints{};
    std::size_t  mDamage{};
    float        mSpeed{};
    Textures::ID mTexture;
    sf::Time     mTimeInterval;
};

/**
 * \brief Additional structure to work with projectiles' behaviour.
 */
struct ProjectileData
{
    std::size_t  mDamage;
    float        mSpeed;
    Textures::ID mTexture;
};

/**
 * \brief Additional structure to work with collectable items.
 */
struct PickupData
{
    std::function<void(Player&)> mAction;
    Textures::ID                 mTexture;
};

// Initialization block.

std::map<Type::ID, EntityData>     initializeHeroData();
std::map<Type::ID, EntityData>     initializeEnemyData();
ShadowData                         initializeShadowData();
GolemDarkData                      initializeGolemDarkData();
std::map<Type::ID, ProjectileData> initializeProjectileData();
std::map<Type::ID, PickupData>     initializePickupData();

#endif // DATATABLES_HPP
