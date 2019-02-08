#include <iostream>

#include "player.hpp"


namespace
{
    const std::map<Type::ID, EntityData> PLAYER_TABLE = initializeHeroData();
}

Player::Player(const Type::ID id, const TextureHolder& textures, const FontHolder&, 
               const level& lvl, const float X, const  float Y, const int width, const int height, 
               PlayerInfo& playerInfo)
: Entity(id, X, Y, width, height, PLAYER_TABLE.at(id).mSpeed, PLAYER_TABLE.at(id).mHitpoints,
         PLAYER_TABLE.at(id).mDamage)
, _beforeJump(0.f)
, _afterJump(0.f)
, _fallingHeight(0.f)
, _isLeft(false)
, _isRight(false)
, _needFirstMainDelay(false)
, _hadFirstMainDelay(false)
, _needFirstMiniDelay(false)
, _hadFirstMiniDelay(false)
, mStayTimer(0.f)
, mOnPlatform(0.f)
, mShootTimer(0.f)
, mJumpTimer(0.f)
, mDelayTimer(0.f)
, mTransitTimer(0.f)
, mCounter(0)
, mDialogNumber(0)
, mMaxHitpoints(PLAYER_TABLE.at(id).mHitpoints)
, mIsShoot(false)
, mCanShoted(true)
, mIsJumped(false)
, mShooted(false)
, mIsShooting(false)
, mDoubleJump(false)
, mCanDoubleJump(false)
, mPressJump(false)
, mIsRichedEnd(false)
, mHasStartedFirstMainBoss(false)
, mHasStartedFirstMiniBoss(false)
, mGotKey(false)
, mActivatedGate(false)
, mHasTeleported(false)
, mIsStartedDelay(false)
, mState(Player::State::Idle)
, mPlayerInfo(playerInfo)
{
    // Инициализируем и получаем все объекты для взаимодействия персонажа с картой
    mLevelObjects = lvl.get_all_objects();
    mTexture = textures.get(Textures::ID::Archer);
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(77, 1104, 100, 110));
    mSprite.setScale(0.33f, 0.33f);
}

void Player::doDelay(const float dt, bool& flag, bool& checker)
{
    mDelayTimer += dt;
    if (mDelayTimer > 1500.f)
    {
        flag = false;
        checker = true;
        mDelayTimer = 0.f;
        mIsStartedDelay = false;
    }
    else if (mDelayTimer > 250.f)
    {
        mIsStartedDelay = true;
    }
}


void Player::control(const float dt)
{
    if (!mCanShoted)
    {
        mShootTimer += dt;
        if (mShootTimer > 750.f)
        {
            mShootTimer = 0.f;
            mCanShoted = true;
        }
    }
    else if (sf::Keyboard::isKeyPressed(mPlayerInfo.getAssignedKey(PlayerInfo::Action::Fire)) &&
             mCanShoted)
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
        mJumpTimer += dt;
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

    if (sf::Keyboard::isKeyPressed(mPlayerInfo.getAssignedKey(PlayerInfo::Action::MoveUp)))
    {
        if (mOnGround)
        {
            //mState = State::Jump;
            dy = -0.35f;

            // Прыжок != на земле
            mOnGround = false;
            mCanDoubleJump = true;
            mPressJump = true;
        }
        else if (mDoubleJump)
        {
            //mState = State::Jump;
            dy = -0.425f;

            // Прыжок != на земле
            mOnGround = false;
            mDoubleJump = false;
        }
    }


    if (sf::Keyboard::isKeyPressed(mPlayerInfo.getAssignedKey(PlayerInfo::Action::MoveDown)))
    {
        dx = 0.f;
        //mState = State::Down;
        //mSpeed = 0.1f;
    }

    if (sf::Keyboard::isKeyPressed(mPlayerInfo.getAssignedKey(PlayerInfo::Action::MoveLeft)))
    {
        mState = State::Left;
        mSpeed = 0.09f;
        _isLeft = true;
        mMoveTimer += dt;
    }
    else if (_isLeft)
    {
        _isLeft = false;
        if (mMoveTimer < 100.f)
        {
            mCurrentFrame = static_cast<int>(mCurrentFrame) + 0.5f;
        }
        mMoveTimer = 0.f;
    }

    if (sf::Keyboard::isKeyPressed(mPlayerInfo.getAssignedKey(PlayerInfo::Action::MoveRight)))
    {
        mState = State::Right;
        mSpeed = 0.09f;
        _isRight = true;
        mMoveTimer += dt;
    }
    else if (_isRight)
    {
        _isRight = false;
        if (mMoveTimer < 100.f)
        {
            mCurrentFrame = static_cast<int>(mCurrentFrame) + 0.5f;
        }
        mMoveTimer = 0.f;
    }

    if (sf::Keyboard::isKeyPressed(mPlayerInfo.getAssignedKey(PlayerInfo::Action::MoveRight)) &&
        sf::Keyboard::isKeyPressed(mPlayerInfo.getAssignedKey(PlayerInfo::Action::MoveLeft)))
    {
        mSpeed = 0.f;
    }

    if (sf::Keyboard::isKeyPressed(mPlayerInfo.getAssignedKey(PlayerInfo::Action::UseDoor)) &&
        mPlayerInfo.mCanTransit)
    {
        if (!mPlayerInfo.mDoTransit)
        {
            mPlayerInfo.mNeedTransit = true;
            mSpeed = 0.f;
            dy = 0.f;
        }
        else
        {
            mTransitTimer += dt;
        }

        if (mTransitTimer > 100.f)
        {
            mTransitTimer = 0.f;
            mPlayerInfo.mNeedTransit = true;
            mPlayerInfo.mDoTransit = false;
        }
    }
}

void Player::checkCollisionWithMap(const float Dx, const float Dy)
{
    mDialogNumber = 0;

    bool transitFound = false;
    for (const auto& object : mLevelObjects)
    {
        // Проверяем пересечение с объектом
        if (getRect().intersects(object.rect))
        {
            // Если встретили препятствие
            if (object.name == "solid" || object.name == "rock")
            {
                if (Dy > 0.f)
                {
                    y = object.rect.top - mHeight;
                    dy = 0.f;
                    mOnGround = true;
                    mDoubleJump = false;
                    mCanDoubleJump = false;
                    mPressJump = false;

                    if (_beforeJump != 0.f)
                    {
                        _afterJump = y;
                    }

                    _fallingHeight = _afterJump - _beforeJump;

                    if (_fallingHeight > 100.f)
                    {
                        mHitpoints -= static_cast<int>(0.45f * (_fallingHeight - 100.f));
                    }

                    _beforeJump = 0.f;
                    _afterJump = 0.f;
                }
                if (Dy < 0.f)
                {
                    y = object.rect.top + object.rect.height;
                    dy = 0.f;
                }
                if (Dx > 0.f)
                {
                    x = object.rect.left - mWidth;
                }
                if (Dx < 0.f)
                {
                    x = object.rect.left + object.rect.width;
                }
            }

            // Если встретили смерть
            if (object.name == "death")
            {
                mHitpoints = 0;
            }

            // Если встретили диалог
            if (object.name == "dialogMessage")
            {
                mDialogNumber = std::stoi(object.type);
                if (std::stoi(object.type) == 3 && mGotKey)
                {
                    mActivatedGate = true;
                    mDialogNumber = 5;
                }
                
                if (std::stoi(object.type) == 8 && !_hadFirstMiniDelay)
                {
                    mDialogNumber = 0;
                }
            }

            // Если встретили переход
            if (object.name == "door")
            {
                transitFound = true;
                mPlayerInfo.mCanTransit = true;
                mPlayerInfo.mNumberOfDoor = { object.id, object.type };
            }

            // Если встретили конец уровня
            if (object.name == "end")
            {
                mIsRichedEnd = true;
            }

            // Если встретили босса
            if (object.name == "boss")
            {
                switch (std::stoi(object.type))
                {
                    case 1:
                        mHasStartedFirstMainBoss = true;
                        mPlayerInfo.mQuests.at(PlayerInfo::Quest::FightShadow) = true;
                        _needFirstMainDelay = !_hadFirstMainDelay;
                        break;
                    case 2:
                        mHasStartedFirstMiniBoss = true;
                        mPlayerInfo.mQuests.at(PlayerInfo::Quest::FightGolemDark) = true;
                        _needFirstMiniDelay = !_hadFirstMiniDelay;
                        break;
                    default:
                        mHasStartedFirstMainBoss = false;
                        mHasStartedFirstMiniBoss = false;
                        break;
                }
            }
        }
        // Если мы не пересекаемся не с какими прямоугольниками и dy > 0, то onGround = false 
        else if (dy > 0.f)
        {
            mOnGround = false;

            if (_beforeJump == 0.f)
            {
                _beforeJump = y;
            }
        }
    }

    if (!transitFound)
    {
        mPlayerInfo.mCanTransit = false;
    }
}

void Player::update(const float dt)
{
    if (mPlayerInfo.mQuests.at(PlayerInfo::Quest::KillDwarvenCommanderM))
    {
        mGotKey = true;
    }

    if (mDialogNumber == 9 && !mHasTeleported)
    {
        x = 8304.f;
        y = 1168.f;
        mHasTeleported = true;
    }


    if (_needFirstMainDelay)
    {
        doDelay(dt, _needFirstMainDelay, _hadFirstMainDelay);
    }
    if (_needFirstMiniDelay)
    {
        doDelay(dt, _needFirstMiniDelay, _hadFirstMiniDelay);
    }

    switch (mTypeID)
    {
        case Type::ID::Archer:
            if (mLife && mHitpoints > 0 && !mIsRichedEnd && !mIsStartedDelay)
            {
                control(dt);
            }

            // В зависимости от направления
            switch (mState)
            {
                case Player::State::Left:
                    dx = -mSpeed;
                    break;
                case Player::State::Right:
                    dx = mSpeed;
                    break;
                case Player::State::Up:

                    break;
                case Player::State::Down:
                    dx = 0.f;
                    break;
                case Player::State::Jump:

                    break;
                case Player::State::Idle:

                    break;
                default:
                    mState = Player::State::Idle;
                    break;
            }

            if (mOnPlatform != 0.f)
            {
                if (dx > 0.f)
                {
                    x += dx * dt + (mOnPlatform > 0.f ? 0.f : -mOnPlatform) * dt;
                }
                else if (dx < 0.f)
                {
                    x += dx * dt + (mOnPlatform > 0.f ? -mOnPlatform : 0.f) * dt;
                }
            }
            else if (!mShooted)
            {
                x += dx * dt;
            }

            // Обрабатываем столкновения по x
            checkCollisionWithMap(dx, 0.f);
            y += dy * dt;
        
            // Обрабатываем столкновения по y
            checkCollisionWithMap(0.f, dy);

            // Обнуляем скорость, чтобы объект остановился
            if (!mIsMove)
            {
                mSpeed = 0.f;
            }
            mSprite.setPosition(x + mWidth / 2.f - 13.f, y + mHeight / 2.f - 10.f);

            if (mSpeed > 0.f)
            {
                mSpeed -= 0.0085f;
            }
            else if (mSpeed < 0.f)
            {
                mSpeed = 0.f;
            }

            if (_beforeJump == 0.f && dy > 0.f && !mOnGround)
            {
                _beforeJump = y;
            }

            // Притяжение к земле
            dy += 0.0015f * dt;


            if (-0.05f < dx && dx < 0.05f)
            {
                dx = 0.f;
            }

            if (mLife && mHitpoints > 0)
            {
                if (mOnGround)
                {
                    mCurrentFrame += 0.006f * dt;
                }

                if (mCurrentFrame > 4.f)
                {
                    mCurrentFrame -= 4.f;
                }

                if (mShooted)
                {
                    mCurrentAttack += 0.0035f * dt;
                    if (mCurrentAttack > 2.f)
                    {
                        mCurrentAttack = 0.f;
                        mShooted = false;
                        return;
                    }
                    else if (1.7f < mCurrentAttack && mCurrentAttack < 1.76f)
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
                        mSprite.setTextureRect(sf::IntRect(256 * (static_cast<int>(mCurrentAttack) + 4) + 77,
                                               (((mState == State::Right) || (mState == State::Idle)) ? 1104 : 77), 100, 110));
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
                            mSprite.setTextureRect(sf::IntRect(256 * 3 + 77,
                                                   (((mState == State::Right) || (mState == State::Idle)) ? 1104 : 77), 100, 110));
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
                        mSprite.setTextureRect(sf::IntRect(256 * 0 + 77,
                                               (((mState == State::Right) || (mState == State::Idle)) ? 1104 : 77), 100, 110));
                        mIsJumped = false;
                        mCurrentFrame = 0.f;
                    }
                    if (dx == 0.f)
                    {
                        mStayTimer += dt;
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
                mCurrentDeath += 0.003f * dt;
                //dx = 0.f;
                //dy = 0.f;

                if (mCurrentDeath > 2.f)
                {
                    mCurrentDeath = 2.f;
                    mDeathTimer += dt;
                    if (mCounter == 2)
                    {
                        mCounter = 0;
                        mCurrentDeath = 2.f;
                        mLife = false;
                    }
                    else if (mDeathTimer > 1000.f)
                    {
                        mDeathTimer = 0.f;
                        ++mCounter;
                    }
                }
                mSprite.setPosition(x + (mWidth / 2.f) - 14.f, y + (mHeight / 2.f) - 10.f);
                if (static_cast<int>(mCurrentDeath) == 0)
                {
                    mSprite.setTextureRect(sf::IntRect(256 * 0 + 77, (mState == State::Left ? 77 : 1104), 100, 110));
                }
                else if (static_cast<int>(mCurrentDeath) == 1)
                {
                    mSprite.setTextureRect(sf::IntRect(256 * 6 + 77, (mState == State::Left ? 77 : 1104), 100, 110));
                }
                else if (static_cast<int>(mCurrentDeath) == 2)
                {
                    mSprite.setTextureRect(sf::IntRect(256 * 7 + 77, (mState == State::Left ? 1869 : 848), 120, 110)); //77 - 1104
                }
            }

            break;

        default:
            std::cout << "Invalid hero type\n";
            break;
    }

    if (!mLife && mPlayerInfo.mCanRessurect)
    {
        mLife = true;
        mHitpoints = mMaxHitpoints;
        x = mPlayerInfo.mLastSavePoint.x;
        y = mPlayerInfo.mLastSavePoint.y;
        mCurrentDeath = 0.f;
        mPlayerInfo.mCanRessurect = false;
    }
}
