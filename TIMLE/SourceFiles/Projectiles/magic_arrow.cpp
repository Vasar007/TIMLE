#include <cmath>

#include "Projectiles/magic_arrow.hpp"


MagicArrow::MagicArrow(const Type::ID id,const TextureHolder& textures, const FontHolder& fonts,
                       const level& lvl, const float X, const float Y, const int width, 
                       const int height, const float tX, const float tY)
: Projectile(id, textures, fonts, lvl, X, Y, width, height)
, mTargetX(tX)
, mTargetY(tY)
, mVx((mTargetX - x) / 100.f)
, mVy((mTargetY - y) / 100.f)    // 100 - дистанция
, mDistance(std::sqrt((mTargetX - x)*(mTargetX - x) + (mTargetY - y)*(mTargetY - y)))
{
    mLevelObjects = lvl.get_objects("solid");
    mSpeed = 0.2f;
    mTexture = textures.get(Textures::ID::MagicArrow);

    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, 44, 18));
    mSprite.setScale(0.5f, 0.5f);
}

void MagicArrow::update(const float dt)
{
    mMoveTimer += dt;
    if (mMoveTimer > 150.f)
    {
        updateDirection(dt);
        mMoveTimer = 0.f;
    }

    x += getVelocity().x * 0.85f;
    y += getVelocity().y * 0.33f;

    // Задержка пули в левой стене, чтобы при проседании кадров она случайно 
    // не вылетела за предел карты и не было ошибки
    if (x <= 0.f)
    {
        x = -mSprite.getLocalBounds().width;
    }
    if (y <= 0.f)
    {
        y = -mSprite.getLocalBounds().height;
    }

    for (const auto& object : mLevelObjects)
    {
        if (getRect().intersects(object.rect))
        {
            mLife = false;
            break;
        }
    }
    if (mMoveTimer > 5000.f)
    {
        mLife = false;
    }

    mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));

    mMoveTimer += dt;

    mCurrentFrame += 0.0045f * dt;
    if (mCurrentFrame > 2.f)
    {
        mCurrentFrame -= 2.f;
    }
    mSprite.setTextureRect(sf::IntRect(44 * static_cast<int>(mCurrentFrame), 0, 44, 18));
}
