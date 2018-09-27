#include "../Include/MinotaurMage.hpp"


MinotaurMage::MinotaurMage(const Type::ID id, const TextureHolder& textures, 
                           const FontHolder& fonts, const Level& lvl, const float X, const float Y,
                           const int width, const int height, const std::string& type)
: Enemy(id, textures, fonts, lvl, X, Y, width, height, type)
{
    mTexture = textures.get(Textures::ID::MinotaurMage);
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(8, 4, mWidth, mHeight));
    mSprite.setScale(0.5f, 0.5f);
    dx = 0.075f;
}

void MinotaurMage::checkCollisionWithMap(const float Dx, const float Dy)
{
    for (const auto& object : mLevelObjects)
    {
        // Проверяем пересечение с объектом
        if (getRect().intersects(object.mRect))
        {
            if (object.mName == "enemyBorder")
            {
                if (Dy > 0.f)
                {
                    y = object.mRect.top - mHeight;
                    dy = 0.f;
                    mOnGround = true;
                }
                if (Dy < 0.f)
                {
                    y = object.mRect.top + object.mRect.height;
                    dy = 0.f;
                }
                if (Dx > 0.f)
                {
                    x = object.mRect.left - mWidth;
                    mIsTurned = true;
                }
                if (Dx < 0.f)
                {
                    x = object.mRect.left + object.mRect.width;
                    mIsTurned = true;
                }
            }

            // Если встретили смерть
            if (object.mName == "death")
            {
                mHitpoints = 0;
            }
        }
    }
}

void MinotaurMage::update(const float dt)
{
    // Притяжение к земле
    dy += 0.0015f * dt;
    y += dy * dt;
    checkCollisionWithMap(0.f, dy);

    if (mIsTurned)
    {
        mMoveTimer += dt;
    }
    if (mMoveTimer > 2000.f && mIsTurned)
    {
        dx = -dx;
        mMoveTimer = 0.f;
        mIsTurned = false;
        mSprite.scale(-1.f, 1.f);
    }

    if (mLife && (mHitpoints > 0))
    {
        //if (!isAttacked && !isTurned)
        //    x += dx * time;
        checkCollisionWithMap(dx, 0.f);

        /// Animation
        mCurrentFrame += 0.003f * dt;
        if (mCurrentFrame > 2.f)
        {
            mCurrentFrame -= 2.f;
        }

        mSprite.setPosition(x + (mWidth / 2.f) - 12.f + (dx > 0 ? 0.f : 24.f), y + (mHeight / 2.f) - 13.f);

        if (mIsAttacked)
        {
            mCurrentAttack += 0.005f * dt;
            if (mCurrentAttack > 5.f)
            {
                mCurrentAttack -= 5.f;
                mIsAttacked = false;
                mIsBack = false;
                mIsHittedOnce = false;
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
                mSprite.setTextureRect(sf::IntRect(120 * static_cast<int>(mCurrentAttack) + 8, 4, 80, 90));
            }
            else if (dx < 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(120 * static_cast<int>(mCurrentAttack) + 8, 4, 80, 90));
            }
        }
        else
        {
            mCurrentAttack = 0.f;
            if (dx > 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(120 * static_cast<int>(mCurrentFrame) + 8, 4, 80, 90));
            }
            else if (dx < 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(120 * static_cast<int>(mCurrentFrame) + 8, 4, 80, 90));
            }
        }
    }

    if (mHitpoints <= 0)
    {
        mMoveTimer += dt;
        if (mMoveTimer > 1000.f)
        {
            ++mCounter;
            mMoveTimer = 0.f;
        }
        mCurrentDeath += 0.0075f * dt;
        dx = 0.f;
        dy = 0.f;
        if (mCurrentDeath > 11.f)
        {
            mCurrentDeath = 11.f;
            if (mCounter == 8)
                mLife = false;
        }
        mSprite.setPosition(x + (mWidth / 2.f) - 12.f, y + (mHeight / 2.f) - 14.f);
        mSprite.setTextureRect(sf::IntRect(120 * (static_cast<int>(mCurrentDeath) % 5) + 8, 100 * ((static_cast<int>(mCurrentDeath) / 5) + 1) + 4, 105, 90));
    }
}
