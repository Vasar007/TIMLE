#include "../Include/Tentacle.hpp"


Tentacle::Tentacle(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, 
				   float X, float Y, int width, int height, std::string Type)
: Enemy(Id, textures, fonts, lvl, X, Y, width, height, Type)
, mAppearing(0.f)
, mDisappearing(0.f)
{
	mTexture = textures.get(Textures::Tentacle);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, mWidth, mHeight));
	mSprite.setScale(0.5f, 0.5f);
	dx = 0.f;
}

void Tentacle::checkCollisionWithMap(float Dx, float Dy)
{
	for (size_t i = 0; i < mLevelObjects.size(); i++)
	{
		// Проверяем пересечение с объектом
		if (getRect().intersects(mLevelObjects[i].mRect))
		{
			// Если встретили смерть
			if (mLevelObjects[i].mName == "death")
			{
				mHitpoints = 0;
			}
		}
	}
}

void Tentacle::appear(float dt)
{
	mAppearing += 0.0075f * dt;
	if (mAppearing >= 13.f)
	{
		mAppearing = 0.f;
		mIsEnabled = false;
		return;
	}
	mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
	mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mAppearing) < 5 ? static_cast<int>(mAppearing) : static_cast<int>(mAppearing) - 5),
		90 * (static_cast<int>(mAppearing) < 5 ? 0 : 1), 25, 90));

}

void Tentacle::disappear(float dt)
{
	mDisappearing += 0.0075f * dt;
	if (mDisappearing >= 13.f)
	{
		mDisappearing = 0.f;
		mIsDisabled = false;
		return;
	}

	mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
	mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mDisappearing) < 8 ? 7 - static_cast<int>(mDisappearing): 12 - static_cast<int>(mDisappearing)),
		90 * (static_cast<int>(mDisappearing) < 8 ? 1 : 0), 25, 90));
}

void Tentacle::update(float dt)
{
	if (!mIsStarted)
	{
		mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
		mSprite.setTextureRect(sf::IntRect(0, 0, 25, 90));
		mCurrentDeath = 0.f;
		return;
	}
	else if (!mIsEnd)
	{
		mCurrentDeath += 0.0075f * dt;
		if (mCurrentDeath > 12.f)
		{
			mCurrentDeath = 12.f;
			mIsEnd = true;
			mIsEnabling = true;
		}
		mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
		mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mCurrentDeath) < 5 ? static_cast<int>(mCurrentDeath) : static_cast<int>(mCurrentDeath) - 5),
			90 * (static_cast<int>(mCurrentDeath) < 5 ? 0 : 1), 25, 90));
		return;
	}

	if (mLife && (mHitpoints > 0))
	{
		if (mIsEnabled)
		{
			appear(dt);
		}

		if (mIsDisabled)
		{
			disappear(dt);
		}

		if (!mIsEnabled && !mIsDisabled && mIsEnabling)
		{
			mCurrentFrame += 0.0075f * dt;
			if (mCurrentFrame > 8.f)
			{
				mCurrentFrame -= 8.f;
			}
			checkCollisionWithMap(dx, 0.f);

			if (mIsAttacked)
			{
				mCurrentAttack += 0.005f * dt;
				if (mCurrentAttack > 2.f)
				{
					mCurrentAttack -= 1.f;
					mIsAttacked = false;
					mIsHittedOnce = false;
				}
				else if (static_cast<int>(mCurrentAttack) == 1 && !mIsHittedOnce)
				{
					mIsHitted = true;
					mIsHittedOnce = true;
				}
				else
				{
					mIsHitted = false;
				}
			}
			else
			{
				mCurrentAttack = 0.f;
			}
			mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
			mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mCurrentFrame) % 8), 90 * 2, 25, 90));
			mCurrentDeath = 0.f;
		}
	}

	if (mHitpoints <= 0)
	{
		mCurrentDeath += 0.0075f * dt;
		dx = 0.f;
		dy = 0.f;
		mIsTurned = false;
		if (mCurrentDeath >= 12.f)
		{
			mCurrentDeath = 12.f;
			mLife = false;
		}
		mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
		mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mCurrentDeath) < 8 ? 7 - static_cast<int>(mCurrentDeath) : 12 - static_cast<int>(mCurrentDeath)),
			90 * (static_cast<int>(mCurrentDeath) < 8 ? 1 : 0), 25, 90));
	}
}
