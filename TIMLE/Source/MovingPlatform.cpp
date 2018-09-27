#include "../Include/MovingPlatform.hpp"


MovingPlatform::MovingPlatform(Type::ID id, const TextureHolder& textures, const FontHolder&,
                               const Level&, const float X, const float Y, const int width, 
                               const int height)
: Entity(id, X, Y, width, height, 50.f, 100, 0)
{
    mTexture = textures.get(Textures::ID::MovingPlatform);
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, width, height));
    mSprite.setScale(0.5f, 0.5f);
    // Изначальное ускорение
    dx = 0.08f;
}

void MovingPlatform::update(const float dt)
{
    // Движение по горизонтали
    x += dx * dt;
    mMoveTimer += dt;
    if (mMoveTimer > 2000.f)
    {
        dx *= -1.f;
        // Если прошло 2 секунды, то меняем направление движения платформы на противоположное
        mMoveTimer = 0.f;
    }
    mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
}