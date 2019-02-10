#include "Projectiles/flamestrike.hpp"


Flamestrike::Flamestrike(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
                         const level& lvl, const float X, const float Y, const int width, 
                         const int height)
: Projectile(id, textures, fonts, lvl, X, Y, width, height)
{
    mTexture = textures.get(Textures::ID::Fire);

    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, 128, 256));
    mSprite.setScale(0.5f, 0.5f);
    const auto color = sf::Color(mSprite.getColor().r, mSprite.getColor().g, mSprite.getColor().b, 0);
    mSprite.setColor(color);

    mIsStarted = true;
    mIsEnd = false;
}

void Flamestrike::update(const float dt)
{
    if (mIsStarted)
    {
        mMoveTimer += 0.0085f * dt;

        const auto color = mSprite.getColor();

        if (mMoveTimer > 5.f)
        {
            mIsStarted = false;
            mMoveTimer = -5.f;
        }
        else
        {
            if (color.a + static_cast<sf::Uint8>(mMoveTimer) * 5 > 255)
            {
                mSprite.setColor(sf::Color(color.r, color.g, color.b, 255));
            }
            else
            {
                mSprite.setColor(sf::Color(color.r, color.g, color.b,
                    color.a + static_cast<sf::Uint8>(mMoveTimer) * 5));
            }
        }
    }

    mCurrentFrame += 0.0075f * dt;
    if (mCurrentFrame > 24.f)
    {
        mCurrentFrame -= 24.f;
        mIsEnd = true;
    }

    if (mIsAttacked)
    {
        mCurrentAttack += 0.0075f * dt;
        if (mCurrentAttack > 2.f)
        {
            mCurrentAttack -= 2.f;
            mIsAttacked = false;
            mIsHittedOnce = false;
        }
        else if (static_cast<int>(mCurrentAttack) == 1 && !mIsHittedOnce && !mIsStarted)
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

    mSprite.setTextureRect(sf::IntRect(128 * (static_cast<int>(mCurrentFrame) % 8),
                           256 * (static_cast<int>(mCurrentFrame) / 8), 128, 256));


    mSprite.setPosition(x + (mWidth / 2.f) - 30.f, y + (mHeight / 2.f) - 91.f);

    if (mIsEnd)
    {
        mCurrentDeath += 0.0075f * dt;
        
        const auto color = mSprite.getColor();

        if (mCurrentDeath > 5.f)
        {
            mLife = false;
            mCurrentDeath = -5.f;
        }
        else
        {
            if (color.a - static_cast<sf::Uint8>(mCurrentDeath) * 5 < 0)
            {
                mSprite.setColor(sf::Color(color.r, color.g, color.b, 0));
            }
            else
            {
                mSprite.setColor(sf::Color(color.r, color.g, color.b, 
                                 color.a - static_cast<sf::Uint8>(mCurrentDeath) * 5));
            }
        }
    }
}
