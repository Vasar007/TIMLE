#include "../Include/Bullet.hpp"
#include "../Include/DataTables.hpp"


namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Bullet::Bullet(Type::ID Id,const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, float tX, float tY)
: Entity(Id, X, Y, width, height, Table[Id - Type::HeroCount - Type::EnemyCount].speed, 100, Table[Id - Type::HeroCount - Type::EnemyCount].damage)	// При клике мышкой
, mTargetX(tX)
, mTargetY(tY)
, mVx((mTargetX - x) / 100.f)
, mVy((mTargetY - y) / 100.f)	// 100 - дистанция
, mDistance(sqrt((mTargetX - x) - (mTargetY - y)))
, mIsMouse(true)
{
	mLevelObjects = lvl.getObjects("solid");	// Инициализируем .получаем нужные объекты для взаимодействия пули с картой
	mSpeed = 0.2f;
	mTexture = textures.get(Textures::Bullet);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, width, height));
	mSprite.setScale(0.5f, 0.5f);
}

Bullet::Bullet(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, int dir)
: Entity(Id, X, Y, width, height, Table[Id - Type::HeroCount - Type::EnemyCount].speed, 100, Table[Id - Type::HeroCount - Type::EnemyCount].damage)	// При нажатии space
, mTargetX(X + 150.f * ((dir - 1 == 0) || (dir - 1 == 4) ? 1.f : -1.f))
, mTargetY(Y)
, mVx((mTargetX - x) / 100.f)
, mVy((mTargetY - y) / 100.f)	// 100 - дистанция
, mDistance(sqrt((mTargetX - x) - (mTargetY - y)))
, mIsMouse(false)
{
	mLevelObjects = lvl.getObjects("solid");	// Инициализируем .получаем нужные объекты для взаимодействия пули с картой
	mSpeed = 0.2f;
	mTexture = textures.get(Textures::Bullet);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, width, height));
	mSprite.setScale(0.5f, 0.5f);
}

void Bullet::update(float time)
{

	//x += mSpeed*time*(targetX - x) /20;	// Само движение пули по х
	//y += mSpeed*time*(targetY - y) /20;	// По у

	x += mVx * time * mSpeed;
	y += mVy * time * mSpeed;

	if (x <= 0.f)
		x = -mSprite.getLocalBounds().width;	// Задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
	if (y <= 0.f)
		y = -mSprite.getLocalBounds().height;
	x += dx * time;	// Само движение пули по х
	y += dy * time;	// По у
	for (size_t i = 0; i < mLevelObjects.size(); i++)	// Проход по объектам solid
	{
		if (getRect().intersects(mLevelObjects[i].mRect))	// Если этот объект столкнулся с пулей,
		{
			mLife = false;	// То пуля умирает
		}
	}
	if (mMoveTimer > 3000)
	{
		mLife = false;
	}

	mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 2.f);	// Задается позицию пуле

	mMoveTimer += time;	// Наращиваем таймер

	mCurrentFrame += 0.005f * time;
	if (mCurrentFrame > 5)
		mCurrentFrame -= 5;
	mSprite.setTextureRect(sf::IntRect(13 * static_cast<int>(mCurrentFrame), 0, 13, 13));
}