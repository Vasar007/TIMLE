#include "../Include/GolemDark.hpp"


namespace
{
	const GolemDarkData GBTable = initializeGolemDarkData();
}

GolemDark::GolemDark(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type)
: Entity(Id, X, Y, width, height, GBTable.speed, GBTable.hitpoints, GBTable.damage)
, mCounter(0)
, mIsTurned(false)
{
	// Инициализируем.получаем нужные объекты для взаимодействия голема с картой
	std::vector<Object> levelObjects = lvl.getObjects("enemyBorder");
	for (size_t i = 0; i < levelObjects.size(); i++)
		if (levelObjects[i].mType == Type)
			mLevelObjects.push_back(levelObjects[i]);

	levelObjects = lvl.getObjects("death");
	for (size_t i = 0; i < levelObjects.size(); i++)
		mLevelObjects.push_back(levelObjects[i]);

	mTexture = textures.get(Textures::GolemDark);
	mTextureAttack = textures.get(Textures::GolemDarkAttack);
	mTextureDeath = textures.get(Textures::GolemDarkDeath);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 192, 64, 64));
	mSprite.setScale(1.0f, 1.0f);
	dx = 0.07f;
	mIsEnd = false;
}

void GolemDark::checkCollisionWithMap(float Dx, float Dy)
{
	for (size_t i = 0; i < mLevelObjects.size(); i++)
		// Проверяем пересечение с объектом
		if (getRect().intersects(mLevelObjects[i].mRect))
		{
			if (mLevelObjects[i].mName == "enemyBorder")
			{
				if (Dy > 0.f)
				{
					y = mLevelObjects[i].mRect.top - mHeight;
					dy = 0.f;
					mOnGround = true;
				}
				if (Dy < 0.f)
				{
					y = mLevelObjects[i].mRect.top + mLevelObjects[i].mRect.height;
					dy = 0.f;
				}
				if (Dx > 0.f)
				{
					x = mLevelObjects[i].mRect.left - mWidth;
					mIsTurned = true;
				}
				if (Dx < 0.f)
				{
					x = mLevelObjects[i].mRect.left + mLevelObjects[i].mRect.width;
					mIsTurned = true;;
				}
			}

			// Если встретили смерть
			if (mLevelObjects[i].mName == "death")
			{
				mHitpoints = 0;
			}
		}
}

void GolemDark::update(float time)
{
	// Притяжение к земле
	dy += 0.0015f * time;
	y += dy * time;
	checkCollisionWithMap(0.f, dy);

	if (!mIsStarted)
	{
		mSprite.setTexture(mTextureDeath);
		mSprite.setPosition(x + (mWidth / 2.f) + 3.f, y + (mHeight / 2.f) + 1.f);
		mSprite.setTextureRect(sf::IntRect(64 * 5, 64, 64, 64));
		mCurrentDeath = 5.f;
		return;
	}
	else if (!mIsEnd)
	{
		mCurrentDeath -= 0.005f * time;
		if (mCurrentDeath < 0.f)
		{
			mCurrentDeath = 0.f;
			mIsEnd = true;
		}
		mSprite.setTexture(mTextureDeath);
		mSprite.setPosition(x + (mWidth / 2.f) + 3.f, y + (mHeight / 2.f) + 1.f);
		mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentDeath), 64, 64, 64));
		return;
	}

	mMoveTimer += time;
	if (mMoveTimer > 2000.f && mIsTurned)
	{
		dx = -dx;
		mMoveTimer = 0.f;
		mIsTurned = false;
	}
	else if (mMoveTimer > 1000.f && !mIsTurned)
	{
		mMoveTimer = 0.f;
		if (mHitpoints <= 0)
			mCounter++;
	}

	if (mLife && (mHitpoints > 0))
	{
		if (!mIsAttacked && !mIsTurned)
			x += dx * time;
		checkCollisionWithMap(dx, 0.f);

		mSprite.setPosition(x + (mWidth / 2.f) + 11.f, y + (mHeight / 2.f) - 2.f);


		mCurrentFrame += 0.005f * time;
		if (mCurrentFrame > 7.f)
		{
			mCurrentFrame -= 7.f;
		}
		if (mIsAttacked)
		{
			mCurrentAttack += 0.005f * time;
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
			else {
				mIsHitted = false;
			}
			mSprite.setTexture(mTextureAttack);
			mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 13.f : -4.f), y + (mHeight / 2.f) - 11.f);
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
		mCurrentDeath += 0.0035f * time;
		dx = 0.f;
		dy = 0.f;
		if (mCurrentDeath > 6.f)
		{
			mCurrentDeath = 6.f;
			if (mCounter == 6)
				mLife = false;
		}
		mSprite.setTexture(mTextureDeath);
		mSprite.setPosition(x + (mWidth / 2.f) + 5.f, y + (mHeight / 2.f) + 1.f);
		mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentDeath), 0, 64, 64));
	}
}
