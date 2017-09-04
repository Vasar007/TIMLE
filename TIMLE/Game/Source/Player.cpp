#include "../Include/Player.hpp"


namespace
{
	const std::vector<EntityData> Table = initializeHeroData();
}

Player::Player(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, PlayerInfo* playerInfo)
: Entity(Id, X, Y, width, height, Table[Id].speed, Table[Id].hitpoints, Table[Id].damage)
, mStayTimer(0.f)
, mOnPlatform(0.f)
, mShootTimer(0.f)
, mJumpTimer(0.f)
, mBeforeJump(0.f)
, mAfterJump(0.f)
, mFallingHeight(0.f)
, mCounter(0)
, mDialogNumber(0)
, mIsShoot(false)
, mCanShoted(true)
, mIsJumped(false)
, mIsLeft(false)
, mIsRight(false)
, mShooted(false)
, mIsShooting(false)
, mDoubleJump(false)
, mCanDoubleJump(false)
, mPressJump(false)
, mIsRichedEnd(false)
, mHasStartedFirstBoss(false)
, mHasStartedSecondBoss(false)
, mGotKey(false)
, mActivatedGate(false)
, mHasTeleported(false)
, mState(Stay)
, mPlayerInfo(playerInfo)
{
	// Инициализируем и получаем все объекты для взаимодействия персонажа с картой
	mLevelObjects = lvl.getAllObjects();
	mTexture = textures.get(Textures::Archer);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(77, 1104, 100, 110));
	mSprite.setScale(0.33f, 0.33f);
}

void Player::control(float time)
{
	if (!mCanShoted)
	{
		mShootTimer += time;
		if (mShootTimer > 750.f)
		{
			mShootTimer = 0.f;
			mCanShoted = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::Fire)) && mCanShoted)
	{
		mIsShoot = true;
		mCanShoted = false;
	}
	else
	{
		mShootTimer = 0.f;
	}

	if (mCanDoubleJump)
	{
		mJumpTimer += time;
		if (mJumpTimer > 333.f)
		{
			mJumpTimer = 0.f;
			mDoubleJump = true;
			mCanDoubleJump = false;
		}
	}
	else if (!mOnGround && !mPressJump)
	{
		mCanDoubleJump = true;
		mPressJump = true;
	}
	else
	{
		mJumpTimer = 0.f;
	}

	if (sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::MoveUp)))
	{
		if (mOnGround)
		{
			//mState = jump;
			dy = -0.35f;

			// Прыжок != на земле
			mOnGround = false;
			mCanDoubleJump = true;
			mPressJump = true;
		}
		else if (mDoubleJump)
		{
			//mState = jump;
			dy = -0.5f;

			// Прыжок != на земле
			mOnGround = false;
			mDoubleJump = false;
		}
	}


	if (sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::MoveDown)))
	{
		//mState = down;
		//mSpeed = 0.1;
	}

	if (sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::MoveLeft)))
	{
		mState = Left;
		mSpeed = 0.09f;
		mIsLeft = true;
		mMoveTimer += time;
	}
	else if (mIsLeft)
	{
		mIsLeft = false;
		if (mMoveTimer < 100.f)
		{
			mCurrentFrame = static_cast<int>(mCurrentFrame) + 0.5f;
		}
		mMoveTimer = 0.f;
	}

	if (sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::MoveRight)))
	{
		mState = Right;
		mSpeed = 0.09f;
		mIsRight = true;
		mMoveTimer += time;
	}
	else if (mIsRight)
	{
		mIsRight = false;
		if (mMoveTimer < 100.f)
		{
			mCurrentFrame = static_cast<int>(mCurrentFrame) + 0.5f;
		}
		mMoveTimer = 0.f;
	}

	if (sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::MoveRight)) &&
		sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::MoveLeft)))
	{
		mSpeed = 0.f;
	}
}

void Player::checkCollisionWithMap(float Dx, float Dy)
{
	mDialogNumber = 0;

	for (size_t i = 0; i < mLevelObjects.size(); i++)
		// Проверяем пересечение с объектом
		if (getRect().intersects(mLevelObjects[i].mRect))
		{
			// Если встретили препятствие
			if (mLevelObjects[i].mName == "solid" || mLevelObjects[i].mName == "rock")
			{	
				if (Dy > 0.f)
				{
					y = mLevelObjects[i].mRect.top - mHeight;
					dy = 0.f;
					mOnGround = true;
					mDoubleJump = false;
					mCanDoubleJump = false;
					mPressJump = false;

					if (mBeforeJump != 0.f)
						mAfterJump = y;

					mFallingHeight = mAfterJump - mBeforeJump;

					if (mFallingHeight > 100.f)
						mHitpoints -=  static_cast<int>(0.4f * (mFallingHeight - 100.f));

					mBeforeJump = 0.f;
					mAfterJump = 0.f;
				}
				if (Dy < 0.f)
				{
					y = mLevelObjects[i].mRect.top + mLevelObjects[i].mRect.height;
					dy = 0.f;
				}
				if (Dx > 0.f)
				{
					x = mLevelObjects[i].mRect.left - mWidth;
				}
				if (Dx < 0.f)
				{
					x = mLevelObjects[i].mRect.left + mLevelObjects[i].mRect.width;
				}
			}

			// Если встретили смерть
			if (mLevelObjects[i].mName == "death")
			{	
				mHitpoints = 0;
			}

			// Если встретили диалог
			if (mLevelObjects[i].mName == "dialogMessage")
			{
				mDialogNumber = std::stoi(mLevelObjects[i].mType);
				if (std::stoi(mLevelObjects[i].mType) == 3 && mGotKey)
				{
					mActivatedGate = true;
					mDialogNumber = 5;
				}
			}

			// Если встретили конец уровня
			if (mLevelObjects[i].mName == "end")
			{
				mIsRichedEnd = true;
			}

			// Если встретили босса
			if (mLevelObjects[i].mName == "boss")
			{
				switch (std::stoi(mLevelObjects[i].mType))
				{
					case 1:
						mHasStartedFirstBoss = true;
						break;
					case 2:
						mHasStartedSecondBoss = true;
						break;
					case 3:
						break;
					default:
						mHasStartedFirstBoss = false;
						mHasStartedSecondBoss = false;
						break;
				}
			}
		}
		// Если мы не пересекаемся не с какими прямоугольниками и dy > 0, то onGround = false 
		else if (dy > 0.f)
		{
			mOnGround = false;

			if (mBeforeJump == 0.f)
				mBeforeJump = y;
		}
}

void Player::update(float time)
{
	if (mPlayerInfo->mQuests[1])
		mGotKey = true;

	if ((mDialogNumber == 9) && !mHasTeleported)
	{
		x = 8304;
		y = 1168;
		mHasTeleported = true;
	}

	switch (mTypeID)
	{
		case Type::ID::Archer:
			if (mLife && (mHitpoints > 0) && !mIsRichedEnd)
			{
				control(time);
			}

			// В зависимости от направления
			switch (mState)
			{
				case Left:
					dx = -mSpeed;
					break;
				case Right:
					dx = mSpeed;
					break;
				case Up:

					break;
				case Down:
					dx = 0.f;
					break;
				case Jump:

					break;
				case Stay:

					break;
				default:
					mState = Stay;
					break;
			}

			if (mOnPlatform != 0.f)
			{
				if (dx > 0.f)
					x += dx * time + (mOnPlatform > 0.f ? 0.f : -mOnPlatform) * time;
				else if (dx < 0.f)
					x += dx * time + (mOnPlatform > 0.f ? -mOnPlatform : 0.f) * time;
			}
			else if (!mShooted)
			{
				x += dx * time;
			}

			// Обрабатываем столкновения по x
			checkCollisionWithMap(dx, 0.f);
			y += dy * time;
		
			// Обрабатываем столкновения по y
			checkCollisionWithMap(0.f, dy);

			// Обнуляем скорость, чтобы объект остановился
			if (!mIsMove)
				mSpeed = 0.f;
			mSprite.setPosition(x + (mWidth / 2.f) - 13.f, y + (mHeight / 2.f) - 10.f);

			if (mSpeed > 0.f)
				mSpeed -= 0.0085f;
			else if (mSpeed < 0.f)
				mSpeed = 0.f;

			if (mBeforeJump == 0.f && dy > 0.f && !mOnGround)
				mBeforeJump = y;

			// Притяжение к земле
			dy += 0.0015f * time;


			if ((dx < 0.05f) && (dx > -0.05f))
			{
				dx = 0.f;
			}

			if (mLife && (mHitpoints > 0))
			{
				if (mOnGround)
					mCurrentFrame += 0.006f * time;

				if (mCurrentFrame > 4.f)
					mCurrentFrame -= 4.f;

				if (mShooted)
				{
					mCurrentAttack += 0.0035f * time;
					if (mCurrentAttack > 2.f)
					{
						mCurrentAttack = 0.f;
						mShooted = false;
						return;
					}
					else if ((mCurrentAttack > 1.7f) && (mCurrentAttack < 1.76f))
					{
						mIsShooting = true;
					}

					if (dx > 0.f)
					{
						mSprite.setTextureRect(sf::IntRect(256 * (static_cast<int>(mCurrentAttack) + 4) + 77, 1104, 100, 110));
					}
					else if (dx < 0.f)
					{
						mSprite.setTextureRect(sf::IntRect(256 * (static_cast<int>(mCurrentAttack) + 4) + 77, 77, 100, 110));
					}
					else if (dx == 0.f)
					{
						mSprite.setTextureRect(sf::IntRect(256 * (static_cast<int>(mCurrentAttack) + 4) + 77, (((mState == Right) || (mState == Stay)) ? 1104 : 77), 100, 110));
						mCurrentFrame = 0.f;
					}
				}
				else
				{
					if (!mOnGround)
					{
						mIsJumped = true;
						if (dx > 0.f)
						{
							mSprite.setTextureRect(sf::IntRect(256 * 3 + 77, 1104, 100, 110));
						}
						else if (dx == 0.f)
						{
							mSprite.setTextureRect(sf::IntRect(256 * 3 + 77, (((mState == Right) || (mState == Stay)) ? 1104 : 77), 100, 110));
						}
						else
						{
							mSprite.setTextureRect(sf::IntRect(256 * 3 + 77, 77, 100, 110));
						}
					}
					else if (dx > 0.f)
					{
						mSprite.setTextureRect(sf::IntRect(256 * static_cast<int>(mCurrentFrame) + 77, 1104, 100, 110));
					}
					else if (dx < 0.f)
					{
						mSprite.setTextureRect(sf::IntRect(256 * static_cast<int>(mCurrentFrame) + 77, 77, 100, 110));
					}
					else if ((dx == 0.f) && mIsJumped)
					{
						mSprite.setTextureRect(sf::IntRect(256 * 0 + 77, (((mState == Right) || (mState == Stay)) ? 1104 : 77), 100, 110));
						mIsJumped = false;
						mCurrentFrame = 0.f;
					}
					if (dx == 0.f)
					{
						mStayTimer += time;
						if (mStayTimer > 150.f)
						{
							mIsJumped = true;
							mStayTimer = 0.f;
						}
					}
					else
					{
						mStayTimer = 0.f;
					}
				}
			}

			if (mHitpoints <= 0)
			{
				mCurrentDeath += 0.003f * time;
				dx = 0.f;
				if (y > 2350.f)
					dy = 0.f;
				if (mCurrentDeath > 2.f)
				{
					mCurrentDeath = 2.f;
					mMoveTimer += time;
					if (mMoveTimer > 1000.f)
					{
						mMoveTimer = 0.f;
						mCounter++;
					}
					if (mCounter == 2)
						mLife = false;
				}
				mSprite.setPosition(x + (mWidth / 2.f) - 14.f, y + (mHeight / 2.f) - 10.f);
				if (static_cast<int>(mCurrentDeath) == 0)
				{
					mSprite.setTextureRect(sf::IntRect(256 * 0 + 77, (mState == Left ? 77 : 1104), 100, 110));
				}
				else if (static_cast<int>(mCurrentDeath) == 1)
				{
					mSprite.setTextureRect(sf::IntRect(256 * 6 + 77, (mState == Left ? 77 : 1104), 100, 110));
				}
				else if (static_cast<int>(mCurrentDeath) == 2)
				{
					mSprite.setTextureRect(sf::IntRect(256 * 7 + 77, (mState == Left ? 1869 : 848), 120, 110)); //77 - 1104
				}
			}

			break;

		default:
			std::cout << "Invalid hero type!" << std::endl;
			break;
	}
}