#include <cmath>

#include "data_tables.hpp"
#include "utility.hpp"

#include "projectile.hpp"


namespace
{
    const std::map<Type::ID, ProjectileData> PROJECTILE_TABLE = initializeProjectileData();
}

Projectile::Projectile(const Type::ID id, const TextureHolder&, const FontHolder&,
                       const Level&, const float X, const float Y, const int width, const int height)
: Entity(id, X, Y, width, height, PROJECTILE_TABLE.at(id).mSpeed, 100,
         PROJECTILE_TABLE.at(id).mDamage)
, _guided(false)
{
    if (id == Type::ID::MagicArrow)
    {
        _guided = true;
    }
}

void Projectile::guideTowards(const sf::Vector2f position)
{
    _targetDirection = utils::unitVector(position - getWorldPosition());
}

bool Projectile::isGuided() const
{
    return _guided;
}

void Projectile::updateDirection(const float dt)
{
    if (isGuided())
    {
        const float approachRate = 100.f;

        // float dt => sf::Time dt.asSeconds()
        auto newVelocity = utils::unitVector(
            approachRate * dt * _targetDirection + getVelocity()
        );
        newVelocity *= getMaxSpeed();
        const auto angle = atan2(newVelocity.y, newVelocity.x);

        mSprite.setRotation(utils::toDegree(angle));
        setVelocity(newVelocity);
    }
}

unsigned int Projectile::getCategory() const
{
    if (mTypeID == Type::ID::AlliedBullet)
    {
        return Category::AlliedProjectile;
    }
    else
    {
        return Category::EnemyProjectile;
    }
}

sf::FloatRect Projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
    return PROJECTILE_TABLE.at(mTypeID).mSpeed;
}

int Projectile::getDamage() const
{
    return PROJECTILE_TABLE.at(mTypeID).mDamage;
}
