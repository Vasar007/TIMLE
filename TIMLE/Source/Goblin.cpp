#include "../Include/Goblin.hpp"


Goblin::Goblin(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
               const Level& lvl, const float X, const float Y, const int width,
               const int height, const std::string& type)
: Enemy(id, textures, fonts, lvl, X, Y, width, height, type)
{
    mTexture = textures.get(Textures::ID::Goblin);
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(10, 67, mWidth, mHeight));
    mSprite.setScale(0.5f, 0.5f);
    dx = 0.07f;
}

void Goblin::checkCollisionWithMap(const float Dx, const float Dy)
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
                    mIsTurned = true;;
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

void Goblin::update(const float dt)
{
    // Притяжение к земле
    dy += 0.0015f * dt;
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
    }

    if (mLife && (mHitpoints > 0))
    {
        if (!mIsAttacked && !mIsTurned)
        {
            x += dx * dt;
        }
        checkCollisionWithMap(dx, 0.f);

        mCurrentFrame += 0.005f * dt;
        if (mCurrentFrame > 6.f)
        {
            mCurrentFrame -= 6.f;
        }

        mSprite.setPosition(x + (mWidth / 2.f) + (static_cast<int>(mCurrentFrame) == 0 ? -5.f : -6.f), y + (mHeight / 2.f) - 5.f);

        if (mIsAttacked)
        {
            int temp;
            mCurrentAttack += 0.0075f * dt;
            if (mCurrentAttack > 7.f)
            {
                mCurrentAttack -= 7.f;
                mIsAttacked = false;
                mIsBack = false;
                mIsHittedOnce = false;
            }
            else if (static_cast<int>(mCurrentAttack) == 3 && !mIsHittedOnce)
            {
                mIsHitted = true;
                mIsHittedOnce = true;
            }
            else
            {
                mIsHitted = false;
            }
            switch (static_cast<int>(mCurrentAttack))
            {
                case 0:
                    temp = 7;
                    break;
                case 1:
                    temp = 10;
                    break;
                case 2:
                    temp = 9;
                    break;
                case 3:
                    temp = 8;
                    break;
                case 4:
                    temp = 9;
                    break;
                case 5:
                    temp = 10;
                    break;
                case 6:
                    temp = 7;
                    break;
                default:
                    temp = 0;
                    break;
            }
            mSprite.setPosition(x + (mWidth / 2.f) + (static_cast<int>(mCurrentFrame) == 0 ? -5.f : -6.f) + (dx > 0 ? - 3.f : 0.f), y + (mHeight / 2.f) - 5.f);
            if (dx > 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(64 * temp + 10, 67, 60, 56));
            }
            else if (dx < 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(64 * temp + 10, 195, 60, 56));
            }
        }
        else
        {
            mCurrentAttack = 0.f;
            if (mIsTurned)
            {
                mCurrentFrame = 7.f;
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + 10, (dx > 0 ? 67 : 195), 50, 56));
            }
            if (dx > 0.f && !mIsTurned)
            {
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + 10, 67, 50, 56));
            }
            else if (dx < 0.f && !mIsTurned)
            {
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + 10, 195, 50, 56));
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
        mCurrentDeath += 0.0035f * dt;
        dx = 0.f;
        dy = 0.f;
        if (mCurrentDeath > 2.f)
        {
            mCurrentDeath = 2.f;
            if (mCounter == 6)
            {
                mLife = false;
            }
        }
        mSprite.setPosition(x + (mWidth / 2.f) - 5.f, y + (mHeight / 2.f) - 5.f);
        mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentDeath) + 10, 259, 50, 56));
    }
}
