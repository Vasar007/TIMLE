#include <utility>
#include "../Include/Effect.hpp"


Effect::Effect(const Type::ID id, const float X, const float Y, const int width, const int height,
               std::string type)
: x(X)
, y(Y)
, mWidth(width)
, mHeight(height)
, mCurrentFrame(0.f)
, mLife(true)
, mIsStarted(false)
, mIsEnd(false)
, mType(std::move(type))
, mTypeID(id)
{
    mSprite.setOrigin(static_cast<float>(mWidth) / 2.f, static_cast<float>(mHeight) / 2.f);
}

sf::FloatRect Effect::getRect() const
{
    return sf::FloatRect(x, y, static_cast<float>(mWidth), static_cast<float>(mHeight));
}

sf::Vector2f Effect::getCenter() const
{
    return sf::Vector2f(x + static_cast<float>(mWidth) / 2.f,
                        y + static_cast<float>(mHeight) / 2.f);
}

sf::Vector2f Effect::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform Effect::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;

    transform *= mSprite.getTransform();

    return transform;
}

void Effect::draw(sf::RenderTarget& target) const
{
    target.draw(mSprite);
}
