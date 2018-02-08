#include "../Include/Shadow.hpp"


namespace
{
	const ShadowData SHADOW_TABLE = initializeShadowData();
}

Shadow::Shadow(const Type::ID id, const TextureHolder& textures, const FontHolder&,
			  const Level&, const float X, const float Y, const int width, const int height, 
			  const std::string&)
: Entity(id, X, Y, width, height, SHADOW_TABLE.mSpeed, SHADOW_TABLE.mHitpoints, 
		 SHADOW_TABLE.mDamage)
, _counter(0)
, _numTelPoint(0)
, _appearing(0.f)
, _disappearing(0.f)
, _stayTimer(0.f)
, mDelayTimer(0.f)
, _inaccuracy(0.f)
, _isTeleported(false)
, _isTeleporting(false)
, _isDisappeared(false)
, _isDisappearing(false)
, _isNeedStay(false)
, _isNeedDelay(false)
, _teleporPoints(SHADOW_TABLE.mTeleportPoints)
, mIsStay(false)
, mIsDelay(false)
, mIsCalling(false)
, mIsWithdrawing(false)
{
	mTexture = textures.get(Textures::ID::Shadow);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 280, 80, 70));
	dx = 0.03f;
	mSprite.setScale(0.5f, 0.5f);
	mSprite.scale(-1.f, 1.f);
}

void Shadow::appear(const float dt)
{
	_appearing += 0.0075f * dt;
	if (_appearing >= 14.f)
	{
		_appearing = 0.f;
		_isTeleporting = false;
		_isTeleported = true;
		mIsHittedOnce = false;
		return;
	}
	mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
	mSprite.setTextureRect(sf::IntRect(80 * (static_cast<int>(_appearing) % 4), 70 * (static_cast<int>(_appearing) / 4), 80, 70));
}

void Shadow::disappear(const float dt)
{
	_disappearing += 0.0075f * dt;
	if (_disappearing >= 14.f)
	{
		_disappearing = 0.f;
		_isDisappeared = true;
		_isDisappearing = false;
		return;
	}
	mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
	mSprite.setTextureRect(sf::IntRect(80 * ((13 - static_cast<int>(_disappearing)) % 4), 70 * ((13 - static_cast<int>(_disappearing)) / 4), 80, 70));
}

void Shadow::update(const float dt)
{
	if (!mIsStarted)
	{
		mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
		mSprite.setTextureRect(sf::IntRect(0, 0, 80, 70));
		mCurrentDeath = 0.f;
		return;
	}
	if (!mIsEnd)
	{
		mCurrentDeath += 0.0075f * dt;
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
	mMoveTimer += dt;
	if (mMoveTimer > 1000.f)
	{
		mMoveTimer = 0.f;
		if (mHitpoints <= 0)
		{
			++_counter;
		}
	}

	if (mLife && (mHitpoints > 0))
	{

		mCurrentFrame += 0.0075f * dt;
		if (mCurrentFrame > 4.f)
		{
			mCurrentFrame -= 4.f;
		}

		if (mIsAttacked && (_numTelPoint <= static_cast<int>(_teleporPoints.size())) && !mIsDelay && !mIsStay)
		{
			if (!_isDisappeared && !_isDisappearing)
			{
				_isDisappearing = true;
			}

			if (_isDisappearing)
			{
				disappear(dt);
			}
			else if (!_isTeleported && !_isTeleporting)
			{
				x = _teleporPoints[_numTelPoint].x;
				y = _teleporPoints[_numTelPoint].y;
				switch (_numTelPoint)
				{
					case 0:
						if (dx > 0.f)
						{
							dx = -dx;
							mSprite.scale(-1.f, 1.f);
						}
						mIsCalling = true;
						_isNeedDelay = true;
						break;
					case 1:
						if (dx < 0.f)
						{
							dx = -dx;
							mSprite.scale(-1.f, 1.f);
						}
						_isNeedDelay = true;
						break;
					case 2:
						if (mIsBack)
						{
							dx = -dx;
							mSprite.scale(-1.f, 1.f);
						}
						_isNeedDelay = true;
						break;
					case 3:
						if (mIsBack)
						{
							dx = -dx;
							mSprite.scale(-1.f, 1.f);
						}
						mIsWithdrawing = true;
						_isNeedStay = true;
						break;
					default:
						dx = 0.03f;
						break;
				}

				++_numTelPoint;
				_isTeleporting = true;
			}

			if (_isTeleporting)
			{
				appear(dt);
			}

			if (_isTeleported && !mIsHittedOnce)
			{
				mIsHitted = true;
				mIsHittedOnce = true;
				mIsAttacked = false;

				_isTeleported = false;
				_isTeleporting = false;
				
				_isDisappeared = false;
				_isDisappearing = false;

				if (_isNeedDelay)
				{
					_isNeedDelay = false;
					mIsDelay = true;
				}

				if (_isNeedStay)
				{
					_isNeedStay = false;
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
				mDelayTimer += 0.0025f * dt;
			}

			if (mIsStay)
			{
				_stayTimer += 0.0005f * dt;
				mIsHitted = false;
			}

			if (mDelayTimer > 2.f)
			{
				mDelayTimer = 0.f;
				mIsDelay = false;
			}

			if (_stayTimer > 2.f)
			{
				_stayTimer = 0.f;
				mIsStay = false;
				_numTelPoint = 0;
			}

			mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
			mSprite.setTextureRect(sf::IntRect(80 * static_cast<int>(mCurrentFrame), 280, 80, 70));
		}
		mCurrentDeath = 14.f;
		_inaccuracy = dx;
	}

	if (mHitpoints <= 0)
	{
		mCurrentDeath -= 0.0075f * dt;
		dx = 0.f;
		dy = 0.f;
		if (mCurrentDeath < 0.f)
		{
			mCurrentDeath = 0.f;
			if (_counter == 6)
			{
				mLife = false;
			}
		}
		mSprite.setPosition(x + (mWidth / 2.f) + (_inaccuracy > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
		mSprite.setTextureRect(sf::IntRect(80 * (static_cast<int>(mCurrentDeath) % 4), 70 * (static_cast<int>(mCurrentDeath) / 4), 80, 70));
	}
}
