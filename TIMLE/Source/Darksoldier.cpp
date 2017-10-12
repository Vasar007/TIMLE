#include "../Include/DarkSoldier.hpp"


DarkSoldier::DarkSoldier(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, 
						 Level &lvl, float X, float Y, int width, int height, std::string Type)
: Enemy(Id, textures, fonts, lvl, X, Y, width, height, Type)
{
	mTexture = textures.get(Textures::DarkSoldier);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(14, 131, mWidth, mHeight));
	mSprite.setScale(0.5f, 0.5f);
	dx = 0.05f;
}

void DarkSoldier::checkCollisionWithMap(float Dx, float Dy)
{
	for (size_t i = 0; i < mLevelObjects.size(); i++)
	{
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
}

void DarkSoldier::update(float dt)
{
	if (mIsTurned)
	{
		mMoveTimer += dt;
	}
	if (mMoveTimer > 2000.f && mIsTurned)
	{
		dx = -dx;
		mMoveTimer = 0.f;
		mIsTurned = false;
	}

	if (mLife && (mHitpoints > 0))
	{
		if (!mIsAttacked && !mIsTurned)
		{
			x += dx * dt;
			mSprite.setPosition(x + (mWidth / 2.f) - 4.f, y + (mHeight / 2.f) - 9.f);
		}
		else if (!mIsTurned)
		{
			mSprite.setPosition(x + (mWidth / 2.f) - 4.f, y + (mHeight / 2.f) - 7.f);
		}
		else
		{
			mSprite.setPosition(x + (mWidth / 2.f) - 4.f, y + (mHeight / 2.f) - 7.f);
		}
		checkCollisionWithMap(dx, 0.f);

		// Притяжение к земле
		dy += 0.0015f * dt;
		y += dy * dt;
		checkCollisionWithMap(0.f, dy);

		mCurrentFrame += 0.005f * dt;
		if (mCurrentFrame > 5.f)
		{
			mCurrentFrame -= 5.f;
		}
		if (mIsAttacked)
		{
			mCurrentAttack += 0.005f * dt;
			if (mCurrentAttack > 4.f)
			{
				mCurrentAttack = 0.f;
				mIsAttacked = false;
				mIsHitted = true;
				mIsBack = false;
			}
			else
			{
				mIsAttacked = false;
				mIsHitted = false;
			}
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(0 + 14, 3, 40, 60));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(0 + 14, 3, 40, 60));
			}
		}
		else
		{
			mCurrentAttack = 0.f;
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(64 * (mIsTurned ? 0 : static_cast<int>(mCurrentFrame)) + 14, (mIsTurned ? 3 : 131), 40, 60));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(64 * (mIsTurned ? 0 : static_cast<int>(mCurrentFrame)) + 14, (mIsTurned ? 3 : 195), 40, 60));
			}
		}
	}

	if (mHitpoints <= 0)
	{
		mCurrentDeath += 0.0035f * dt;
		dx = 0.f;
		dy = 0.f;
		if (mCurrentDeath > 5.f)
		{
			mCurrentDeath = 5.f;
			mLife = false;
		}
		mSprite.setPosition(x + (mWidth / 2.f) - 4.f, y + (mHeight / 2.f) - 7.f);
		mSprite.setTextureRect(sf::IntRect(0 + 14, 3, 40, 60));
	}
}