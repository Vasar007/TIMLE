#include "../Include/Entity.hpp"


Entity::Entity(Type::ID Id, float X, float Y, int width, int height, float speed, int hitpoints,
			   size_t damage, std::string Type)
: x(X)
, y(Y)
, dx(0.f)
, dy(0.f)
, mSpeed(speed)
, mMoveTimer(0.f)
, mDeathTimer(0.f)
, mCurrentFrame(0.f)
, mCurrentAttack (0.f)
, mCurrentDeath(0.f)
, mWidth(width)
, mHeight(height)
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
, mType(Type)
, mTypeID(Id)
{
	mSprite.setOrigin(static_cast<float>(mWidth) / 2.f, static_cast<float>(mHeight) / 2.f); 
	//centerOrigin(mSprite);
}

Entity::~Entity()
{
}


sf::FloatRect Entity::getRect() const
{
	// Функция получения прямоугольника, его координат и размеров(ширина, высота)
	return sf::FloatRect(x, y, static_cast<float>(mWidth), static_cast<float>(mHeight));
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
