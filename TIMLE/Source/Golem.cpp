#include "../Include/Golem.hpp"


Golem::Golem(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
             const Level& lvl, const float X, const float Y, const int width,
             const int height, const std::string& type)
: Enemy(id, textures, fonts, lvl, X, Y, width, height, type)
{
    mTexture = textures.get(Textures::ID::Golem);
    mTextureAttack = textures.get(Textures::ID::GolemAttack);
    mTextureDeath = textures.get(Textures::ID::GolemDeath);
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 192, mWidth, mHeight));
    mSprite.setScale(0.6f, 0.6f);
    dx = 0.05f;
    mIsEnd = false;
}

void Golem::checkCollisionWithMap(const float Dx, const float Dy)
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

void Golem::update(const float dt)
{
    // Притяжение к земле
    dy += 0.0015f * dt;
    y += dy * dt;
    checkCollisionWithMap(0.f, dy);

    if (!mIsStarted)
    {
        mSprite.setTexture(mTextureDeath);
        mSprite.setPosition(x + (mWidth / 2.f) - 6.f, y + (mHeight / 2.f) - 4.f);
        mSprite.setTextureRect(sf::IntRect(64 * 5, 64, 64, 64));
        mCurrentDeath = 5.f;
        return;
    }
    else if (!mIsEnd)
    {
        mCurrentDeath -= 0.005f * dt;
        if (mCurrentDeath < 0.f)
        {
            mCurrentDeath = 0.f;
            mIsEnd = true;
        }
        mSprite.setTexture(mTextureDeath);
        mSprite.setPosition(x + (mWidth / 2.f) - 6.f, y + (mHeight / 2.f) - 4.f);
        mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentDeath), 64, 64, 64));
        return;
    }

    if (mIsTurned)
    {
        mMoveTimer += dt;
    }
    if (mMoveTimer > 2000.f && mIsTurned)
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

        mSprite.setPosition(x + (mWidth / 2.f) + 2.f, y + (mHeight / 2.f) - 7.f);

        mCurrentFrame += 0.005f * dt;
        if (mCurrentFrame > 7.f)
        {
            mCurrentFrame -= 7.f;
        }
        if (mIsAttacked)
        {
            mCurrentAttack += 0.005f * dt;
            if (mCurrentAttack > 7.f)
            {
                mCurrentAttack -= 7.f;
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
            mSprite.setTexture(mTextureAttack);
            mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 3.f : -10.f), y + (mHeight / 2.f) - 12.f);
            if (dx > 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentAttack), 311, 64, 72));
            }
            else if (dx < 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentAttack), 119, 64, 72));
            }
        }
        else
        {
            mSprite.setTexture(mTexture);
            mCurrentAttack = 0.f;
            if (mIsTurned)
            {
                mCurrentFrame = 0.f;
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + (dx > 0 ? 0 : 17), (dx > 0 ? 192 : 64), 48, 64));
            }
            if (dx > 0.f && !mIsTurned)
            {
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame), 192, 48, 64));
            }
            else if (dx < 0.f && !mIsTurned)
            {
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + 17, 64, 48, 64));
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
        if (mCurrentDeath > 6.f)
        {
            mCurrentDeath = 6.f;
            if (mCounter == 6)
            {
                mLife = false;
            }
        }
        mSprite.setTexture(mTextureDeath);
        mSprite.setPosition(x + (mWidth / 2.f) - 4.f, y + (mHeight / 2.f) - 3.f);
        mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentDeath), 0, 64, 64));
    }
}
