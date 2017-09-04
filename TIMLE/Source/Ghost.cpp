#include "../Include/Ghost.hpp"


Ghost::Ghost(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, std::string Type)
: Enemy(Id, textures, fonts, lvl, X, Y, width, height, Type)

{
	mTexture = textures.get(Textures::Ghost);
	mTextureDeath = textures.get(Textures::GhostDeath);
	mSprite.setTexture(mTexture);
	mSprite.setScale(0.5f, 0.5f);
	mSprite.setTextureRect(sf::IntRect(0, 0, mWidth, mHeight));
	mSprite.scale(-1.f, 1.f);
	dx = 0.1f;
}

void Ghost::checkCollisionWithMap(float Dx, float Dy)
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

void Ghost::update(float time)
{
	// Притяжение к земле
	dy += 0.0015f * time;
	y += dy * time;
	checkCollisionWithMap(0.f, dy);

	if (mIsTurned)
		mMoveTimer += time;
	if (mMoveTimer > 500.f && mIsTurned)
	{
		dx = -dx;
		mMoveTimer = 0.f;
		mIsTurned = false;
		mSprite.scale(-1.f, 1.f);
	}

	if (mLife && (mHitpoints > 0))
	{
		if (!mIsAttacked && !mIsTurned)
		{
			x += dx * time;
			mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 15.f : -15.f), y + (mHeight / 2.f) - 20.f);
		}
		else if (!mIsTurned)
		{
			mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 15.f : -15.f), y + (mHeight / 2.f) - 20.f);
		}
		else
		{
			mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 15.f : -15.f), y + (mHeight / 2.f) - 20.f);
		}
		checkCollisionWithMap(dx, 0.f);

		mCurrentFrame += 0.005f * time;
		if (mCurrentFrame > 3.f)
		{
			mCurrentFrame -= 3.f;
		}
		if (mIsAttacked)
		{
			mCurrentAttack += 0.009f * time;
			if (mCurrentAttack > 10.f)
			{
				mCurrentAttack -= 10.f;
				mIsAttacked = false;
				mIsBack = false;
				mIsHittedOnce = false;
			}
			else if (static_cast<int>(mCurrentAttack) == 8 && !mIsHittedOnce)
			{
				mIsHitted = true;
				mIsHittedOnce = true;
			}
			else
			{
				mIsHitted = false;
				mIsBack = true;
			}
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(102 * static_cast<int>(mCurrentAttack), 0, 102, 90));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(102 * static_cast<int>(mCurrentAttack), 0, 102, 90));
			}
		}
		else
		{
			mCurrentAttack = 0.f;
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(102 * static_cast<int>(mCurrentFrame), 0, 102, 90));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(102 * static_cast<int>(mCurrentFrame), 0, 102, 90));
			}
		}
	}

	if (mHitpoints <= 0)
	{
		mCurrentDeath += 0.0035f * time;
		dx = 0.f;
		dy = 0.f;
		mIsTurned = false;
		if (mCurrentDeath > 6.f)
		{
			mCurrentDeath = 6.f;
			mLife = false;
		}
		mSprite.setTexture(mTextureDeath);
		mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f) + 0.f);
		mSprite.setTextureRect(sf::IntRect(64 * (static_cast<int>(mCurrentDeath) < 3 ? static_cast<int>(mCurrentDeath) : static_cast<int>(mCurrentDeath) + 5), 0, 64, 50));
	}
}