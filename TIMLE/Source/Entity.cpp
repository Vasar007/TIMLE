#include <utility>
#include "../Include/Entity.hpp"


Entity::Entity(const Type::ID id, const float X, const  float Y, const int width, const int height, 
               const float speed, const int hitpoints, const std::size_t damage,
               std::string type, const FuncCalcBodyRect& calcBodyRect)
: _calcBodyRect(calcBodyRect)
, x(X)
, y(Y)
, dx(0.f)
, dy(0.f)
, mSpeed(speed)
, mMoveTimer(0.f)
, mDeathTimer(0.f)
, mCurrentFrame(0.f)
, mCurrentAttack(0.f)
, mCurrentDeath(0.f)
, mWidth(width)
, mHeight(height)
, mBodyX(0.f)
, mBodyY(0.f)
, mBodyWidth(0.f)
, mBodyHeight(0.f)
, mHitpoints(hitpoints)
, mDamage(damage)
, mLife(true)
, mIsMove(true)
, mOnGround(false)
, mIsAttacked(false)
, mIsHitted(false)
, mIsHittedOnce(false)
, mIsStarted(false)
, mIsBack(false)
, mIsSpawn(false)
, mIsEnd(false)
, mIsEnabled(false)
, mIsDisabled(false)
, mIsEnabling(false)
, mType(std::move(type))
, mTypeID(id)
{
    mSprite.setOrigin(static_cast<float>(mWidth) / 2.f, static_cast<float>(mHeight) / 2.f);
    //centerOrigin(mSprite);
}

void Entity::setVelocity(const sf::Vector2f velocity)
{
    _velocity = velocity;
}

void Entity::setVelocity(const float vx, const float vy)
{
    _velocity.x = vx;
    _velocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
    return _velocity;
}

sf::FloatRect Entity::getRect() const
{
    return sf::FloatRect(x, y, static_cast<float>(mWidth), static_cast<float>(mHeight));
}

sf::Vector2f Entity::getCenter() const
{
    return sf::Vector2f(x + static_cast<float>(mWidth) / 2.f,
                        y + static_cast<float>(mHeight) / 2.f);
}

sf::FloatRect Entity::getBodyRect() const
{
    if (!_calcBodyRect)
    {
        return sf::FloatRect();
    }

    return _calcBodyRect(x, y, static_cast<float>(mWidth), static_cast<float>(mHeight), dx);
}

sf::Vector2f Entity::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform Entity::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;

    transform *= mSprite.getTransform();

    return transform;
}

void Entity::draw(sf::RenderTarget& target) const
{
    target.draw(mSprite);
}
