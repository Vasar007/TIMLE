#include "Enemies/dark_archer.hpp"


DarkArcher::DarkArcher(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
                       const level& lvl, const float X, const  float Y, const int width, 
                       const int height, const std::string& type)
: Enemy(id, textures, fonts, lvl, X, Y, width, height, type)
{
    mTexture = textures.get(Textures::ID::DarkArcher);
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, mWidth, mHeight));
    mSprite.setScale(0.75f, 0.75f);
    dx = 0.075f;
    mIsStarted = false;
    mIsEnd = false;
}

void DarkArcher::checkCollisionWithMap(const float Dx, const float Dy)
{
    for (const auto& object : mLevelObjects)
    {
        // Проверяем пересечение с объектом
        if (getRect().intersects(object.rect))
        {
            if (object.name == "enemyBorder")
            {
                if (Dy > 0.f)
                {
                    y = object.rect.top - mHeight;
                    dy = 0.f;
                    mOnGround = true;
                }
                if (Dy < 0.f)
                {
                    y = object.rect.top + object.rect.height;
                    dy = 0.f;
                }
                if (Dx > 0.f)
                {
                    x = object.rect.left - mWidth;
                    mIsTurned = true;
                }
                if (Dx < 0.f)
                {
                    x = object.rect.left + object.rect.width;
                    mIsTurned = true;
                }
            }

            // Если встретили смерть
            if (object.name == "death")
            {
                mHitpoints = 0;
            }
        }
    }
}

void DarkArcher::update(const float dt)
{
    // Притяжение к земле
    dy += 0.00015f * dt;
    y += dy * dt;
    checkCollisionWithMap(0.f, dy);

    if (mIsTurned)
    {
        mMoveTimer += dt;
    }
    if (mMoveTimer > 500.f && mIsTurned)
    {
        dx = -dx;
        mMoveTimer = 0.f;
        mIsTurned = false;
        mSprite.scale(-1.f, 1.f);
    }

    if (mLife && mHitpoints > 0)
    {
        if (!mIsAttacked && !mIsTurned && !mIsStarted && !mIsDisabled)
        {
            x += dx * dt;
        }
        checkCollisionWithMap(dx, 0.f);

        mDeathTimer += 0.005f * dt;
        if (static_cast<int>(mDeathTimer) == 2 && !mIsAttacked && !mIsStarted && !mIsDisabled)
        {
            dy += -0.003f;
        }
        else if (mDeathTimer > 2.1f)
        {
            mDeathTimer -= 2.1f;
        }

        mSprite.setPosition(x + mWidth / 2.f + (dx > 0.f ? -10.f : 10.f), y + mHeight / 2.f - 13.f);

        if (mIsStarted)
        {
            mCurrentFrame += 0.0075f * dt;
        }
        if (mCurrentFrame > 5.f)
        {
            mCurrentFrame -= 5.f;
            mIsStarted = false;
            mIsEnd = true;
            mIsAttacked = true;
        }

        if (mIsAttacked)
        {
            mCurrentAttack += 0.004f * dt;
            if (mCurrentAttack > 5.f)
            {
                mCurrentAttack -= 5.f;
                mIsAttacked = false;
                mIsBack = false;
                mIsHittedOnce = false;

                mCurrentFrame = 5.f;
                mIsDisabled = true;
            }
            else if (static_cast<int>(mCurrentAttack) == 4 && !mIsHittedOnce)
            {
                mIsHitted = true;
                mIsHittedOnce = true;
            }
            else
            {
                mIsHitted = false;
            }

            if (dx > 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(76 * static_cast<int>(mCurrentAttack), 76, 76, 76));
            }
            else if (dx < 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(76 * static_cast<int>(mCurrentAttack), 76, 76, 76));
            }
        }
        else if (mIsDisabled)
        {
            mCurrentAttack = 0.f;
            mCurrentFrame -= 0.0075f * dt;

            if (mCurrentFrame < 0.f)
            {
                mCurrentFrame = 0.f;
                mIsDisabled = false;
                mIsEnd = false;
            }

            if (mIsTurned)
            {
                mCurrentFrame = 5.f;
                mSprite.setTextureRect(sf::IntRect(76 * static_cast<int>(mCurrentFrame), 0, 76, 76));
            }
            if (dx > 0.f && !mIsTurned)
            {
                mSprite.setTextureRect(sf::IntRect(76 * static_cast<int>(mCurrentFrame), 0, 76, 76));
            }
            else if (dx < 0.f && !mIsTurned)
            {
                mSprite.setTextureRect(sf::IntRect(76 * static_cast<int>(mCurrentFrame), 0, 76, 76));
            }
        }
        else if (!mIsEnd)
        {
            mCurrentAttack = 0.f;
            if (mIsStarted)
            {
                mIsEnd = false;
            }

            if (mIsTurned)
            {
                mCurrentFrame = 0.f;
                mSprite.setTextureRect(sf::IntRect(76 * static_cast<int>(mCurrentFrame), 0, 76, 76));
            }
            if (dx > 0.f && !mIsTurned)
            {
                mSprite.setTextureRect(sf::IntRect(76 * static_cast<int>(mCurrentFrame), 0, 76, 76));
            }
            else if (dx < 0.f && !mIsTurned)
            {
                mSprite.setTextureRect(sf::IntRect(76 * static_cast<int>(mCurrentFrame), 0, 76, 76));
            }
        }
    }

    if (mHitpoints <= 0)
    {
        mMoveTimer += dt;
        if (mMoveTimer > 1000.f)
        {
            mMoveTimer = 0.f;
            ++mCounter;
        }
        mCurrentDeath += 0.0035f * dt;
        dx = 0.f;
        dy = 0.f;
        if (mCurrentDeath > 1.f)
        {
            mCurrentDeath = 1.f;
            if (mCounter == 6)
            {
                mLife = false;
            }
        }
        mSprite.setPosition(x + mWidth / 2.f + (dx > 0.f ? -10.f : 10.f), y + mHeight / 2.f - 13.f);
        mSprite.setTextureRect(sf::IntRect(76 * static_cast<int>(mCurrentDeath), 152, 76, 76));
    }
}
