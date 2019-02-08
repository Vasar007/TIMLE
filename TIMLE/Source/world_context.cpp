#include <string>

#include "player.hpp"
#include "rock.hpp"
#include "utility.hpp"

#include "world_context.hpp"


WorldContext::WorldContext(TextureHolder& textures, FontHolder& fonts,
                           const std::size_t currentLevelNumber, level& lvl, const bool debug)
: _currentLevelNumber(currentLevelNumber)
, _level(lvl)
, _textures(textures)
, _fonts(fonts)
, _debug(debug)
{
}

void WorldContext::buildBossData()
{
    switch (_currentLevelNumber)
    {
        case 1:
        {
            // Create and initialize first boss Shadow.
            const auto shadowObj = _level.get_object("bossShadow");
            _shadowBoss.mShadow = std::make_unique<Shadow>(
                Type::ID::Shadow, _textures, _fonts, _level,
                shadowObj.rect.left, shadowObj.rect.top, 40, 35, shadowObj.type
            );
            _shadowBoss.mShadowLifeBar = std::make_unique<LifeBar>(
                Type::ID::ShadowBossBar, _textures, _fonts, _shadowBoss.mShadow->mHitpoints
            );

            for (std::size_t i = 0; i < _shadowBoss.mNumberOfTentacles; ++i)
            {
                _shadowBoss.mTentaclesStatic.emplace_back(Type::ID::Tentacle, _textures, _fonts,
                                                          _level, 12013.f + 13.f * i, 994.f, 13, 45, "0");
                _shadowBoss.mTentaclesStatic.emplace_back(Type::ID::Tentacle, _textures, _fonts,
                                                          _level, 12061.f + 13.f * i, 1073.f, 13, 45, "0");
                _shadowBoss.mTentaclesStatic.emplace_back(Type::ID::Tentacle, _textures, _fonts,
                                                          _level, 12285.f + 13.f * i, 1073.f, 13, 45, "0");
                _shadowBoss.mTentaclesStatic.emplace_back(Type::ID::Tentacle, _textures, _fonts,
                                                          _level, 12333.f + 13.f * i, 994.f, 13, 45, "0");
                _shadowBoss.mTentaclesStatic.emplace_back(Type::ID::Tentacle, _textures, _fonts,
                                                          _level, 12173.f + 13.f * i, 946.f, 13, 45, "0");
            }

            // Create and initialize first mini-boss GolemDark.
            const auto golemObj = _level.get_object("bossGolemDark");
            _golemBoss.mGolem = std::make_unique<GolemDark>(
                Type::ID::GolemDark, _textures, _fonts, _level,
                golemObj.rect.left, golemObj.rect.top, 70, 60, golemObj.type
            );
            _golemBoss.mGolemLifeBar = std::make_unique<LifeBar>(
                Type::ID::GolemDarkBossBar, _textures, _fonts, _golemBoss.mGolem->mHitpoints
            );

            _golemBoss.mRocks.emplace_back(Type::ID::Rock, 8192.f, 2272.f);
            _golemBoss.mRocks.emplace_back(Type::ID::Rock, 8272.f, 2240.f);
            _golemBoss.mRocks.emplace_back(Type::ID::Rock, 8336.f, 2192.f);
            _golemBoss.mRocks.emplace_back(Type::ID::Rock, 8384.f, 2160.f);
            _golemBoss.mRocks.emplace_back(Type::ID::Rock, 8464.f, 2160.f);
            _golemBoss.mRocks.emplace_back(Type::ID::Rock, 8560.f, 2144.f);
            _golemBoss.mRocks.emplace_back(Type::ID::Rock, 8656.f, 2144.f);
            _golemBoss.mRocks.emplace_back(Type::ID::Rock, 8736.f, 2160.f);
            break;
        }

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            break;

        default:
            std::cerr << "Error! Out of range level.\n";
            throw std::invalid_argument(std::to_string(_currentLevelNumber));
    }
}

void WorldContext::draw(sf::RenderTarget& target) const
{
    switch (_currentLevelNumber)
    {
        case 1:
        {
            if (_shadowBoss.mIsActive)
            {
                for (const auto& tentacle : _shadowBoss.mTentaclesStatic)
                {
                    if (_debug)
                    {
                        auto shape = utils::buildBorderLines(
                            tentacle.getRect(), sf::Color::Transparent, sf::Color::Red, 1.f
                        );
                        target.draw(shape);
                    }

                    tentacle.draw(target);
                }

                for (const auto& tentacle : _shadowBoss.mTentacles)
                {
                    if (_debug)
                    {
                        auto shape = utils::buildBorderLines(
                            tentacle.getRect(), sf::Color::Transparent, sf::Color::Red, 1.f
                        );
                        target.draw(shape);
                    }

                    tentacle.draw(target);
                }

                if (_debug)
                {
                    auto shape = utils::buildBorderLines(
                        _shadowBoss.mShadow->getRect(), sf::Color::Transparent, sf::Color::Red, 1.f
                    );
                    target.draw(shape);
                }

                _shadowBoss.mShadowLifeBar->draw(target);
                _shadowBoss.mShadow->draw(target);
            }
            if (_golemBoss.mGolem->mLife)
            {
                if (_golemBoss.mIsActive)
                {
                    if (_debug)
                    {
                        auto shape = utils::buildBorderLines(
                            _golemBoss.mGolem->getRect(), sf::Color::Transparent, sf::Color::Red, 1.f
                        );
                        target.draw(shape);
                    }

                    _golemBoss.mGolemLifeBar->draw(target);
                }
                _golemBoss.mGolem->draw(target);
            }
            break;
        }

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            break;

        default:
            std::cerr << "Error! Out of range level.\n";
            throw std::invalid_argument(std::to_string(_currentLevelNumber));
    }
}

void WorldContext::update(const sf::Time dt)
{
    switch (_currentLevelNumber)
    {
        case 1:
            // Updates ShadowBoss actions.
            if (_shadowBoss.mIsActive)
            {
                for (auto it = _shadowBoss.mTentaclesStatic.begin();
                    it != _shadowBoss.mTentaclesStatic.end();)
                {
                    it->update(static_cast<float>(dt.asMilliseconds()));
                    if (it->mLife)
                    {
                        ++it;
                    }
                    else
                    {
                        it = _shadowBoss.mTentaclesStatic.erase(it);
                    }
                }

                for (auto it = _shadowBoss.mTentacles.begin(); it != _shadowBoss.mTentacles.end();)
                {
                    it->update(static_cast<float>(dt.asMilliseconds()));
                    if (it->mLife)
                    {
                        ++it;
                    }
                    else
                    {
                        it = _shadowBoss.mTentacles.erase(it);
                    }
                }

                _shadowBoss.mShadowLifeBar->update(_shadowBoss.mShadow->mHitpoints);
                _shadowBoss.mShadow->update(static_cast<float>(dt.asMilliseconds()));
            }

            // Updates GolemDark actions.
            if (_golemBoss.mIsActive)
            {
                _golemBoss.mGolemLifeBar->update(_golemBoss.mGolem->mHitpoints);
                _golemBoss.mGolem->update(static_cast<float>(dt.asMilliseconds()));
            }
            break;

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            break;

        default:
            std::cerr << "Error! Out of range level.\n";
            throw std::invalid_argument(std::to_string(_currentLevelNumber));
    }
}

void WorldContext::processGolemDarkEvents(sf::View& view,
                                          std::vector<std::unique_ptr<Entity>>& entities)
{
    if (_golemBoss.mIsActive && _golemBoss.mIsShaked)
    {
        ++_golemBoss.mCameraCounter;
        switch (_golemBoss.mCameraCounter)
        {
            case 1:
                view.move(10, 10);
                break;
            
            case 2:
                view.move(-20, 0);
                break;
            
            case 3:
                view.move(0, -20);
                break;
            
            case 4:
                view.move(0, 20);
                break;
            
            default:
            {
                view.move(10, -10);
                _golemBoss.mCameraCounter = 0;
                _golemBoss.mIsShaked = false;

                const int randomNum = utils::randomInt(8);
                std::unique_ptr<Entity> rock = std::make_unique<Rock>(
                    Type::ID::Rock, _textures, _fonts, _level,
                    _golemBoss.mRocks.at(randomNum).x,
                    _golemBoss.mRocks.at(randomNum).y, 16, "3");
                rock->mIsAttacked = true;
                entities.emplace_back(std::move(rock));
                break;
            }
        }
    }
}

void WorldContext::processMainEvents(PlayerInfo& playerInfo, AudioManager& audioManager)
{
    auto playerHero = playerInfo.getPlayer();
    auto playerRect = playerHero->getRect();

    switch (_currentLevelNumber)
    {
        case 1:
        {
            /// If we activated first boss Shadow...
            if (playerHero->mHasStartedFirstMainBoss && !_shadowBoss.mIsFinished)
            {
                _shadowBoss.mIsActive = true;

                if (_shadowBoss.mShadow->mTypeID == Type::ID::Shadow)
                {
                    // If Shadow was activated first time...
                    if (!_shadowBoss.mShadow->mIsStarted)
                    {
                        _shadowBoss.mShadow->mIsStarted = true;
                        audioManager.setMusic(Music::ID::FirstBossMusic);
                    }
                    else if (playerHero->mHitpoints > 0)
                    {
                        if (!_shadowBoss.mShadow->mIsBack &&
                            ((playerHero->x - _shadowBoss.mShadow->x > 0.f &&
                                _shadowBoss.mShadow->dx < 0.f) ||
                                (playerHero->x - _shadowBoss.mShadow->x < 0.f &&
                                    _shadowBoss.mShadow->dx > 0.f)))
                        {
                            _shadowBoss.mShadow->mIsBack = true;
                            _shadowBoss.mShadow->dx = -_shadowBoss.mShadow->dx;
                            _shadowBoss.mShadow->mSprite.scale(-1.f, 1.f);
                        }
                        else
                        {
                            _shadowBoss.mShadow->mIsBack = false;
                        }
                        _shadowBoss.mShadow->mIsAttacked = true;
                        if (_shadowBoss.mShadow->mIsHitted && !_shadowBoss.mShadow->idling)
                        {
                            playerHero->mHitpoints -= _shadowBoss.mShadow->mDamage;
                            _shadowBoss.mTentacles.emplace_back(Type::ID::Tentacle, _textures,
                                                                _fonts, _level, playerHero->x - 50.f,
                                                                _shadowBoss.mShadow->y - 10.f, 13, 45, "1");
                            _shadowBoss.mTentacles.emplace_back(Type::ID::Tentacle, _textures,
                                                                _fonts, _level, playerHero->x - 25.f,
                                                                _shadowBoss.mShadow->y - 10.f, 13, 45, "1");
                            _shadowBoss.mTentacles.emplace_back(Type::ID::Tentacle, _textures,
                                                                _fonts, _level, playerHero->x,
                                                                _shadowBoss.mShadow->y - 10.f, 13, 45, "1");
                            _shadowBoss.mTentacles.emplace_back(Type::ID::Tentacle, _textures,
                                                                _fonts, _level, playerHero->x + 25.f,
                                                                _shadowBoss.mShadow->y - 10.f, 13, 45, "1");
                            _shadowBoss.mTentacles.emplace_back(Type::ID::Tentacle, _textures,
                                                                _fonts, _level, playerHero->x + 50.f,
                                                                _shadowBoss.mShadow->y - 10.f, 13, 45, "1");
                            _shadowBoss.mShadow->mIsHitted = false;
                            std::cout << "Shadow ATTACK\n";
                        }
                    }
                }

                if (_shadowBoss.mShadow->getRect().intersects(playerRect) &&
                    _shadowBoss.mShadow->is_delay)
                {
                    playerHero->mHitpoints = 0;
                }

                for (auto& tentacle : _shadowBoss.mTentaclesStatic)
                {
                    if (_shadowBoss.mShadow->summoning)
                    {
                        if (!tentacle.mIsStarted)
                        {
                            tentacle.mIsStarted = true;
                        }
                        else
                        {
                            tentacle.mIsEnabled = true;
                        }
                        tentacle.mIsEnabling = true;
                    }
                    else if (_shadowBoss.mShadow->calling_of)
                    {
                        tentacle.mIsDisabled = true;
                        tentacle.mIsEnabling = false;
                    }

                    if (_shadowBoss.mIsFinished)
                    {
                        tentacle.mHitpoints = 0;
                    }

                    if (tentacle.getRect().intersects(playerRect) && playerHero->mHitpoints > 0)
                    {
                        tentacle.mIsAttacked = true;
                        if (tentacle.mIsHitted)
                        {
                            playerHero->mHitpoints -= tentacle.mDamage;
                            tentacle.mIsHitted = false;
                            std::cout << "Hit\n";
                        }
                    }
                }

                for (auto& tentacle : _shadowBoss.mTentacles)
                {
                    if (_shadowBoss.mShadow->calling_of)
                    {
                        tentacle.mHitpoints = 0;
                    }
                    else
                    {
                        tentacle.mIsStarted = true;
                        tentacle.mIsEnabling = true;

                        if (tentacle.getRect().intersects(playerRect) && playerHero->mHitpoints > 0)
                        {
                            tentacle.mIsAttacked = true;
                            if (tentacle.mIsHitted)
                            {
                                playerHero->mHitpoints -= tentacle.mDamage;
                                tentacle.mIsHitted = false;
                                std::cout << "Hit\n";
                            }
                        }
                    }
                }
            }
            else if (_shadowBoss.mIsFinished && !_shadowBoss.mShadow->mLife && _shadowBoss.mIsActive)
            {
                _shadowBoss.mIsActive = false;
                playerInfo.mQuests.at(PlayerInfo::Quest::KillShadow) = true;
                audioManager.setMusic(Music::ID::FirstMainMusic);
            }


            /// If we activated firs mini-boss GolemDark...
            if (playerHero->mHasStartedFirstMiniBoss && !_golemBoss.mIsFinished)
            {
                _golemBoss.mIsActive = true;

                if (_golemBoss.mGolem->mTypeID == Type::ID::GolemDark)
                {
                    if (!_golemBoss.mIsWeakened &&
                        (playerInfo.mChosenSolution.at(PlayerInfo::Solution::InteractWithGolem) == 1))
                    {
                        _golemBoss.mGolem->mHitpoints -= 100;
                        _golemBoss.mIsWeakened = true;
                    }

                    // If GolemDark was activated first time.
                    if (!_golemBoss.mGolem->mIsStarted)
                    {
                        _golemBoss.mGolem->mIsStarted = true;
                        _golemBoss.mGolem->mCurrentDeath = 5.f;
                        audioManager.setMusic(Music::ID::FirstMiniBossMusic);
                    }
                    else if (_golemBoss.mGolem->getRect().intersects(playerRect))
                    {
                        if (playerHero->mHitpoints > 0)
                        {
                            if (!_golemBoss.mGolem->mIsBack &&
                                ((playerHero->x - _golemBoss.mGolem->x > 0.f &&
                                (_golemBoss.mGolem->dx < 0.f)) ||
                                    ((playerHero->x - _golemBoss.mGolem->x) < 0.f &&
                                        _golemBoss.mGolem->dx > 0.f)))
                            {
                                _golemBoss.mGolem->mIsBack = true;
                                _golemBoss.mGolem->dx = -_golemBoss.mGolem->dx;
                            }
                            _golemBoss.mGolem->mIsAttacked = true;
                            _golemBoss.mGolem->mCurrentFrame = 0.f;
                            if (_golemBoss.mGolem->mIsHitted)
                            {
                                playerHero->mHitpoints -= _golemBoss.mGolem->mDamage;
                                _golemBoss.mGolem->mIsHitted = false;
                                _golemBoss.mIsShaked = true;
                                std::cout << "GolemDark ATTACK\n";
                            }
                        }
                    }

                    if (_golemBoss.mGolem->mIsHitted)
                    {
                        _golemBoss.mIsShaked = true;
                    }
                }
            }
            else if (_golemBoss.mIsFinished && !_golemBoss.mGolem->mLife && _golemBoss.mIsActive)
            {
                _golemBoss.mIsActive = false;
                playerInfo.mQuests.at(PlayerInfo::Quest::KillGolemDark) = true;

                // Temporary object.
                object tempObject;

                tempObject.name = "dialogMessage";
                tempObject.type = "9";

                sf::FloatRect objectRect;
                objectRect.top = 2388;
                objectRect.left = 8352;
                objectRect.height = 60;
                objectRect.width = 80;
                tempObject.rect = std::move(objectRect);
                _level.objects.push_back(tempObject);
                playerHero->mLevelObjects.push_back(std::move(tempObject));

                audioManager.setMusic(Music::ID::FirstMainMusic);
            }

            break;
        }

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            break;

        default:
            std::cerr << "Error! Out of range level.\n";
            throw std::invalid_argument(std::to_string(_currentLevelNumber));
    }
}

void WorldContext::handleCollisions(Entity& entity)
{
    switch (_currentLevelNumber)
    {
        case 1:
        {
            /// Damage on the first boss Shadow.
            if (_shadowBoss.mShadow->getRect().intersects(entity.getRect()) &&
                _shadowBoss.mShadow->mLife && _shadowBoss.mShadow->mHitpoints > 0 &&
                _shadowBoss.mIsActive && _shadowBoss.mShadow->idling)
            {
                _shadowBoss.mShadow->mHitpoints -= entity.mDamage;
                if (_shadowBoss.mShadow->mHitpoints <= 0)
                {
                    _shadowBoss.mIsFinished = true;
                }
                entity.mLife = false;
            }

            /// Damage on the first mini-boss GolemDark.
            if (_golemBoss.mGolem->getRect().intersects(entity.getRect()) &&
                _golemBoss.mGolem->mLife && _golemBoss.mGolem->mHitpoints > 0 &&
                _golemBoss.mIsActive)
            {
                _golemBoss.mGolem->mHitpoints -= entity.mDamage;
                if (_golemBoss.mGolem->mHitpoints <= 0)
                {
                    _golemBoss.mIsFinished = true;
                }
                entity.mLife = false;
            }

            break;
        }
        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            break;

        default:
            std::cerr << "Error! Out of range level.\n";
            throw std::invalid_argument(std::to_string(_currentLevelNumber));
    }
}

bool WorldContext::isGolemDarkActive() const
{
    return _golemBoss.mIsActive;
}
