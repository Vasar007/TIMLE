#include "../Include/Shadow.hpp"


namespace
{
	const ShadowData STable = initializeShadowData();
}

Shadow::Shadow(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type)
: Entity(Id, X, Y, width, height, STable.speed, STable.hitpoints, STable.damage)
, mCounter(0)
, mNumTelPoint(0)
, mAppearing(0.f)
, mDisappearing(0.f)
, mStayTimer(0.f)
, mDelayTimer(0.f)
, mInaccuracy(0.f)
, mIsTeleported(false)
, mIsTeleporting(false)
, mIsDisappeared(false)
, mIsDisappearing(false)
, mIsStay(false)
, mIsNeedStay(false)
, mIsDelay(false)
, mIsNeedDelay(false)
, mIsCalling(false)
, mIsWithdrawing(false)
, mTeleporPoints(STable.teleportPoints)
{
	mTexture = textures.get(Textures::Shadow);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 280, mWidth, mHeight));
	dx = 0.03f;
	mSprite.setScale(0.5f, 0.5f);
	mSprite.scale(-1.f, 1.f);
}

void Shadow::appear(float time)
{
	mAppearing += 0.0075f * time;
	if (mAppearing >= 14.f)
	{
		mAppearing = 0.f;
		mIsTeleporting = false;
		mIsTeleported = true;
		mIsHittedOnce = false;
		return;
	}
	mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
	mSprite.setTextureRect(sf::IntRect(80 * (static_cast<int>(mAppearing) % 4), 70 * (static_cast<int>(mAppearing) / 4), 80, 70));
}

void Shadow::disappear(float time)
{
	mDisappearing += 0.0075f * time;
	if (mDisappearing >= 14.f)
	{
		mDisappearing = 0.f;
		mIsDisappeared = true;
		mIsDisappearing = false;
		return;
	}
	mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
	mSprite.setTextureRect(sf::IntRect(80 * ((13 - static_cast<int>(mDisappearing)) % 4), 70 * ((13 - static_cast<int>(mDisappearing)) / 4), 80, 70));
}

void Shadow::update(float time)
{
	if (!mIsStarted)
	{
		mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
		mSprite.setTextureRect(sf::IntRect(0, 0, 80, 70));
		mCurrentDeath = 0.f;
		return;
	}
	else if (!mIsEnd)
	{
		mCurrentDeath += 0.0075f * time;
		if (mCurrentDeath > 13.f)
		{
			mCurrentDeath = 13.f;
			mIsEnd = true;
			//mIsDelay = true;
		}
		mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
		mSprite.setTextureRect(sf::IntRect(80 * (static_cast<int>(mCurrentDeath) % 4), 70 * (static_cast<int>(mCurrentDeath) / 4), 80, 70));
		return;
	}

	mIsCalling = false;
	mIsWithdrawing = false;
	mMoveTimer += time;
	if (mMoveTimer > 1000.f)
	{
		mMoveTimer = 0.f;
		if (mHitpoints <= 0)
			mCounter++;
	}

	if (mLife && (mHitpoints > 0))
	{

		mCurrentFrame += 0.0075f * time;
		if (mCurrentFrame > 4.f)
		{
			mCurrentFrame -= 4.f;
		}

		if (mIsAttacked && (mNumTelPoint <= static_cast<int>(mTeleporPoints.size())) && !mIsDelay && !mIsStay)
		{
			if (!mIsDisappeared && !mIsDisappearing)
			{
				mIsDisappearing = true;
			}

			if (mIsDisappearing)
			{
				disappear(time);
			}
			else if (!mIsTeleported && !mIsTeleporting)
			{
				x = mTeleporPoints[mNumTelPoint].x;
				y = mTeleporPoints[mNumTelPoint].y;
				switch (mNumTelPoint)
				{
					case 0:
						if (dx > 0.f)
						{
							dx = -dx;
							mSprite.scale(-1.f, 1.f);
						}
						mIsCalling = true;
						mIsNeedDelay = true;
						break;
					case 1:
						if (dx < 0.f)
						{
							dx = -dx;
							mSprite.scale(-1.f, 1.f);
						}
						mIsNeedDelay = true;
						break;
					case 2:
						if (mIsBack)
						{
							dx = -dx;
							mSprite.scale(-1.f, 1.f);
						}
						mIsNeedDelay = true;
						break;
					case 3:
						if (mIsBack)
						{
							dx = -dx;
							mSprite.scale(-1.f, 1.f);
						}
						mIsWithdrawing = true;
						mIsNeedStay = true;
						break;
					default:
						dx = 0.03f;
						break;
				}

				mNumTelPoint++;
				mIsTeleporting = true;
			}

			if (mIsTeleporting)
			{
				appear(time);
			}

			if (mIsTeleported && !mIsHittedOnce)
			{
				mIsHitted = true;
				mIsHittedOnce = true;
				mIsAttacked = false;

				mIsTeleported = false;
				mIsTeleporting = false;
				
				mIsDisappeared = false;
				mIsDisappearing = false;

				if (mIsNeedDelay)
				{
					mIsNeedDelay = false;
					mIsDelay = true;
				}

				if (mIsNeedStay)
				{
					mIsNeedStay = false;
					mIsStay = true;
				}
				mCurrentFrame = 0.f;
			}
			else {
				mIsHitted = false;
			}
		}
		else
		{
			if (mIsDelay)
			{
				mDelayTimer += 0.0025f * time;
			}

			if (mIsStay)
			{
				mStayTimer += 0.0005f * time;
				mIsHitted = false;
			}

			if (mDelayTimer > 2.f)
			{
				mDelayTimer = 0.f;
				mIsDelay = false;
			}

			if (mStayTimer > 2.f)
			{
				mStayTimer = 0.f;
				mIsStay = false;
				mNumTelPoint = 0;
			}

			mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
			mSprite.setTextureRect(sf::IntRect(80 * static_cast<int>(mCurrentFrame), 280, 80, 70));
		}
		mCurrentDeath = 14.f;
		mInaccuracy = dx;
	}

	if (mHitpoints <= 0)
	{
		mCurrentDeath -= 0.0075f * time;
		dx = 0.f;
		dy = 0.f;
		if (mCurrentDeath < 0.f)
		{
			mCurrentDeath = 0.f;
			if (mCounter == 6)
				mLife = false;
		}
		mSprite.setPosition(x + (mWidth / 2.f) + (mInaccuracy > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
		mSprite.setTextureRect(sf::IntRect(80 * (static_cast<int>(mCurrentDeath) % 4), 70 * (static_cast<int>(mCurrentDeath) / 4), 80, 70));
	}
}
