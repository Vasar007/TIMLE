#include "shadow.hpp"


namespace
{
    const ShadowData SHADOW_TABLE = initializeShadowData();
}

Shadow::Shadow(const Type::ID id, const TextureHolder& textures, const FontHolder&,
              const level&, const float x_coord, const float y_coord, const int width,
              const int height, const std::string&)
: Entity(id, x_coord, y_coord, width, height, SHADOW_TABLE.mSpeed, SHADOW_TABLE.mHitpoints, 
         SHADOW_TABLE.mDamage)
, _counter(0)
, _teleportation_point_number(0)
, _appearence_timer(0.f)
, _disappearance_timer(0.f)
, _stay_timer(0.f)
, _delay_timer(0.f)
, _inaccuracy(0.f)
, _teleported(false)
, _teleporting(false)
, _is_isappeared(false)
, _disappearing(false)
, _needs_idle(false)
, _needs_delay(false)
, _telepor_points(SHADOW_TABLE.mTeleportPoints)
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
    _appearence_timer += 0.0075f * dt;
    if (_appearence_timer >= 14.f)
    {
        _appearence_timer = 0.f;
        _teleporting = false;
        _teleported = true;
        mIsHittedOnce = false;
        return;
    }
    mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
    mSprite.setTextureRect(sf::IntRect(80 * (static_cast<int>(_appearence_timer) % 4), 70 * (static_cast<int>(_appearence_timer) / 4), 80, 70));
}

void Shadow::disappear(const float dt)
{
    _disappearance_timer += 0.0075f * dt;
    if (_disappearance_timer >= 14.f)
    {
        _disappearance_timer = 0.f;
        _is_isappeared = true;
        _disappearing = false;
        return;
    }
    mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
    mSprite.setTextureRect(sf::IntRect(80 * ((13 - static_cast<int>(_disappearance_timer)) % 4), 70 * ((13 - static_cast<int>(_disappearance_timer)) / 4), 80, 70));
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

        if (mIsAttacked && (_teleportation_point_number <= static_cast<int>(_telepor_points.size())) && !is_delay && !idling)
        {
            if (!_is_isappeared && !_disappearing)
            {
                _disappearing = true;
            }

            if (_disappearing)
            {
                disappear(dt);
            }
            else if (!_teleported && !_teleporting)
            {
                x = _telepor_points[_teleportation_point_number].x;
                y = _telepor_points[_teleportation_point_number].y;
                switch (_teleportation_point_number)
                {
                    case 0:
                        if (dx > 0.f)
                        {
                            dx = -dx;
                            mSprite.scale(-1.f, 1.f);
                        }
                        summoning = true;
                        _needs_delay = true;
                        break;
                    case 1:
                        if (dx < 0.f)
                        {
                            dx = -dx;
                            mSprite.scale(-1.f, 1.f);
                        }
                        _needs_delay = true;
                        break;
                    case 2:
                        if (mIsBack)
                        {
                            dx = -dx;
                            mSprite.scale(-1.f, 1.f);
                        }
                        _needs_delay = true;
                        break;
                    case 3:
                        if (mIsBack)
                        {
                            dx = -dx;
                            mSprite.scale(-1.f, 1.f);
                        }
                        calling_of = true;
                        _needs_idle = true;
                        break;
                    default:
                        dx = 0.03f;
                        break;
                }

                ++_teleportation_point_number;
                _teleporting = true;
            }

            if (_teleporting)
            {
                appear(dt);
            }

            if (_teleported && !mIsHittedOnce)
            {
                mIsHitted = true;
                mIsHittedOnce = true;
                mIsAttacked = false;

                _teleported = false;
                _teleporting = false;
                
                _is_isappeared = false;
                _disappearing = false;

                if (_needs_delay)
                {
                    _needs_delay = false;
                    is_delay = true;
                }

                if (_needs_idle)
                {
                    _needs_idle = false;
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
                _delay_timer += 0.0025f * dt;
            }

            if (idling)
            {
                _stay_timer += 0.0005f * dt;
                mIsHitted = false;
            }

            if (_delay_timer > 2.f)
            {
                _delay_timer = 0.f;
                is_delay = false;
            }

            if (_stay_timer > 2.f)
            {
                _stay_timer = 0.f;
                idling = false;
                _teleportation_point_number = 0;
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
