#include "../Include/Shadow.hpp"


namespace
{
    const ShadowData SHADOW_TABLE = initializeShadowData();
}

Shadow::Shadow(const Type::ID id, const TextureHolder& textures, const FontHolder&,
              const Level&, const float x_coord, const float y_coord, const int width,
              const int height, const std::string&)
: Entity(id, x_coord, y_coord, width, height, SHADOW_TABLE.mSpeed, SHADOW_TABLE.mHitpoints, 
         SHADOW_TABLE.mDamage)
, counter_(0)
, teleportation_point_number_(0)
, appearence_timer_(0.f)
, disappearance_timer_(0.f)
, stay_timer_(0.f)
, delay_timer_(0.f)
, inaccuracy_(0.f)
, teleported_(false)
, teleporting_(false)
, isDisappeared_(false)
, disappearing_(false)
, needs_idle_(false)
, needs_delay_(false)
, telepor_points_(SHADOW_TABLE.mTeleportPoints)
, idling(false)
, is_delay(false)
, summoning(false)
, calling_of(false)
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
    appearence_timer_ += 0.0075f * dt;
    if (appearence_timer_ >= 14.f)
    {
        appearence_timer_ = 0.f;
        teleporting_ = false;
        teleported_ = true;
        mIsHittedOnce = false;
        return;
    }
    mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
    mSprite.setTextureRect(sf::IntRect(80 * (static_cast<int>(appearence_timer_) % 4), 70 * (static_cast<int>(appearence_timer_) / 4), 80, 70));
}

void Shadow::disappear(const float dt)
{
    disappearance_timer_ += 0.0075f * dt;
    if (disappearance_timer_ >= 14.f)
    {
        disappearance_timer_ = 0.f;
        isDisappeared_ = true;
        disappearing_ = false;
        return;
    }
    mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
    mSprite.setTextureRect(sf::IntRect(80 * ((13 - static_cast<int>(disappearance_timer_)) % 4), 70 * ((13 - static_cast<int>(disappearance_timer_)) / 4), 80, 70));
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

    summoning = false;
    calling_of = false;
    mMoveTimer += dt;
    if (mMoveTimer > 1000.f)
    {
        mMoveTimer = 0.f;
        if (mHitpoints <= 0)
        {
            ++counter_;
        }
    }

    if (mLife && (mHitpoints > 0))
    {

        mCurrentFrame += 0.0075f * dt;
        if (mCurrentFrame > 4.f)
        {
            mCurrentFrame -= 4.f;
        }

        if (mIsAttacked && (teleportation_point_number_ <= static_cast<int>(telepor_points_.size())) && !is_delay && !idling)
        {
            if (!isDisappeared_ && !disappearing_)
            {
                disappearing_ = true;
            }

            if (disappearing_)
            {
                disappear(dt);
            }
            else if (!teleported_ && !teleporting_)
            {
                x = telepor_points_[teleportation_point_number_].x;
                y = telepor_points_[teleportation_point_number_].y;
                switch (teleportation_point_number_)
                {
                    case 0:
                        if (dx > 0.f)
                        {
                            dx = -dx;
                            mSprite.scale(-1.f, 1.f);
                        }
                        summoning = true;
                        needs_delay_ = true;
                        break;
                    case 1:
                        if (dx < 0.f)
                        {
                            dx = -dx;
                            mSprite.scale(-1.f, 1.f);
                        }
                        needs_delay_ = true;
                        break;
                    case 2:
                        if (mIsBack)
                        {
                            dx = -dx;
                            mSprite.scale(-1.f, 1.f);
                        }
                        needs_delay_ = true;
                        break;
                    case 3:
                        if (mIsBack)
                        {
                            dx = -dx;
                            mSprite.scale(-1.f, 1.f);
                        }
                        calling_of = true;
                        needs_idle_ = true;
                        break;
                    default:
                        dx = 0.03f;
                        break;
                }

                ++teleportation_point_number_;
                teleporting_ = true;
            }

            if (teleporting_)
            {
                appear(dt);
            }

            if (teleported_ && !mIsHittedOnce)
            {
                mIsHitted = true;
                mIsHittedOnce = true;
                mIsAttacked = false;

                teleported_ = false;
                teleporting_ = false;
                
                isDisappeared_ = false;
                disappearing_ = false;

                if (needs_delay_)
                {
                    needs_delay_ = false;
                    is_delay = true;
                }

                if (needs_idle_)
                {
                    needs_idle_ = false;
                    idling = true;
                }
                mCurrentFrame = 0.f;
            }
            else {
                mIsHitted = false;
            }
        }
        else
        {
            if (is_delay)
            {
                delay_timer_ += 0.0025f * dt;
            }

            if (idling)
            {
                stay_timer_ += 0.0005f * dt;
                mIsHitted = false;
            }

            if (delay_timer_ > 2.f)
            {
                delay_timer_ = 0.f;
                is_delay = false;
            }

            if (stay_timer_ > 2.f)
            {
                stay_timer_ = 0.f;
                idling = false;
                teleportation_point_number_ = 0;
            }

            mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
            mSprite.setTextureRect(sf::IntRect(80 * static_cast<int>(mCurrentFrame), 280, 80, 70));
        }
        mCurrentDeath = 14.f;
        inaccuracy_ = dx;
    }

    if (mHitpoints <= 0)
    {
        mCurrentDeath -= 0.0075f * dt;
        dx = 0.f;
        dy = 0.f;
        if (mCurrentDeath < 0.f)
        {
            mCurrentDeath = 0.f;
            if (counter_ == 6)
            {
                mLife = false;
            }
        }
        mSprite.setPosition(x + (mWidth / 2.f) + (inaccuracy_ > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
        mSprite.setTextureRect(sf::IntRect(80 * (static_cast<int>(mCurrentDeath) % 4), 70 * (static_cast<int>(mCurrentDeath) / 4), 80, 70));
    }
}
