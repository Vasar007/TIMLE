#include "tentacle.hpp"


Tentacle::Tentacle(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
                   const level& lvl, const float X, const float Y, const int width,
                   const int height, const std::string& type)
: Enemy(id, textures, fonts, lvl, X, Y, width, height, type)
, _appearing(0.f)
, _disappearing(0.f)
{
    mTexture = textures.get(Textures::ID::Tentacle);
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, mWidth, mHeight));
    mSprite.setScale(0.5f, 0.5f);
    dx = 0.f;
}

void Tentacle::checkCollisionWithMap(const float, const float)
{
    for (const auto& object : mLevelObjects)
    {
        // Проверяем пересечение с объектом
        if (getRect().intersects(object.rect))
        {
            // Если встретили смерть
            if (object.name == "death")
            {
                mHitpoints = 0;
            }
        }
    }
}

void Tentacle::appear(const float dt)
{
    _appearing += 0.0075f * dt;
    if (_appearing >= 13.f)
    {
        _appearing = 0.f;
        mIsEnabled = false;
        return;
    }
    mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
    mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(_appearing) < 5 ? static_cast<int>(_appearing) : static_cast<int>(_appearing) - 5),
        90 * (static_cast<int>(_appearing) < 5 ? 0 : 1), 25, 90));

}

void Tentacle::disappear(const float dt)
{
    _disappearing += 0.0075f * dt;
    if (_disappearing >= 13.f)
    {
        _disappearing = 0.f;
        mIsDisabled = false;
        return;
    }

    mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
    mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(_disappearing) < 8 ? 7 - static_cast<int>(_disappearing): 12 - static_cast<int>(_disappearing)),
        90 * (static_cast<int>(_disappearing) < 8 ? 1 : 0), 25, 90));
}

void Tentacle::update(const float dt)
{
    if (!mIsStarted)
    {
        mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
        mSprite.setTextureRect(sf::IntRect(0, 0, 25, 90));
        mCurrentDeath = 0.f;
        return;
    }
    if (!mIsEnd)
    {
        mCurrentDeath += 0.0075f * dt;
        if (mCurrentDeath > 12.f)
        {
            mCurrentDeath = 12.f;
            mIsEnd = true;
            mIsEnabling = true;
        }
        mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
        mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mCurrentDeath) < 5 ? static_cast<int>(mCurrentDeath) : static_cast<int>(mCurrentDeath) - 5),
                                           90 * (static_cast<int>(mCurrentDeath) < 5 ? 0 : 1), 25, 90));
        return;
    }

    if (mLife && (mHitpoints > 0))
    {
        if (mIsEnabled)
        {
            appear(dt);
        }

        if (mIsDisabled)
        {
            disappear(dt);
        }

        if (!mIsEnabled && !mIsDisabled && mIsEnabling)
        {
            mCurrentFrame += 0.0075f * dt;
            if (mCurrentFrame > 8.f)
            {
                mCurrentFrame -= 8.f;
            }
            checkCollisionWithMap(dx, 0.f);

            if (mIsAttacked)
            {
                mCurrentAttack += 0.005f * dt;
                if (mCurrentAttack > 2.f)
                {
                    mCurrentAttack -= 1.f;
                    mIsAttacked = false;
                    mIsHittedOnce = false;
                }
                else if (static_cast<int>(mCurrentAttack) == 1 && !mIsHittedOnce)
                {
                    mIsHitted = true;
                    mIsHittedOnce = true;
                }
                else
                {
                    mIsHitted = false;
                }
            }
            else
            {
                mCurrentAttack = 0.f;
            }
            mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
            mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mCurrentFrame) % 8), 90 * 2, 25, 90));
            mCurrentDeath = 0.f;
        }
    }

    if (mHitpoints <= 0)
    {
        mCurrentDeath += 0.0075f * dt;
        dx = 0.f;
        dy = 0.f;
        mIsTurned = false;
        if (mCurrentDeath >= 12.f)
        {
            mCurrentDeath = 12.f;
            mLife = false;
        }
        mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
        mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mCurrentDeath) < 8 ? 7 - static_cast<int>(mCurrentDeath) : 12 - static_cast<int>(mCurrentDeath)),
            90 * (static_cast<int>(mCurrentDeath) < 8 ? 1 : 0), 25, 90));
    }
}
