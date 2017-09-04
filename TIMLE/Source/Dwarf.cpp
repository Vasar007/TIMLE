#include "../Include/Dwarf.hpp"


Dwarf::Dwarf(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, std::string Type, int dwarfType)
: Enemy(Id, textures, fonts, lvl, X, Y, width, height, Type)
, mDwarfType(dwarfType)
, mInaccuracy(0.f)
{
	mTexture = textures.get(Textures::Dwarf);
	mSprite.setTexture(mTexture);
	mSprite.setScale(0.5f, 0.5f);
	mSprite.setTextureRect(sf::IntRect(0, 80 * mDwarfType, mWidth, mHeight));
	dx = 0.075f;
	mSprite.scale(-1.f, 1.f);
	if (Id == Type::DwarvenCommander)
		mIsSpawn = false;
	else
		mIsSpawn = true;
}

void Dwarf::checkCollisionWithMap(float Dx, float Dy)
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
					mIsTurned = true;
				}
			}

			// Если встретили смерть
			if (mLevelObjects[i].mName == "death")
			{
				mHitpoints = 0;
			}
		}
}

void Dwarf::update(float time)
{
	// Притяжение к земле
	dy += 0.0015f * time;
	y += dy * time;
	checkCollisionWithMap(0.f, dy);

	if (!mIsStarted)
	{
		mCurrentDeath += 0.005f * time;
		if (mCurrentDeath > 4.f)
		{
			mCurrentDeath -= 4.f;
		}
		mSprite.setPosition(x + (mWidth / 2.f) + 16.f, y + (mHeight / 2.f) - 19.f);
		mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentDeath) + 16), 80 * mDwarfType, 100, 80));
		mCurrentAttack = 3.f;
		return;
	}
	else if (!mIsEnd)
	{
		mCurrentAttack -= 0.0025f * time;
		if (mCurrentAttack < 0.f)
		{
			mCurrentAttack = 0.f;
			mCurrentDeath = 0.f;
			mIsEnd = true;
		}
		int temp;
		if (static_cast<int>(mCurrentAttack) == 3)
		{
			temp = 0;
		}
		else if (static_cast<int>(mCurrentAttack) == 2)
		{
			temp = 14;
		}
		else
		{
			temp = 13;
		}
		mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 19.f);
		mSprite.setTextureRect(sf::IntRect(100 * temp, 80 * mDwarfType, 100, 80));
		return;
	}

	if (mIsTurned)
		mMoveTimer += time;
	if (mMoveTimer > 2000.f && mIsTurned)
	{
		dx = -dx;
		mMoveTimer = 0;
		mIsTurned = false;
		mSprite.scale(-1.f, 1.f);
	}

	if (mLife && (mHitpoints > 0))
	{
		if (!mIsAttacked && !mIsTurned)
			x += dx * time;
		checkCollisionWithMap(dx, 0.f);

		mSprite.setPosition(x + (mWidth / 2) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2) - 19.f);

		mCurrentFrame += 0.005f * time;
		if (mCurrentFrame > 6.f)
		{
			mCurrentFrame -= 6.f;
		}
		if (mIsAttacked)
		{
			mCurrentAttack += 0.005f * time;
			if (mCurrentAttack > 4.f)
			{
				mCurrentAttack -= 4.f;
				mIsAttacked = false;
				mIsBack = false;
				mIsHittedOnce = false;
			}
			else if (static_cast<int>(mCurrentAttack) == 3 && !mIsHittedOnce)
			{
				mIsHitted = true;
				mIsHittedOnce = true;
			}
			else
			{
				mIsHitted = false;
			}

			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentAttack) + 10), 80 * mDwarfType, 100, 80));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentAttack) + 10), 80 * mDwarfType, 100, 80));
			}
		}
		else
		{
			mCurrentAttack = 0.f;
			if (mIsTurned)
			{
				mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentFrame) % 4), 80 * mDwarfType, 100, 80));
			}
			if (dx > 0.f && !mIsTurned)
			{
				mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentFrame) + 4), 80 * mDwarfType, 100, 80));
			}
			else if (dx < 0.f && !mIsTurned)
			{
				mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentFrame) + 4), 80 * mDwarfType, 100, 80));
			}
		}
		mInaccuracy = dx;
	}

	if (mHitpoints <= 0)
	{
		mMoveTimer += time;
		if (mMoveTimer > 1000.f)
		{
			mCounter++;
			mMoveTimer = 0.f;
		}
		mCurrentDeath += 0.0035f * time;
		dx = 0.f;
		dy = 0.f;
		if (mCurrentDeath > 2.f)
		{
			mCurrentDeath = 2.f;
			if (mCounter == 6)
				mLife = false;
		}
		mSprite.setPosition(x + (mWidth / 2.f) + (mInaccuracy > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 19.f);
		mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentDeath) + (static_cast<int>(mCurrentDeath) == 0? 0 : 13)), 80 * mDwarfType, 100, 80));
	}
}