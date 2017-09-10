#include "../Include/MovingPlatform.hpp"


MovingPlatform::MovingPlatform(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height)
: Entity(Id, X, Y, width, height, 50.f, 100, 0)
{
	mTexture = textures.get(Textures::MovingPlatform);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, width, height));
	// Изначальное ускорение
	dx = 0.08f;
}

void MovingPlatform::update(float dt)
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