#include "Objects/rock.hpp"


Rock::Rock(const Type::ID id, const TextureHolder& textures, const FontHolder&, const level& lvl,
           const float X, const float Y, const int height, const std::string& type)
: Entity(id, X, Y, stoi(type) * 16, height, 50.f, 100, 0, type)
, mSize(std::stoi(type))
{
    mLevelObjects = lvl.get_objects("solid");

    mTexture = textures.get(Textures::ID::RockDirt);
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, 16 * mSize, height));
    mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
}

void Rock::fall()
{
    mIsAttacked = true;
}

void Rock::checkCollisionWithMap(const float, const float Dy)
{
    for (auto& mLevelObject : mLevelObjects)
    {
        // Проверяем пересечение с объектом
        if (getRect().intersects(mLevelObject.rect))
        {
            if (Dy > 0.f)
            {
                y = mLevelObject.rect.top - mHeight;
                dy = 0.f;
                mIsEnd = true;
            }
            if (Dy < 0.f)
            {
                y = mLevelObject.rect.top + mLevelObject.rect.height;
                dy = 0.f;
            }
        }
    }
}

void Rock::update(const float dt)
{
    if (mIsAttacked)
    {
        // Not using now.
        //mCurrentFrame += 0.005f * dt;

        // Притяжение к земле
        dy += 0.0005f * dt;
        y += dy * dt;
        checkCollisionWithMap(0.f, dy);

        mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
    }
}
