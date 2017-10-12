#include "../Include/Bullet.hpp"


Bullet::Bullet(Type::ID Id,const TextureHolder& textures, const FontHolder& fonts, Level &lvl,
			   float X, float Y, int width, int height, float tX, float tY)
: Projectile(Id, textures, fonts, lvl, X, Y, width, height)	// При клике мышкой
, mTargetX(tX)
, mTargetY(tY)
, mVx((mTargetX - x) / 100.f)
, mVy((mTargetY - y) / 100.f)	// 100 - дистанция
, mDistance(sqrt((mTargetX - x) - (mTargetY - y)))
, mIsMouse(true)
{
	mLevelObjects = lvl.getObjects("solid");
	mSpeed = 0.2f;
	mTexture = textures.get(Textures::Bullet);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, width, height));
	mSprite.setScale(0.5f, 0.5f);
}

Bullet::Bullet(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl,
			   float X, float Y, int width, int height, int dir)
: Projectile(Id, textures, fonts, lvl, X, Y, width, height)	// При нажатии space
, mTargetX(X + 150.f * ((dir - 1 == 0) || (dir - 1 == 4) ? 1.f : -1.f))
, mTargetY(Y)
, mVx((mTargetX - x) / 100.f)
, mVy((mTargetY - y) / 100.f)	// 100 - дистанция
, mDistance(sqrt((mTargetX - x) - (mTargetY - y)))
, mIsMouse(false)
{
	mLevelObjects = lvl.getObjects("solid");
	mSpeed = 0.2f;
	mTexture = textures.get(Textures::Bullet);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, width, height));
	mSprite.setScale(0.5f, 0.5f);
}

void Bullet::update(float dt)
{

	//x += mSpeed*time*(targetX - x) /20;	// Само движение пули по х
	//y += mSpeed*time*(targetY - y) /20;	// По у

	x += mVx * dt * mSpeed;
	y += mVy * dt * mSpeed;

	// Задержка пули в левой стене, чтобы при проседании кадров она случайно 
	// не вылетела за предел карты и не было ошибки
	if (x <= 0.f)
	{
		x = -mSprite.getLocalBounds().width;
	}
	if (y <= 0.f)
	{
		y = -mSprite.getLocalBounds().height;
	}
	x += dx * dt;	// Само движение пули по х
	y += dy * dt;	// По у
	for (size_t i = 0; i < mLevelObjects.size(); i++)	// Проход по объектам solid
	{
		if (getRect().intersects(mLevelObjects[i].mRect))
		{
			mLife = false;
		}
	}
	if (mMoveTimer > 3000)
	{
		mLife = false;
	}

	mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 2.f);

	mMoveTimer += dt;

	mCurrentFrame += 0.005f * dt;
	if (mCurrentFrame > 5)
	{
		mCurrentFrame -= 5;
	}
	mSprite.setTextureRect(sf::IntRect(13 * static_cast<int>(mCurrentFrame), 0, 13, 13));
}