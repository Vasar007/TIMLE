#include "../Include/Goblin.hpp"


Goblin::Goblin(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, std::string Type)
: Enemy(Id, textures, fonts, lvl, X, Y, width, height, Type)
{
	mTexture = textures.get(Textures::Goblin);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(10, 67, mWidth, mHeight));
	mSprite.setScale(0.5f, 0.5f);
	dx = 0.07f;
}

void Goblin::checkCollisionWithMap(float Dx, float Dy)
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

void Goblin::update(float time)
{
	// Притяжение к земле
	dy += 0.0015f * time;
	y += dy * time;
	checkCollisionWithMap(0.f, dy);

	if (mIsTurned)
		mMoveTimer += time;
	if (mMoveTimer > 2000.f && mIsTurned)
	{
		dx = -dx;
		mMoveTimer = 0.f;
		mIsTurned = false;
	}

	if (mLife && (mHitpoints > 0))
	{
		if (!mIsAttacked && !mIsTurned)
			x += dx * time;
		checkCollisionWithMap(dx, 0.f);

		mCurrentFrame += 0.005f * time;
		if (mCurrentFrame > 6.f)
		{
			mCurrentFrame -= 6.f;
		}

		mSprite.setPosition(x + (mWidth / 2.f) + (static_cast<int>(mCurrentFrame) == 0 ? -5.f : -6.f), y + (mHeight / 2.f) - 5.f);

		if (mIsAttacked)
		{
			int temp;
			mCurrentAttack += 0.0075f * time;
			if (mCurrentAttack > 7.f)
			{
				mCurrentAttack -= 7.f;
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
			switch (static_cast<int>(mCurrentAttack))
			{
				case 0:
					temp = 7;
					break;
				case 1:
					temp = 10;
					break;
				case 2:
					temp = 9;
					break;
				case 3:
					temp = 8;
					break;
				case 4:
					temp = 9;
					break;
				case 5:
					temp = 10;
					break;
				case 6:
					temp = 7;
					break;
				default:
					temp = 0;
					break;
			}
			mSprite.setPosition(x + (mWidth / 2.f) + (static_cast<int>(mCurrentFrame) == 0 ? -5.f : -6.f) + (dx > 0 ? - 3.f : 0.f), y + (mHeight / 2.f) - 5.f);
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(64 * temp + 10, 67, 60, 56));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(64 * temp + 10, 195, 60, 56));
			}
		}
		else
		{
			mCurrentAttack = 0.f;
			if (mIsTurned)
			{
				mCurrentFrame = 7.f;
				mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + 10, (dx > 0 ? 67 : 195), 50, 56));
			}
			if (dx > 0.f && !mIsTurned)
			{
				mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + 10, 67, 50, 56));
			}
			else if (dx < 0.f && !mIsTurned)
			{
				mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + 10, 195, 50, 56));
			}
		}
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
		mSprite.setPosition(x + (mWidth / 2.f) - 5.f, y + (mHeight / 2.f) - 5.f);
		mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentDeath) + 10, 259, 50, 56));
	}
}