#include "moving_platform.hpp"


MovingPlatform::MovingPlatform(Type::ID id, const TextureHolder& textures, const FontHolder&,
                               const level& lvl, const float X, const float Y, const int width, 
                               const int height, const std::string& type)
: Entity(id, X, Y, width, height, 50.f, 100, 0, type)
{
    for (const auto& object : lvl.get_objects("platformBorder"))
    {
        if (object.type == type)
        {
            mLevelObjects.push_back(object);
        }
    }

    mTexture = textures.get(Textures::ID::MovingPlatform);
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, width, height));
    mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
    // Изначальное ускорение
    dx = 0.08f;
}

void MovingPlatform::checkCollisionWithMap()
{
    for (auto& mLevelObject : mLevelObjects)
    {
        // Проверяем пересечение с объектом
        if (getRect().intersects(mLevelObject.rect))
        {
            dx = -dx;
            dy = -dy;
        }
    }
}

void MovingPlatform::update(const float dt)
{
    // Движение по горизонтали
    x += dx * dt;
    checkCollisionWithMap();

    //mMoveTimer += dt;
    //if (mMoveTimer > 2000.f)
    //{
    //    dx *= -1.f;
    //    // Если прошло 2 секунды, то меняем направление движения платформы на противоположное
    //    mMoveTimer = 0.f;
    //}
    mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
}