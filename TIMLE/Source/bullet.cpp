#include <cmath>

#include "bullet.hpp"


Bullet::Bullet(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts, 
               const Level& lvl, const float X, const float Y, const int width, const int height,
               const float tX, const float tY)
: Projectile(id, textures, fonts, lvl, X, Y, width, height) // При клике мышкой
, mTargetX(tX)
, mTargetY(tY)
, mVx((mTargetX - x) / 100.f)
, mVy((mTargetY - y) / 100.f) // 100 - дистанция
, mDistance(std::sqrt((mTargetX - x) * (mTargetX - x) + (mTargetY - y) * (mTargetY - y)))
, mIsMouse(true)
{
    mLevelObjects = lvl.getObjects("solid");
    mSpeed = 0.2f;
    mTexture = textures.get(Textures::ID::Bullet);

    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, 13, 13));
    mSprite.setScale(0.5f, 0.5f);
}

Bullet::Bullet(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts, 
               const Level& lvl, const float X, const float Y, const int width, const int height, 
               const int dir)
: Projectile(id, textures, fonts, lvl, X, Y, width, height) // При нажатии space
, mTargetX(X + 150.f * ((dir - 1 == 0) || (dir - 1 == 4) ? 1.f : -1.f))
, mTargetY(Y)
, mVx((mTargetX - x) / 100.f)
, mVy((mTargetY - y) / 100.f) // 100 - дистанция
, mDistance(std::sqrt((mTargetX - x) * (mTargetX - x) + (mTargetY - y) * (mTargetY - y)))
, mIsMouse(false)
{
    mLevelObjects = lvl.getObjects("solid");
    mSpeed = 0.2f;
    mTexture = textures.get(Textures::ID::Bullet);

    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, 13, 13));
    mSprite.setScale(0.5f, 0.5f);
}

void Bullet::update(const float dt)
{

    //x += mSpeed*time*(targetX - x) / 20; // Само движение пули по х
    //y += mSpeed*time*(targetY - y) / 20; // По у

    x += mVx * dt * mSpeed;
    y += mVy * dt * mSpeed;

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

    for (const auto& object : mLevelObjects) // Проход по объектам solid
    {
        if (getRect().intersects(object.mRect))
        {
            mLife = false;
        }
    }
    if (mMoveTimer > 3000.f)
    {
        mLife = false;
    }

    mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 2.f);

    mMoveTimer += dt;

    mCurrentFrame += 0.005f * dt;
    if (mCurrentFrame > 5.f)
    {
        mCurrentFrame -= 5.f;
    }
    mSprite.setTextureRect(sf::IntRect(13 * static_cast<int>(mCurrentFrame), 0, 13, 13));
}
