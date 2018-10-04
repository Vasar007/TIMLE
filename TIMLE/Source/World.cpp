#include <limits>

#include <SFML/Graphics/RenderWindow.hpp>

#include "../Include/World.hpp"
#include "../Include/Projectile.hpp"
#include "../Include/Utility.hpp"
#include "../Include/Ghost.hpp"
#include "../Include/Golem.hpp"
#include "../Include/DarkSoldier.hpp"
#include "../Include/Shadow.hpp"
#include "../Include/Goblin.hpp"
#include "../Include/MinotaurMage.hpp"
#include "../Include/Dwarf.hpp"
#include "../Include/MovingPlatform.hpp"
#include "../Include/Bullet.hpp"
#include "../Include/Rock.hpp"
#include "../Include/Gate.hpp"
#include "../Include/DialogPerson.hpp"
#include "../Include/Tentacle.hpp"
#include "../Include/DeadMan.hpp"
#include "../Include/Flamestrike.hpp"
#include "../Include/DarkArcher.hpp"
#include "../Include/MagicArrow.hpp"
#include "../Include/Bloodsplat.hpp"


World::World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
             SoundBufferHolder& sounds, PlayerInfo* playerInfo, AudioManager& audioManager, 
             const State::DebugMode debugMode, const std::size_t levelNumber)
: _window(window)
, _worldView(window.getDefaultView())
, _textures(textures)
, _fonts(fonts)
, _sounds(sounds)
, _audioManager(audioManager)
, _level(nullptr)
, _currentLevelNumber(levelNumber)
, _playerHero(nullptr)
, _playerInfo(playerInfo)
, _lifeBar(nullptr)
, _debug(debugMode == State::DebugMode::DebugOn)
{
    _sound.setBuffer(sounds.get(Sounds::ID::Bullet));

    audioManager.setMusic(Music::ID::FirstMainMusic);

    buildScene();
    _worldView.reset(sf::FloatRect(0, 0, 1280, 720));
    _worldView.zoom(0.5f);
}

void World::loadLevel(const std::size_t levelNumber)
{
    _currentLevelNumber = levelNumber;

    switch (levelNumber)
    {
        case 1:
            _level->loadFromFile("Level1.tmx");
            break;
        
        case 2:
            _level->loadFromFile("Level2.tmx");
            break;
        
        case 3:
            _level->loadFromFile("Level3.tmx");
            break;
        
        case 4:
            _level->loadFromFile("test_map.tmx"); // Test level for Vasilyev.
            break;
        
        case 5:
            _level->loadFromFile("supertest_map.tmx"); // Test level for Gusev.
            break;
        
        default:
            std::cout << "Error! Out of range level.\n";
            _currentLevelNumber = 1;
            break;
    }
}

std::size_t World::getLevelNumber() const
{
    return _currentLevelNumber;
}

void World::update(sf::Time dt)
{
    // Pass the coordinates of the player in the camera control feature.
    setPlayerCoordinateForView(_playerHero->x, _playerHero->y, _currentLevelNumber);

    guideMissiles();
    
    for (auto it = _entities.begin(); it != _entities.end();)
    {
        // Call the update function for objects.
        (*it)->update(static_cast<float>(dt.asMilliseconds()));
        if ((*it)->mLife)
        {
            ++it;
        }
        // If the object is "dead".
        else
        {
            it = _entities.erase(it);
        }
    }

    for (auto it = _effects.begin(); it != _effects.end();)
    {
        (*it)->update(static_cast<float>(dt.asMilliseconds()));
        if ((*it)->mLife)
        {
            ++it;
        }
        else
        {
            it = _effects.erase(it);
        }
    }

    if (_playerHero->mIsShoot && !_playerHero->mShooted)
    {
        _playerHero->mIsShoot = false;
        _playerHero->mShooted = true;
    }
    else if (_playerHero->mIsShooting)
    {
        _playerHero->mIsShooting = false;
        // If shot, it appears the bullet, pass the enum as an int.
        _entities.emplace_back(std::make_unique<Bullet>(
            Type::ID::AlliedBullet, _textures, _fonts, *_level, 
            _playerHero->x + (_playerHero->mWidth / 2.f), 
            _playerHero->y + 6.f, 7, 7, static_cast<int>(_playerHero->mState))
        );
        // Play bullet's sound.
        _sound.play();
    }

    if (_playerInfo->mNeedTransit)
    {
        _playerInfo->mDoTransit = true;
        _playerInfo->mNeedTransit = false;

        Object otherDoor;
        for (const auto& door : _doors)
        {
            if (door.mId != _playerInfo->mNumberOfDoor.first &&
                door.mType == _playerInfo->mNumberOfDoor.second)
            {
                otherDoor = door;
                break;
            }
        }
        _playerInfo->mTransitPos = { otherDoor.mRect.left, otherDoor.mRect.top };
        _playerInfo->mTransiting = true;
    }

    // Updates ShadowBoss actions.
    if (_shadowBoss.mIsActive)
    {
        for (auto it = _shadowBoss.mTentaclesStatic.begin(); 
             it != _shadowBoss.mTentaclesStatic.end();)
        {
            (*it).update(static_cast<float>(dt.asMilliseconds()));
            if ((*it).mLife)
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
            (*it).update(static_cast<float>(dt.asMilliseconds()));
            if ((*it).mLife)
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

    // Updates player's lifebar.
    _lifeBar->update(_playerHero->mHitpoints);

    _playerHero->update(static_cast<float>(dt.asMilliseconds()));

    // Checks collision between different objects.
    handleCollisions(static_cast<float>(dt.asMilliseconds()));

    _playerInfo->mDialogNumber = 0;
    if (_playerHero->mDialogNumber == 0)
    {
        _playerInfo->mDialogNumber = 0;
    }
    else
    {
        _playerInfo->showDialog(_playerHero->mDialogNumber);
    }
}

void World::draw()
{
    _window.setView(_worldView);

    // Draw a level.
    _level->draw(_window);

    const auto center = _window.getView().getCenter();
    const auto size = _window.getView().getSize();

    auto viewRect = sf::FloatRect(center.x - (size.x / 2.f) - 100.f,
                                  center.y - (size.y / 2.f) - 100.f, 
                                  size.x + 100.f, size.y + 100.f);

    for (const auto& entity : _entities)
    {
        if (_debug)
        {
            switch (entity->mTypeID)
            {
                case Type::ID::Ghost:
                    [[fallthrough]];
                case Type::ID::Golem:
                    [[fallthrough]];
                case Type::ID::DarkSoldier:
                    [[fallthrough]];
                case Type::ID::Goblin:
                    [[fallthrough]];
                case Type::ID::MinotaurMage:
                    [[fallthrough]];
                case Type::ID::Dwarf:
                    [[fallthrough]];
                case Type::ID::DwarfArcher:
                    [[fallthrough]];
                case Type::ID::DwarvenCommander:
                    [[fallthrough]];
                case Type::ID::Tentacle:
                    [[fallthrough]];
                case Type::ID::DarkArcher:
                {
                    auto shape = buildBorderLines(entity->getRect(), sf::Color::Transparent,
                                                  sf::Color::Red, 1.f);
                    _window.draw(shape);

                    auto bodyShape = buildBorderLines(entity->getBodyRect(), sf::Color::Transparent,
                                                      sf::Color::Black, 1.f);
                    _window.draw(bodyShape);
                    break;
                }

                case Type::ID::Rock:
                    [[fallthrough]];
                case Type::ID::OpeningGate:
                    [[fallthrough]];
                case Type::ID::ClosingGate:
                    [[fallthrough]];
                case Type::ID::OpenClosingGate:
                {
                    auto shape = buildBorderLines(entity->getRect(), sf::Color::Transparent,
                                                  sf::Color::Yellow, 1.f);
                    _window.draw(shape);
                    break;
                }

                case Type::ID::Oswald:
                    [[fallthrough]];
                case Type::ID::Heinrich:
                    [[fallthrough]];
                case Type::ID::MovingPlatform:
                    [[fallthrough]];
                case Type::ID::DeadJuggernaut:
                    [[fallthrough]];
                case Type::ID::DeadDwarf:
                {
                    auto shape = buildBorderLines(entity->getRect(), sf::Color::Transparent,
                                                  sf::Color::Magenta, 1.f);
                    _window.draw(shape);
                    break;
                }

                case Type::ID::AlliedBullet:
                    [[fallthrough]];
                case Type::ID::EnemyBullet:
                    [[fallthrough]];
                case Type::ID::Flamestrike:
                {
                    auto shape = buildBorderLines(entity->getRect(), sf::Color::Transparent,
                                                  sf::Color::Red, 1.f);
                    _window.draw(shape);
                    break;
                }

                default:
                {
                    auto shape = buildBorderLines(entity->getRect(), sf::Color::Transparent,
                                                  sf::Color::White, 1.f);
                    _window.draw(shape);
                    break;
                }
            }
        }
        
        if (viewRect.contains(entity->x, entity->y))
        {
            entity->draw(_window);
        }
    }

    for (const auto& effect : _effects)
    {
        if (viewRect.contains(effect->x, effect->y))
        {
            effect->draw(_window);
        }
    }

    if (_shadowBoss.mIsActive)
    {
        for (const auto& tentacle : _shadowBoss.mTentaclesStatic)
        {
            if (_debug)
            {
                auto shape = buildBorderLines(tentacle.getRect(), sf::Color::Transparent,
                                              sf::Color::Red, 1.f);
                _window.draw(shape);
            }
            
            tentacle.draw(_window);
        }

        for (const auto& tentacle : _shadowBoss.mTentacles)
        {
            if (_debug)
            {
                auto shape = buildBorderLines(tentacle.getRect(), sf::Color::Transparent,
                                              sf::Color::Red, 1.f);
                _window.draw(shape);
            }
            
            tentacle.draw(_window);
        }
        
        if (_debug)
        {
            auto shape = buildBorderLines(_shadowBoss.mShadow->getRect(), sf::Color::Transparent,
                                          sf::Color::Red, 1.f);
            _window.draw(shape);
        }

        _shadowBoss.mShadowLifeBar->draw(_window);

        _shadowBoss.mShadow->draw(_window);
    }

    if (_golemBoss.mIsActive)
    {
        if (_debug)
        {
            auto shape = buildBorderLines(_golemBoss.mGolem->getRect(), sf::Color::Transparent,
                                          sf::Color::Red, 1.f);
            _window.draw(shape);
        }

        _golemBoss.mGolemLifeBar->draw(_window);

        _golemBoss.mGolem->draw(_window);
    }
    
    if (_debug)
    {
        for (const auto& object : _objects)
        {
            if (object.mName == "solid")
            {
                auto shape = buildBorderLines(object.mRect, sf::Color::Transparent,
                                              sf::Color::Black, 1.f);
                _window.draw(shape);
            }
            else if (object.mName == "enemyBorder")
            {
                auto shape = buildBorderLines(object.mRect, sf::Color::Transparent,
                                              sf::Color::Red, 1.f);
                _window.draw(shape);
            }
            else if (object.mName == "death")
            {
                auto shape = buildBorderLines(object.mRect, sf::Color::Transparent,
                                              sf::Color::Magenta, 1.f);
                _window.draw(shape);
            }
            else if (object.mName == "end")
            {
                auto shape = buildBorderLines(object.mRect, sf::Color::Transparent,
                                              sf::Color::White, 1.f);
                _window.draw(shape);
            }
            else if (object.mName == "boss")
            {
                auto shape = buildBorderLines(object.mRect, sf::Color::Transparent,
                                              sf::Color::Red, 1.f);
                _window.draw(shape);
            }
            else if (object.mName == "dialogMessage")
            {
                auto shape = buildBorderLines(object.mRect, sf::Color::Transparent,
                                              sf::Color::Blue, 1.f);
                _window.draw(shape);
            }
            else if (object.mName == "door")
            {
                auto shape = buildBorderLines(object.mRect, sf::Color::Transparent,
                                              sf::Color::Cyan, 1.f);
                _window.draw(shape);
            }
            else
            {
                auto shape = buildBorderLines(object.mRect, sf::Color::Transparent,
                                              sf::Color::White, 1.f);
                _window.draw(shape);
            }
        }

        auto shape = buildBorderLines(_playerHero->getRect(), sf::Color::Transparent, 
                                      sf::Color::Green, 1.f);
        _window.draw(shape);
    }

    for (const auto& rect : _debugRectsToDraw)
    {
        if (_debug)
        {
            _window.draw(rect);
        }
    }
    _debugRectsToDraw.clear();
    
    _lifeBar->draw(_window);

    _playerHero->draw(_window);
}

bool World::hasAlivePlayer() const
{
    return _playerHero->mLife;
}

bool World::hasPlayerReachedEnd() const
{
    return _playerHero->mIsRichedEnd;
}

void World::setPlayerCoordinateForView(const float x, const float y, const std::size_t levelNumber)
{
    float tempX = x;
    float tempY = y;

    switch (levelNumber)
    {
        case 1:
            if (x < 400.f)
            {
                tempX = 400.f;
            }

            if (y < 100.f)
            {
                tempY = 100.f;
            }

            if (y > 3000.f)
            {
                tempY = 3000.f;
            }

            if (x > 12800.f)
            {
                tempX = 12800.f;
            }
            break;

        case 2:
            break;

        case 3:
            break;

        case 4:
            if (x < 320.f)
            {
                tempX = 320.f;
            }

            if (y < 100.f)
            {
                tempY = 100.f;
            }

            if (y > 620.f)
            {
                tempY = 620.f;
            }

            if (x > 1280.f)
            {
                tempX = 1280.f;
            }
            break;
        
        case 5:
            if (x < 320.f)
            {
                tempX = 320.f;
            }

            if (y < 100.f)
            {
                tempY = 100.f;
            }

            if (y > 620.f)
            {
                tempY = 620.f;
            }

            if (x > 1280.f)
            {
                tempX = 1280.f;
            }
            break;
        
        default:
            std::cout << "Error! Out of range level.\n";
            break;
    }

    _worldView.setCenter(tempX, tempY);

    if (_golemBoss.mIsActive && _golemBoss.mIsShaked)
    {
        ++_golemBoss.mCameraCounter;
        switch (_golemBoss.mCameraCounter)
        {
            case 1:
                _worldView.move(10, 10);
                break;
            
            case 2:
                _worldView.move(-20, 0);
                break;
            
            case 3:
                _worldView.move(0, -20);
                break;
            
            case 4:
                _worldView.move(0, 20);
                break;
            
            default:
            {
                _worldView.move(10, -10);
                _golemBoss.mCameraCounter = 0;
                _golemBoss.mIsShaked = false;

                const int randomNum = randomInt(8);
                std::unique_ptr<Entity> rock = std::make_unique<Rock>(
                    Type::ID::Rock, _textures, _fonts, *_level,
                    _golemBoss.mRocks.at(randomNum).x,
                    _golemBoss.mRocks.at(randomNum).y, 16, "3");
                rock->mIsAttacked = true;
                _entities.emplace_back(std::move(rock));

                break;
            }
        }
    }
}

void World::buildScene()
{
    // Initialization of level.
    _level = std::make_unique<Level>();
    loadLevel(_currentLevelNumber);

    // Add player.
    const auto playerObj = _level->getObject("player");
    _playerHero = std::make_unique<Player>(
        Type::ID::Archer, _textures, _fonts, *_level, playerObj.mRect.left, playerObj.mRect.top,
        20, 30, *_playerInfo
    );

    _playerInfo->setPlayer(_playerHero.get());

    _lifeBar = std::make_unique<LifeBar>(
        Type::ID::HealthBar, _textures, _fonts, _playerHero->mHitpoints
    );

    _playerInfo->mLastSavePoint.x = _playerHero->x;
    _playerInfo->mLastSavePoint.y = _playerHero->y;

    // Create and initialize first boss Shadow.
    const auto shadowObj = _level->getObject("bossShadow");
    _shadowBoss.mShadow = std::make_unique<Shadow>(
        Type::ID::Shadow, _textures, _fonts, *_level,
        shadowObj.mRect.left, shadowObj.mRect.top,  40, 35, shadowObj.mType
    );
    _shadowBoss.mShadowLifeBar = std::make_unique<LifeBar>(
        Type::ID::ShadowBossBar, _textures, _fonts, _shadowBoss.mShadow->mHitpoints
    );

    for (std::size_t i = 0; i < _shadowBoss.mNumberOfTentacles; ++i)
    {
        _shadowBoss.mTentaclesStatic.emplace_back(Type::ID::Tentacle, _textures, _fonts,
                                                  *_level, 12013.f + 13.f * i, 994.f, 13, 45, "0");
        _shadowBoss.mTentaclesStatic.emplace_back(Type::ID::Tentacle, _textures, _fonts,
                                                  *_level, 12061.f + 13.f * i, 1073.f, 13, 45, "0");
        _shadowBoss.mTentaclesStatic.emplace_back(Type::ID::Tentacle, _textures, _fonts,
                                                  *_level, 12285.f + 13.f * i, 1073.f, 13, 45, "0");
        _shadowBoss.mTentaclesStatic.emplace_back(Type::ID::Tentacle, _textures, _fonts,
                                                  *_level, 12333.f + 13.f * i, 994.f, 13, 45, "0");
        _shadowBoss.mTentaclesStatic.emplace_back(Type::ID::Tentacle, _textures, _fonts,
                                                  *_level, 12173.f + 13.f * i, 946.f, 13, 45, "0");
    }

    // Create and initialize first mini-boss GolemDark.
    const auto golemObj = _level->getObject("bossGolemDark");
    _golemBoss.mGolem = std::make_unique<GolemDark>(
        Type::ID::GolemDark, _textures, _fonts, *_level,
        golemObj.mRect.left, golemObj.mRect.top, 70, 60, golemObj.mType
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

    // Add objects and enemies.
    addObjects();
}

void World::addObjects()
{
    // All objects, which contains tmx-file, program read into temporary vector.
    // Add all objects with current name to list of objects.
    for (const auto& object : _level->getObjects("enemyGhost"))
    {
        _entities.emplace_back(
            std::make_unique<Ghost>(Type::ID::Ghost, _textures, _fonts, *_level,
                                    object.mRect.left, object.mRect.top, 
                                    static_cast<int>(object.mRect.width), 
                                    static_cast<int>(object.mRect.height), object.mType)
        );
    }

    for (const auto& object : _level->getObjects("enemyGolem"))
    {
        _entities.emplace_back(
            std::make_unique<Golem>(Type::ID::Golem, _textures, _fonts, *_level,
                                    object.mRect.left, object.mRect.top, 
                                    static_cast<int>(object.mRect.width),
                                    static_cast<int>(object.mRect.height), object.mType)
        );
    }

    for (const auto& object : _level->getObjects("enemyDarkSoldier"))
    {
        _entities.emplace_back(
            std::make_unique<DarkSoldier>(Type::ID::DarkSoldier,  _textures, _fonts, *_level,
                                          object.mRect.left, object.mRect.top, 
                                          static_cast<int>(object.mRect.width),
                                          static_cast<int>(object.mRect.height), object.mType)
        );
    }

    for (const auto& object : _level->getObjects("enemyGoblin"))
    {
        _entities.emplace_back(
            std::make_unique<Goblin>(Type::ID::Goblin,  _textures, _fonts, *_level,
                                     object.mRect.left, object.mRect.top, 
                                     static_cast<int>(object.mRect.width),
                                     static_cast<int>(object.mRect.height), object.mType)
        );
    }

    for (const auto& object : _level->getObjects("enemyMinotaurMage"))
    {
        _entities.emplace_back(
            std::make_unique<MinotaurMage>(Type::ID::MinotaurMage, _textures, _fonts, *_level,
                                            object.mRect.left, object.mRect.top, 
                                            static_cast<int>(object.mRect.width),
                                            static_cast<int>(object.mRect.height), object.mType)
        );
    }

    for (const auto& object : _level->getObjects("enemyDwarf"))
    {
        _entities.emplace_back(
            std::make_unique<Dwarf>(Type::ID::Dwarf, _textures, _fonts, *_level,
                                    object.mRect.left, object.mRect.top, 
                                    static_cast<int>(object.mRect.width),
                                    static_cast<int>(object.mRect.height), object.mType,
                                    Dwarf::DwarfType::Dwarf)
        );
    }

    for (const auto& object : _level->getObjects("enemyDwarvenArcher"))
    {
        _entities.emplace_back(
            std::make_unique<Dwarf>(Type::ID::DwarfArcher, _textures, _fonts, *_level,
                                    object.mRect.left, object.mRect.top, 
                                    static_cast<int>(object.mRect.width),
                                    static_cast<int>(object.mRect.height), object.mType,
                                    Dwarf::DwarfType::DwarvenArcher)
        );
    }

    for (const auto& object : _level->getObjects("enemyDwarvenCommander"))
    {
        _entities.emplace_back(
            std::make_unique<Dwarf>(Type::ID::DwarvenCommander, _textures, _fonts, *_level,
                                    object.mRect.left, object.mRect.top, 
                                    static_cast<int>(object.mRect.width),
                                    static_cast<int>(object.mRect.height), object.mType,
                                    Dwarf::DwarfType::DwarvenCommander)
        );
    }

    for (const auto& object : _level->getObjects("enemyDwarfA"))
    {
        std::unique_ptr<Entity> dwarf = std::make_unique<Dwarf>(
            Type::ID::Dwarf, _textures, _fonts, *_level,
            object.mRect.left, object.mRect.top, 
            static_cast<int>(object.mRect.width),
            static_cast<int>(object.mRect.height), object.mType, Dwarf::DwarfType::Dwarf
        );
        dwarf->mIsStarted = true;
        _entities.emplace_back(std::move(dwarf));
    }

    for (const auto& object : _level->getObjects("enemyDwarvenArcherA"))
    {
        std::unique_ptr<Entity> dwarf = std::make_unique<Dwarf>(
            Type::ID::DwarfArcher, _textures, _fonts, *_level,
            object.mRect.left, object.mRect.top, 
            static_cast<int>(object.mRect.width),
            static_cast<int>(object.mRect.height), object.mType, Dwarf::DwarfType::DwarvenArcher
        );
        dwarf->mIsStarted = true;
        _entities.emplace_back(std::move(dwarf));
    }

    for (const auto& object : _level->getObjects("enemyDwarvenCommanderA"))
    {
        std::unique_ptr<Entity> dwarf = std::make_unique<Dwarf>(
            Type::ID::DwarvenCommander, _textures, _fonts, *_level,
            object.mRect.left, object.mRect.top, 
            static_cast<int>(object.mRect.width),
            static_cast<int>(object.mRect.height), object.mType, Dwarf::DwarfType::DwarvenCommander
        );
        dwarf->mIsStarted = true;
        _entities.emplace_back(std::move(dwarf));
    }

    for (const auto& object : _level->getObjects("enemyDwarvenCommanderM"))
    {
        std::unique_ptr<Entity> dwarf = std::make_unique<Dwarf>(
            Type::ID::DwarvenCommander, _textures, _fonts, *_level,
            object.mRect.left, object.mRect.top, 
            static_cast<int>(object.mRect.width),
            static_cast<int>(object.mRect.height), object.mType, Dwarf::DwarfType::DwarvenCommander
        );
        dwarf->mIsEnabling = true;
        _entities.emplace_back(std::move(dwarf));
    }

    for (const auto& object : _level->getObjects("enemyTentacle"))
    {
        _entities.emplace_back(
            std::make_unique<Tentacle>(Type::ID::Tentacle, _textures, _fonts, *_level,
                                       object.mRect.left, object.mRect.top, 
                                       static_cast<int>(object.mRect.width),
                                       static_cast<int>(object.mRect.height), object.mType)
        );
    }

    for (const auto& object : _level->getObjects("enemyDarkArcher"))
    {
        _entities.emplace_back(
            std::make_unique<DarkArcher>(Type::ID::DarkArcher, _textures, _fonts, *_level,
                                         object.mRect.left, object.mRect.top,
                                         static_cast<int>(object.mRect.width),
                                         static_cast<int>(object.mRect.height), object.mType)
        );
    }

    // Add falling rocks, not enemy.
    for (const auto& object : _level->getObjects("rock"))
    {
        _entities.emplace_back(
            std::make_unique<Rock>(Type::ID::Rock, _textures, _fonts, *_level,
                                   object.mRect.left, object.mRect.top, 16, object.mType)
        );
    }

    // Add opening gates, not enemy.
    for (const auto& object : _level->getObjects("gateO"))
    {
        _entities.emplace_back(
            std::make_unique<Gate>(Type::ID::OpeningGate, _textures, _fonts, *_level,
                                   object.mRect.left, object.mRect.top, 16, object.mType)
        );
    }

    // Add closing gates, not enemy.
    for (const auto& object : _level->getObjects("gateC"))
    {
        _entities.emplace_back(
            std::make_unique<Gate>(Type::ID::ClosingGate, _textures, _fonts, *_level,
                                   object.mRect.left, object.mRect.top, 16, object.mType)
        );
    }

    // Add open-closing gates, not enemy.
    for (const auto& object : _level->getObjects("gateOC"))
    {
        _entities.emplace_back(
            std::make_unique<Gate>(Type::ID::OpenClosingGate, _textures, _fonts, *_level,
                                   object.mRect.left, object.mRect.top, 16, object.mType)
        );
    }

    // Add dialog persons, not enemy.
    for (const auto& object : _level->getObjects("dialogPerson"))
    {
        switch(std::stoi(object.mType))
        {
            case 1:
                _entities.emplace_back(
                    std::make_unique<DialogPerson>(Type::ID::Oswald, _textures, _fonts, *_level,
                                                   object.mRect.left, object.mRect.top, 
                                                   static_cast<int>(object.mRect.width),
                                                   static_cast<int>(object.mRect.height), "2")
                );
                break;
            
            case 2:
                _entities.emplace_back(
                    std::make_unique<DialogPerson>(Type::ID::Heinrich, _textures, _fonts, *_level,
                                                   object.mRect.left, object.mRect.top, 
                                                   static_cast<int>(object.mRect.width),
                                                   static_cast<int>(object.mRect.height), "6")
                );
                break;
            
            default:
                std::cout << "Invalid type dialog person\n";
                break;
        }
    }

    // Add moving platforms, not enemy.
    for (const auto& object : _level->getObjects("movingPlatform"))
    {
        _entities.emplace_back(
            std::make_unique<MovingPlatform>(Type::ID::MovingPlatform, _textures, _fonts, *_level,
                                             object.mRect.left, object.mRect.top, 
                                             static_cast<int>(object.mRect.width),
                                             static_cast<int>(object.mRect.height), object.mType)
        );
    }

    // Add dead men, not enemy.
    for (const auto& object : _level->getObjects("deadMan"))
    {
        switch (std::stoi(object.mType))
        {
            case 1:
                _entities.emplace_back(
                    std::make_unique<DeadMan>(Type::ID::DeadJuggernaut, _textures, _fonts, 
                                              *_level, object.mRect.left, object.mRect.top, 
                                              static_cast<int>(object.mRect.width),
                                              static_cast<int>(object.mRect.height),
                                              object.mType)
                );
                break;
            
            case 2:
                _entities.emplace_back(
                    std::make_unique<DeadMan>(Type::ID::DeadDwarf, _textures, _fonts, *_level,
                                              object.mRect.left, object.mRect.top, 
                                              static_cast<int>(object.mRect.width),
                                              static_cast<int>(object.mRect.height),
                                              object.mType)
                );
                break;
            
            default:
                std::cout << "Invalid type dead person\n";
                break;
        }
    }

    // Add interaction objects, not enemy.
    loadObjects("solid");
    loadObjects("enemyBorder");  
    loadObjects("death");
    loadObjects("end");
    loadObjects("boss");
    loadObjects("dialogMessage");
    loadObjects("door");
    _doors = _level->getObjects("door");
}

bool World::loadObjects(const std::string_view objectName)
{
    const auto objects = _level->getObjects(objectName);
    if (objects.begin() != objects.end())
    {
        _objects.insert(_objects.end(), std::make_move_iterator(objects.begin()),
                        std::make_move_iterator(objects.end()));

        return true;
    }

    return false;
}

void World::addEnemy(const Type::ID type, const float relX, const float relY)
{
    _enemySpawnPoints.emplace_back(type, relX, relY);
}

void World::guideMissiles()
{
    for (const auto& missile : _guidedProjectiles)
    {
        missile->guideTowards(sf::Vector2f(_playerHero->getCenter().x, _playerHero->getCenter().y));
    }
}


void World::spawnEnemies()
{

}

void World::destroyEntitiesOutsideView()
{

}

sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
    // Return view bounds + some area at top and left, where enemies can be.
    auto bounds = getViewBounds();
    bounds.top -= 150.f;
    bounds.height += 300.f;
    bounds.left -= 150.f;
    bounds.width += 300.f;

    return bounds;
}

void World::handleCollisions(const float dt)
{
    _playerHero->mOnPlatform = 0.f;

    auto playerRect = _playerHero->getRect();

    auto battleRect = getBattlefieldBounds();

    /// If we activated first boss Shadow...
    if (_playerHero->mHasStartedFirstMainBoss && !_shadowBoss.mIsFinished)
    {
        _shadowBoss.mIsActive = true;

        if (_shadowBoss.mShadow->mTypeID == Type::ID::Shadow)
        {
            // If Shadow was activated first time...
            if (!_shadowBoss.mShadow->mIsStarted)
            {
                _shadowBoss.mShadow->mIsStarted = true;
                _audioManager.setMusic(Music::ID::FirstBossMusic);
            }
            else if (_playerHero->mHitpoints > 0)
            {
                if (!_shadowBoss.mShadow->mIsBack &&
                    ((_playerHero->x - _shadowBoss.mShadow->x > 0.f &&
                        _shadowBoss.mShadow->dx < 0.f) ||
                        (_playerHero->x - _shadowBoss.mShadow->x < 0.f &&
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
                if (_shadowBoss.mShadow->mIsHitted && !_shadowBoss.mShadow->mIsStay)
                {
                    _playerHero->mHitpoints -= _shadowBoss.mShadow->mDamage;
                    _shadowBoss.mTentacles.emplace_back(Type::ID::Tentacle, _textures,
                                                        _fonts, *_level, _playerHero->x - 50.f, 
                                                        _shadowBoss.mShadow->y - 10.f, 13, 45, "1");
                    _shadowBoss.mTentacles.emplace_back(Type::ID::Tentacle, _textures,
                                                        _fonts, *_level, _playerHero->x - 25.f, 
                                                        _shadowBoss.mShadow->y - 10.f, 13, 45, "1");
                    _shadowBoss.mTentacles.emplace_back(Type::ID::Tentacle, _textures,
                                                        _fonts, *_level, _playerHero->x, 
                                                        _shadowBoss.mShadow->y - 10.f, 13, 45, "1");
                    _shadowBoss.mTentacles.emplace_back(Type::ID::Tentacle, _textures,
                                                        _fonts, *_level, _playerHero->x + 25.f, 
                                                        _shadowBoss.mShadow->y - 10.f, 13, 45, "1");
                    _shadowBoss.mTentacles.emplace_back(Type::ID::Tentacle, _textures,
                                                        _fonts, *_level, _playerHero->x + 50.f, 
                                                        _shadowBoss.mShadow->y - 10.f, 13, 45, "1");
                    _shadowBoss.mShadow->mIsHitted = false;
                    std::cout << "Shadow ATTACK\n";
                }
            }
        }

        if (_shadowBoss.mShadow->getRect().intersects(playerRect) &&
            _shadowBoss.mShadow->mIsDelay)
        {
            _playerHero->mHitpoints = 0;
        }

        for (auto& tentacle : _shadowBoss.mTentaclesStatic)
        {
            if (_shadowBoss.mShadow->mIsCalling)
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
            else if (_shadowBoss.mShadow->mIsWithdrawing)
            {
                tentacle.mIsDisabled = true;
                tentacle.mIsEnabling = false;
            }

            if (_shadowBoss.mIsFinished)
            {
                tentacle.mHitpoints = 0;
            }

            if (tentacle.getRect().intersects(playerRect) && _playerHero->mHitpoints > 0)
            {
                tentacle.mIsAttacked = true;
                if (tentacle.mIsHitted)
                {
                    _playerHero->mHitpoints -= tentacle.mDamage;
                    tentacle.mIsHitted = false;
                    std::cout << "Hit\n";
                }
            }
        }

        for (auto& tentacle : _shadowBoss.mTentacles)
        {
            if (_shadowBoss.mShadow->mIsWithdrawing)
            {
                tentacle.mHitpoints = 0;
            }
            else
            {
                tentacle.mIsStarted = true;
                tentacle.mIsEnabling = true;

                if (tentacle.getRect().intersects(playerRect) && _playerHero->mHitpoints > 0)
                {
                    tentacle.mIsAttacked = true;
                    if (tentacle.mIsHitted)
                    {
                        _playerHero->mHitpoints -= tentacle.mDamage;
                        tentacle.mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                }
            }
        }
    }
    else if (_shadowBoss.mIsFinished && !_shadowBoss.mShadow->mLife)
    {
        _shadowBoss.mIsActive = false;
        _playerInfo->mQuests.at(PlayerInfo::Quest::KillShadow) = true;
        _audioManager.setMusic(Music::ID::FirstMainMusic);
    }


    /// If we activated firs mini-boss GolemDark...
    if (_playerHero->mHasStartedFirstMiniBoss && !_golemBoss.mIsFinished)
    {
        _golemBoss.mIsActive = true;

        if (_golemBoss.mGolem->mTypeID == Type::ID::GolemDark)
        {
            if (!_golemBoss.mIsWeakened &&
                (_playerInfo->mChosenSolution.at(PlayerInfo::Solution::InteractWithGolem) == 1))
            {
                _golemBoss.mGolem->mHitpoints -= 100;
                _golemBoss.mIsWeakened = true;
            }

            // If GolemDark was activated first time.
            if (!_golemBoss.mGolem->mIsStarted)
            {
                _golemBoss.mGolem->mIsStarted = true;
                _golemBoss.mGolem->mCurrentDeath = 5.f;
                _audioManager.setMusic(Music::ID::FirstMiniBossMusic);
            }
            else if (_golemBoss.mGolem->getRect().intersects(playerRect))
            {
                if (_playerHero->mHitpoints > 0)
                {
                    if (!_golemBoss.mGolem->mIsBack &&
                        ((_playerHero->x - _golemBoss.mGolem->x > 0.f &&
                        (_golemBoss.mGolem->dx < 0.f)) ||
                         ((_playerHero->x - _golemBoss.mGolem->x) < 0.f &&
                            _golemBoss.mGolem->dx > 0.f)))
                    {
                        _golemBoss.mGolem->mIsBack = true;
                        _golemBoss.mGolem->dx = -_golemBoss.mGolem->dx;
                    }
                    _golemBoss.mGolem->mIsAttacked = true;
                    _golemBoss.mGolem->mCurrentFrame = 0.f;
                    if (_golemBoss.mGolem->mIsHitted)
                    {
                        _playerHero->mHitpoints -= _golemBoss.mGolem->mDamage;
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
    else if (_golemBoss.mIsFinished && !_golemBoss.mGolem->mLife)
    {
        _golemBoss.mIsActive = false;
        _playerInfo->mQuests.at(PlayerInfo::Quest::KillGolemDark) = true;

        // Temporary object.
        Object tempObject;
        
        tempObject.mName = "dialogMessage";
        tempObject.mType = "9";

        sf::FloatRect objectRect;
        objectRect.top    = 2388;
        objectRect.left   = 8352;
        objectRect.height = 60;
        objectRect.width  = 80;
        tempObject.mRect  = std::move(objectRect);
        _level->mObjects.push_back(tempObject);
        _playerHero->mLevelObjects.push_back(std::move(tempObject));

        _audioManager.setMusic(Music::ID::FirstMainMusic);
    }

    for (auto it = _entities.begin(); it != _entities.end(); ++it)
    {
        if (!battleRect.intersects((*it)->getRect()))
        {
            continue;
        }

        /// If player collided with MovingPlatform...
        if ((*it)->mTypeID == Type::ID::MovingPlatform && (*it)->getRect().intersects(playerRect))
        {
            // If the player is in a state after the jump, i.e. falls down...
            if (_playerHero->dy > 0.f || !_playerHero->mOnGround)
            {
                // If the player is above the platform, i.e. it's his feet at least 
                // (since we already checked that he was confronted with platform)...
                if (_playerHero->y + _playerHero->mHeight < (*it)->y + (*it)->mHeight)
                {
                    _playerHero->y = (*it)->y - _playerHero->mHeight + 3.f;
                    _playerHero->x += (*it)->dx * dt;
                    _playerHero->dy = 0;
                    _playerHero->mOnPlatform = (*it)->dx;
                    // It pushes the player so that he was standing on the platform.
                    _playerHero->mOnGround = true;
                }
            }
        }

        /// Special attacks enemies.
        /// Special attack of MinotaurMage.
        if ((*it)->mTypeID == Type::ID::MinotaurMage && (*it)->mHitpoints > 0)
        {
            sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), 
                                     static_cast<float>((*it)->mHeight));
            std::size_t distance = 0;
            bool isNeedFind = true;

            while (distance < 10 && isNeedFind)
            {
                if (findPlayer.intersects(playerRect) && _playerHero->mLife)
                {
                    (*it)->mIsAttacked = true;
                    break;
                }

                if ((*it)->dx > 0.f)
                {
                    findPlayer.left += 30.f;
                }
                else
                {
                    findPlayer.left -= 30.f;
                }

                // TODO: redo this loop because it's very ineffective.
                std::for_each(_objects.begin(), _objects.end(),
                    [&findPlayer, &isNeedFind](Object& object)
                {
                    if (findPlayer.intersects(object.mRect) && object.mName == "solid")
                    {
                        isNeedFind = false;
                    }
                });

                ++distance;
            }

            _debugRectsToDraw.emplace_back(buildBorderLines(findPlayer, sf::Color::Transparent,
                                                         sf::Color::Black, 1.f));

            if ((*it)->mIsAttacked)
            {
                (*it)->mCurrentFrame = 0.f;

                if ((*it)->mIsHitted)
                {
                    // Нанесение урона
                    std::unique_ptr<Entity> flamestrike = std::make_unique<Flamestrike>(
                        Type::ID::Flamestrike, _textures, _fonts, *_level,
                        _playerHero->x + static_cast<float>(_playerHero->mWidth) / 4.f,
                        _playerHero->y - static_cast<float>(_playerHero->mHeight) / 2.f,
                        13, 45
                    );
                    flamestrike->mIsStarted = true;
                    _entities.emplace_back(std::move(flamestrike));

                    (*it)->mIsHitted = false;
                    std::cout << "Hit\n";
                }
            }
        }

        /// Special attack of DwarfArcher.
        if ((*it)->mTypeID == Type::ID::DwarfArcher && (*it)->mHitpoints > 0)
        {
            sf::FloatRect findPlayer((*it)->x, (*it)->y - 5.f, static_cast<float>((*it)->mWidth), 
                                     static_cast<float>((*it)->mHeight));
            std::size_t distance = 0;
            bool isNeedFind = true;

            while (distance < 10 && isNeedFind) 
            {
                if (findPlayer.intersects(playerRect) && _playerHero->mLife)
                {
                    (*it)->mIsAttacked = true;
                    break;
                }

                if ((*it)->dx > 0.f)
                {
                    findPlayer.left += 30.f;
                }
                else
                {
                    findPlayer.left -= 30.f;
                }

                // TODO: redo this loop because it's very ineffective.
                std::for_each(_objects.begin(), _objects.end(),
                    [&findPlayer, &isNeedFind](Object& object)
                {
                    if (findPlayer.intersects(object.mRect) && object.mName == "solid")
                    {
                        isNeedFind = false;
                    }
                });

                ++distance;
            }
            
            _debugRectsToDraw.emplace_back(buildBorderLines(findPlayer, sf::Color::Transparent,
                                                            sf::Color::Black, 1.f));

            if ((*it)->mIsAttacked)
            {
                (*it)->mCurrentFrame = 0.f;
                if ((*it)->mIsHitted)
                {
                    _entities.emplace_back(
                        std::make_unique<Bullet>(
                            Type::ID::EnemyBullet, _textures, _fonts,
                            *_level, (*it)->x + ((*it)->mWidth / 2.f),
                            (*it)->y + ((*it)->mHeight / 2.f) - 3.f, 7, 7,
                            (*it)->dx > 0.f ? 1 : 0)
                    );
                    _sound.play();
                    (*it)->mIsHitted = false;
                    std::cout << "Shoot\n";
                }
            }
        }

        /// Special attack of DarkArcher.
        if ((*it)->mTypeID == Type::ID::DarkArcher && (*it)->mHitpoints > 0)
        {
            sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth),
                                     static_cast<float>((*it)->mHeight));

            std::size_t distance = 0;
            bool isNeedFind = true;

            while (distance < 15 && isNeedFind)
            {
                if (findPlayer.intersects(playerRect) && _playerHero->mLife)
                {
                    if (!(*it)->mIsEnd)
                    {
                        (*it)->mIsStarted = true;
                    }
                    else
                    {
                        (*it)->mIsAttacked = true;
                    }
                    break;
                }

                if ((*it)->dx > 0.f)
                {
                    findPlayer.left += 30.f;
                }
                else
                {
                    findPlayer.left -= 30.f;
                }

                // TODO: redo this loop because it's very ineffective.
                //std::for_each(_objects.begin(), _objects.end(),
                //    [&findPlayer, &isNeedFind](Object& object)
                //{
                //    if (findPlayer.intersects(object.mRect) && object.mName == "solid")
                //    {
                //        isNeedFind = false;
                //    }
                //});

                ++distance;
            }

            _debugRectsToDraw.emplace_back(buildBorderLines(findPlayer, sf::Color::Transparent,
                                                            sf::Color::Black, 1.f));

            if ((*it)->mIsAttacked)
            {
                (*it)->mCurrentFrame = 0.f;
                if ((*it)->mIsHitted)
                {
                    std::unique_ptr<Projectile> bullet = std::make_unique<MagicArrow>(
                        Type::ID::MagicArrow, _textures, _fonts, *_level,
                        (*it)->x + (*it)->mWidth / 2.f,
                        (*it)->y + (*it)->mHeight / 2.f - 12.f, 
                        13, 9,
                        _playerHero->getCenter().x,
                        _playerHero->getCenter().y
                    );
                    _guidedProjectiles.emplace_back(bullet.get());
                    _entities.emplace_back(std::move(bullet));
                    _sound.play();
                    (*it)->mIsHitted = false;
                    std::cout << "Shoot\n";
                }
            }
        }

        /// The fall of the stones.
        if ((*it)->mTypeID == Type::ID::Rock && !(*it)->mIsAttacked)
        {
            sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), 
                                     static_cast<float>((*it)->mHeight) + _playerHero->y);

            _debugRectsToDraw.emplace_back(buildBorderLines(findPlayer, sf::Color::Transparent,
                                                            sf::Color::Black, 1.f));

            std::size_t distance = 0;

            while (distance < 10)
            {
                if (findPlayer.intersects(playerRect) && _playerHero->mLife)
                {
                    (*it)->mIsAttacked = true;
                    break;
                }

                findPlayer.top += 30.f;

                ++distance;
            }
        }

        /// Closing the gates.
        if ((*it)->mTypeID == Type::ID::ClosingGate && !(*it)->mIsAttacked)
        {
            sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), 
                                     static_cast<float>((*it)->mHeight));
            
            size_t distance = 0;
            while (distance < 10) 
            {
                if (findPlayer.intersects(playerRect) && _playerHero->mLife)
                {
                    (*it)->mIsAttacked = true;
                    break;
                }

                findPlayer.top += 30.f;

                ++distance;
            }
        }

        /// Opening of the gates.
        if ((*it)->mTypeID == Type::ID::OpeningGate && !(*it)->mIsStarted)
        {
            if ((*it)->mType == "3" && _playerInfo->mQuests.at(PlayerInfo::Quest::TalkWithHeinrich))
            {
                (*it)->mIsStarted = true;
            }

            if ((*it)->mType == "4" && _playerHero->mActivatedGate)
            {
                (*it)->mIsStarted = true;
            }

            if ((*it)->mType == "5" && _playerInfo->mQuests.at(PlayerInfo::Quest::KillShadow))
            {
                (*it)->mIsStarted = true;
            }
        }

        ///    Damage on the first boss Shadow.
        if (_shadowBoss.mShadow->getRect().intersects((*it)->getRect()) &&
            (*it)->mTypeID == Type::ID::AlliedBullet && _shadowBoss.mShadow->mLife &&
            _shadowBoss.mShadow->mHitpoints > 0 && _shadowBoss.mIsActive &&
            _shadowBoss.mShadow->mIsStay)
        {
            _shadowBoss.mShadow->mHitpoints -= (*it)->mDamage;
            if (_shadowBoss.mShadow->mHitpoints <= 0)
            {
                _shadowBoss.mIsFinished = true;
            }
            (*it)->mLife = false;
        }

        /// Damage on the first mini-boss GolemDark.
        if (_golemBoss.mGolem->getRect().intersects((*it)->getRect()) &&
            (*it)->mTypeID == Type::ID::AlliedBullet && _golemBoss.mGolem->mLife &&
            _golemBoss.mGolem->mHitpoints > 0 && _golemBoss.mIsActive)
        {
            _golemBoss.mGolem->mHitpoints -= (*it)->mDamage;
            if (_golemBoss.mGolem->mHitpoints <= 0)
            {
                _golemBoss.mIsFinished = true;
            }
            (*it)->mLife = false;
        }

        /// If the rectangle of the sprite of the object intersects with the player...
        if ((*it)->getRect().intersects(playerRect))
        {
            /// If faced with Ghost...
            if ((*it)->mTypeID == Type::ID::Ghost)
            {
                if (_playerHero->mHitpoints > 0)
                {
                    if (!(*it)->mIsBack && ((_playerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
                        (_playerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
                    {
                        (*it)->mIsBack = true;
                        (*it)->dx = -(*it)->dx;
                        (*it)->mSprite.scale(-1.f, 1.f);
                    }

                    (*it)->mIsAttacked = true;
                    (*it)->mCurrentFrame = 0.f;
                    
                    if ((*it)->mIsHitted)
                    {
                        // Наносит урон
                        _playerHero->mHitpoints -= (*it)->mDamage;
                        // Игрок отскакивает от врага
                        _playerHero->dy = -0.1f;
                        //p.mSpeed = 0.5f * (*it)->dx;
                        //(*it)->isBack = false;
                        (*it)->mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                }
            }

            /// If faced with Golem...
            if ((*it)->mTypeID == Type::ID::Golem)
            {
                // Если голем активирован
                if (!(*it)->mIsStarted)
                {
                    (*it)->mIsStarted = true;
                }
                else if (_playerHero->mHitpoints > 0)
                {
                    if (!(*it)->mIsBack && ((_playerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
                        (_playerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
                    {
                        (*it)->mIsBack = true;
                        (*it)->dx = -(*it)->dx;
                    }
                    
                    (*it)->mIsAttacked = true;
                    (*it)->mCurrentFrame = 0.f;
            
                    if ((*it)->mIsHitted)
                    {
                        _playerHero->mHitpoints -= (*it)->mDamage;
                        (*it)->mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                }
            }

            /// If faced with DarkSoldier...
            if ((*it)->mTypeID == Type::ID::DarkSoldier)
            {
                if (_playerHero->mHitpoints > 0)
                {
                    if (!(*it)->mIsBack && ((_playerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
                        (_playerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
                    {
                        (*it)->mIsBack = true;
                        (*it)->dx = -(*it)->dx;
                    }
                    
                    // Враг останавливается
                    (*it)->mIsAttacked = true;
                    (*it)->mCurrentFrame = 0.f;
                    
                    if ((*it)->mIsHitted)
                    {
                        // Солдат телепортирует игрока
                        _playerHero->x = ((*it)->dx > 0 ? 1.2f : 0.8f) * (*it)->x;
                        // Игрок отскакивает от врага
                        _playerHero->y = 0.9f * (*it)->y;
                        _playerHero->dy = -0.25f;
                        (*it)->mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                    //p.x = 0.97f * ((*it)->dx > 0? (*it)->x + 60.f : (*it)->x);
                    //p.dy = -0.5f;    // Игрок отскакивает от врага
                }
            }

            /// If faced with Goblin...
            if ((*it)->mTypeID == Type::ID::Goblin)
            {
                if (_playerHero->mHitpoints > 0)
                {
                    if (!(*it)->mIsBack && ((_playerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
                        (_playerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
                    {
                        (*it)->mIsBack = true;
                        (*it)->dx = -(*it)->dx;
                    }
                    
                    (*it)->mIsAttacked = true;
                    (*it)->mCurrentFrame = 0.f;
                    
                    if ((*it)->mIsHitted)
                    {
                        _playerHero->mHitpoints -= (*it)->mDamage;
                        (*it)->mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                }
            }

            /// If faced with MinotaurMage...
            if ((*it)->mTypeID == Type::ID::MinotaurMage)
            {
                if (_playerHero->mHitpoints > 0)
                {
                    if (!(*it)->mIsBack && ((_playerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
                        (_playerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
                    {
                        (*it)->mIsBack = true;
                        (*it)->dx = -(*it)->dx;
                        (*it)->mSprite.scale(-1.f, 1.f);
                    }
                    
                    (*it)->mIsAttacked = true;
                    (*it)->mCurrentFrame = 0.f;
                    
                    if ((*it)->mIsHitted)
                    {
                        _playerHero->mHitpoints -= (*it)->mDamage;
                        (*it)->mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                }
            }

            /// If faced with Dwarf...
            if ((*it)->mTypeID == Type::ID::Dwarf)
            {
                // Если дварф не активирован
                if (!(*it)->mIsStarted)
                {
                    (*it)->mIsStarted = true;
                }
                else if (_playerHero->mHitpoints > 0)
                {
                    if (!(*it)->mIsBack && ((_playerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
                        (_playerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
                    {
                        (*it)->mIsBack = true;
                        (*it)->dx = -(*it)->dx;
                        (*it)->mSprite.scale(-1.f, 1.f);
                    }
                    
                    (*it)->mIsAttacked = true;
                    (*it)->mCurrentFrame = 0.f;
                    
                    if ((*it)->mIsHitted)
                    {
                        _playerHero->mHitpoints -= (*it)->mDamage;
                        (*it)->mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                }
            }

            /// If faced with DwarfArcher...
            if ((*it)->mTypeID == Type::ID::DwarfArcher)
            {
                // Если дварф-лучник не активирован
                if (!(*it)->mIsStarted)
                {
                    (*it)->mIsStarted = true;
                }
                else if (_playerHero->mHitpoints > 0)
                {
                    if (!(*it)->mIsBack && ((_playerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
                        (_playerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
                    {
                        (*it)->mIsBack = true;
                        (*it)->dx = -(*it)->dx;
                        (*it)->mSprite.scale(-1.f, 1.f);
                    }
                    
                    (*it)->mIsAttacked = true;
                    (*it)->mCurrentFrame = 0.f;
                    
                    if ((*it)->mIsHitted)
                    {
                        _playerHero->mHitpoints -= (*it)->mDamage;
                        (*it)->mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                }
            }

            /// If faced with DwarvenCommander...
            if ((*it)->mTypeID == Type::ID::DwarvenCommander)
            {
                // Если дварф не активирован
                if (!(*it)->mIsStarted)
                {
                    (*it)->mIsStarted = true;
                    // Создаём рядом ещё двух гномов обычного типа, если сагрили командира гномов
                    if (!(*it)->mIsSpawn)
                    {
                        if (_playerHero->dx >= 0.f)
                        {
                            std::unique_ptr<Entity> first_dwarf = std::make_unique<Dwarf>(
                                Type::ID::Dwarf, _textures, _fonts, *_level,
                                (*it)->x - 30.f, (*it)->y, 40, 27, 
                                (*it)->mType, Dwarf::DwarfType::Dwarf
                            );
                            first_dwarf->mIsStarted = true;
                            _entities.emplace_back(std::move(first_dwarf));

                            std::unique_ptr<Entity> second_dwarf = std::make_unique<Dwarf>(
                                Type::ID::DwarfArcher, _textures, _fonts, *_level,
                                (*it)->x + 30.f, (*it)->y, 40, 27,
                                (*it)->mType, Dwarf::DwarfType::DwarvenArcher
                            );
                            second_dwarf->mIsStarted = true;
                            second_dwarf->mIsEnabling = false;
                            second_dwarf->dx *= -1.f;
                            second_dwarf->mSprite.scale(-1.f, 1.f);
                            _entities.emplace_back(std::move(second_dwarf));
                        }
                        else
                        {
                            std::unique_ptr<Entity> first_dwarf = std::make_unique<Dwarf>(
                                Type::ID::Dwarf, _textures, _fonts, *_level,
                                (*it)->x - 30.f, (*it)->y, 40, 27, 
                                (*it)->mType, Dwarf::DwarfType::Dwarf);
                            first_dwarf->mIsStarted = true;
                            first_dwarf->dx *= -1.f;
                            first_dwarf->mSprite.scale(-1.f, 1.f);
                            _entities.emplace_back(std::move(first_dwarf));

                            std::unique_ptr<Entity> second_dwarf = std::make_unique<Dwarf>(
                                Type::ID::DwarfArcher, _textures, _fonts, *_level,
                                (*it)->x + 30.f, (*it)->y, 40, 27,
                                (*it)->mType, Dwarf::DwarfType::DwarvenArcher);
                            second_dwarf->mIsStarted = true;
                            second_dwarf->mIsEnabling = false;
                            _entities.emplace_back(std::move(second_dwarf));
                        }
                        (*it)->mIsSpawn = true;
                    }
                }
                else if (_playerHero->mHitpoints > 0)
                {
                    if (!(*it)->mIsBack && ((_playerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
                        (_playerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
                    {
                        (*it)->mIsBack = true;
                        (*it)->dx = -(*it)->dx;
                        (*it)->mSprite.scale(-1.f, 1.f);
                    }
                    
                    (*it)->mIsAttacked = true;
                    (*it)->mCurrentFrame = 0.f;
                    
                    if ((*it)->mIsHitted)
                    {
                        _playerHero->mHitpoints -= (*it)->mDamage;
                        (*it)->mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                }
            }

            /// If faced with Tentacle...
            if ((*it)->mTypeID == Type::ID::Tentacle)
            {
                // Если щупальце не активировано
                if (!(*it)->mIsStarted)
                {
                    (*it)->mIsStarted = true;
                }
                else if (_playerHero->mHitpoints > 0)
                {
                    (*it)->mIsAttacked = true;
                    
                    if ((*it)->mIsHitted)
                    {
                        _playerHero->mHitpoints -= (*it)->mDamage;
                        (*it)->mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                }
            }

            /// If faced with DarkArcher...
            if ((*it)->mTypeID == Type::ID::DarkArcher)
            {
                // Если тёмный лучник не активирован
                if (!(*it)->mIsEnd)
                {
                    (*it)->mIsStarted = true;

                    if (!(*it)->mIsBack && ((_playerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
                        (_playerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
                    {
                        (*it)->mIsBack = true;
                        (*it)->dx = -(*it)->dx;
                        (*it)->mSprite.scale(-1.f, 1.f);
                    }
                }
                else if (_playerHero->mHitpoints > 0)
                {
                    if (!(*it)->mIsBack && ((_playerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
                        (_playerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
                    {
                        (*it)->mIsBack = true;
                        (*it)->dx = -(*it)->dx;
                        (*it)->mSprite.scale(-1.f, 1.f);
                    }

                    (*it)->mIsAttacked = true;
                    (*it)->mCurrentFrame = 0.f;

                    if ((*it)->mIsHitted)
                    {
                        _playerHero->mHitpoints -= (*it)->mDamage;
                        (*it)->mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                }
            }

            /// If faced with Flamestrike...
            if ((*it)->mTypeID == Type::ID::Flamestrike)
            {
                if (_playerHero->mHitpoints > 0)
                {
                    //mPlayerHero->mHitpoints -= (*it)->mDamage;
                    //std::cout << "Hit\n";

                    (*it)->mIsAttacked = true;
                    
                    if ((*it)->mIsHitted)
                    {
                        _playerHero->mHitpoints -= (*it)->mDamage;
                        (*it)->mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                }
            }
        }

        /// If faced two objects: Player and EnemyBullet...
        if (playerRect.intersects((*it)->getRect()) && 
            (*it)->mTypeID == Type::ID::EnemyBullet && _playerHero->mHitpoints > 0)
        {
            _playerHero->mHitpoints -= (*it)->mDamage;
            (*it)->mLife = false;
        }

        /// If faced two objects: Player and MagicArrow...
        if (playerRect.intersects((*it)->getRect()) &&
            (*it)->mTypeID == Type::ID::MagicArrow && _playerHero->mHitpoints > 0)
        {
            _playerHero->mHitpoints -= (*it)->mDamage;
            (*it)->mLife = false;
        }

        /// If faced two objects: Player and Rock...
        if (playerRect.intersects((*it)->getRect()) && (*it)->mTypeID == Type::ID::Rock &&
            (*it)->mIsAttacked && !(*it)->mIsEnd && _playerHero->mHitpoints > 0)
        {
            _playerHero->mHitpoints = 0;
        }

        /// If faced two objects: Player and Gate...
        if (playerRect.intersects((*it)->getRect()) &&
            ((*it)->mTypeID == Type::ID::OpeningGate || (*it)->mTypeID == Type::ID::ClosingGate ||
            (*it)->mTypeID == Type::ID::OpenClosingGate)  && _playerHero->mHitpoints > 0)
        {
            if (((*it)->mTypeID == Type::ClosingGate || (*it)->mTypeID == Type::OpenClosingGate) &&
                (_playerHero->y <= (*it)->y + (*it)->mHeight) &&
                (_playerHero->y + _playerHero->mWidth >= (*it)->y + (*it)->mHeight))
            {
                _playerHero->mHitpoints = 0;
            }

            if (_playerHero->dx > 0.f)
            {
                _playerHero->x = (*it)->x - _playerHero->mWidth;
                _playerHero->dx = 0.f;
            }
            if (_playerHero->dx < 0.f)
            {
                _playerHero->x = (*it)->x + (*it)->mWidth;
                _playerHero->dx = 0.f;
            }
            else
            {
                _playerHero->x = (*it)->x + (_playerHero->mState == Player::State::Right ?
                                             -_playerHero->mWidth : (*it)->mWidth);
                _playerHero->dx = 0.f;
            }
        }

        /// If faced two objects: Player and DialogPerson...
        if (playerRect.intersects((*it)->getRect()) && ((*it)->mTypeID == Type::ID::Oswald ||
            (*it)->mTypeID == Type::ID::Heinrich) && !(*it)->mIsEnd &&
            _playerHero->mHitpoints > 0)
        {
            (*it)->mIsEnd = true;
            _playerHero->mDialogNumber = std::stoi((*it)->mType);

            switch ((*it)->mTypeID)
            {
                case Type::ID::Oswald:
                    _playerInfo->mQuests.at(PlayerInfo::Quest::TalkWithOswald) = true;
                    break;
                
                case Type::ID::Heinrich:
                    _playerInfo->mQuests.at(PlayerInfo::Quest::TalkWithHeinrich) = true;
                    break;
                
                default:
                    std::cout << "Invalid dialog person type\n";
                    break;
            }
        }

        /// Add a fallen stone in solid objects to check collisions.
        if ((*it)->mTypeID == Type::ID::Rock && (*it)->mIsEnd && !(*it)->mIsSpawn &&
            _playerHero->mHitpoints > 0 && !_golemBoss.mIsActive)
        {
            (*it)->mIsSpawn = true;

            // Temporary object.
            Object tempObject;
            tempObject.mName = "solid";
            tempObject.mType = (*it)->mType;
            tempObject.mSprite = (*it)->mSprite;

            sf::FloatRect objectRect;
            objectRect.top = (*it)->y;
            objectRect.left = (*it)->x;
            objectRect.height = static_cast<float>((*it)->mHeight);
            objectRect.width = static_cast<float>((*it)->mWidth);
            tempObject.mRect = objectRect;
            _level->mObjects.push_back(tempObject);
            _playerHero->mLevelObjects.push_back(std::move(tempObject));
        }

        /// Add the closed gate in the solid objects to check collisions.
        if ((*it)->mTypeID == Type::ID::ClosingGate && (*it)->mIsEnd && !(*it)->mIsSpawn &&
            _playerHero->mHitpoints > 0)
        {
            (*it)->mIsSpawn = true;

            // Temporary object.
            Object tempObject;
            tempObject.mName = "solid";
            tempObject.mType = (*it)->mType;
            tempObject.mSprite = (*it)->mSprite;

            sf::FloatRect objectRect;
            objectRect.top = (*it)->y;
            objectRect.left = (*it)->x;
            objectRect.height = static_cast<float>((*it)->mHeight);
            objectRect.width = static_cast<float>((*it)->mWidth);
            tempObject.mRect = objectRect;
            _level->mObjects.push_back(tempObject);
            _playerHero->mLevelObjects.push_back(std::move(tempObject));
        }

        /// Collision detection between objects.
        for (auto it2 = it; it2 != _entities.end(); ++it2)
        {
            // This must be different objects.
            if (*it != *it2)
            {
                auto entityRect = (*it)->getRect();
                auto objectRect = (*it2)->getRect();

                auto entityBodyRect = (*it)->getBodyRect();

                /// If faced two objects: Enemy and Rock...
                if (entityRect.intersects(objectRect) &&
                    (*it)->mTypeID != Type::ID::Rock && (*it2)->mTypeID == Type::ID::Rock &&
                    (*it2)->mIsAttacked && !(*it2)->mIsEnd && (*it)->mHitpoints > 0)
                {
                    (*it)->mHitpoints = 0;
                    (*it)->mIsStarted = true;
                    (*it)->mIsEnd = true;
                    (*it)->mIsSpawn = true;
                }

                /// If faced two objects: Bullet and Gate...
                if (entityRect.intersects(objectRect) &&
                    ((*it2)->mTypeID == Type::ID::AlliedBullet ||
                    (*it2)->mTypeID == Type::ID::EnemyBullet) &&
                        ((*it)->mTypeID == Type::ID::OpeningGate ||
                    (*it)->mTypeID == Type::ID::ClosingGate ||
                            (*it)->mTypeID == Type::ID::OpenClosingGate))
                {
                    (*it2)->mLife = false;
                }

                /// If faced two objects: Bullet and Rock...
                if (entityRect.intersects(objectRect) &&
                    ((*it2)->mTypeID == Type::ID::AlliedBullet ||
                    (*it2)->mTypeID == Type::ID::EnemyBullet) &&
                        (*it)->mTypeID == Type::ID::Rock && !_golemBoss.mIsActive)
                {
                    (*it2)->mLife = false;
                }

                /// If faced two objects: Ghost and Bullet...
                if (entityBodyRect.intersects(objectRect) &&
                    (*it)->mTypeID == Type::ID::Ghost &&
                    (*it2)->mTypeID == Type::ID::AlliedBullet && (*it)->mHitpoints > 0)
                {
                    (*it)->mHitpoints -= (*it2)->mDamage;
                    (*it2)->mLife = false;
                }

                /// If faced two objects: Golem and Bullet...
                if (entityBodyRect.intersects(objectRect) &&
                    (*it)->mTypeID == Type::ID::Golem &&
                    (*it2)->mTypeID == Type::ID::AlliedBullet &&
                    (*it)->mHitpoints > 0 && (*it)->mIsEnd)
                {
                    (*it)->mHitpoints -= (*it2)->mDamage;
                    (*it2)->mLife = false;
                }

                /// If faced two objects: Golem and Bullet...
                if (entityBodyRect.intersects(objectRect) &&
                    (*it)->mTypeID == Type::ID::DarkSoldier &&
                    (*it2)->mTypeID == Type::ID::AlliedBullet && (*it)->mHitpoints > 0)
                {
                    (*it)->mHitpoints -= (*it2)->mDamage;
                    (*it2)->mLife = false;
                }

                /// If faced two objects: Shadow and Bullet...
                if (entityBodyRect.intersects(objectRect) &&
                    (*it)->mTypeID == Type::ID::Shadow &&
                    (*it2)->mTypeID == Type::ID::AlliedBullet &&
                    (*it)->mHitpoints > 0 && (*it)->mIsEnd)
                {
                    (*it)->mHitpoints -= (*it2)->mDamage;
                    (*it2)->mLife = false;
                }

                /// If faced two objects: Goblin and Bullet...
                if (entityBodyRect.intersects(objectRect) &&
                    (*it)->mTypeID == Type::ID::Goblin &&
                    (*it2)->mTypeID == Type::ID::AlliedBullet && (*it)->mHitpoints > 0)
                {
                    (*it)->mHitpoints -= (*it2)->mDamage;
                    if ((*it)->mHitpoints <= 0)
                    {
                        _effects.emplace_back(
                            std::make_unique<Bloodsplat>(Type::ID::Bloodsplat, _textures,
                                                         (*it)->x, (*it)->y, 48, 24)
                        );
                    }

                    (*it2)->mLife = false;
                }

                /// If faced two objects: MinotaurMage and Bullet...
                if (entityBodyRect.intersects(objectRect) &&
                    (*it)->mTypeID == Type::ID::MinotaurMage &&
                    (*it2)->mTypeID == Type::ID::AlliedBullet && (*it)->mHitpoints > 0)
                {
                    (*it)->mHitpoints -= (*it2)->mDamage;
                    if ((*it)->mHitpoints <= 0)
                    {
                        _effects.emplace_back(
                            std::make_unique<Bloodsplat>(Type::ID::Bloodsplat, _textures,
                                                         (*it)->x + 7.f, (*it)->y + 7.f, 48, 24)
                        );
                    }

                    (*it2)->mLife = false;
                }

                /// If faced two objects: Dwarf and Bullet...
                if (entityBodyRect.intersects(objectRect) &&
                    (*it)->mTypeID == Type::ID::Dwarf &&
                    (*it2)->mTypeID == Type::ID::AlliedBullet && (*it)->mHitpoints > 0 &&
                    (*it)->mIsEnd)
                {
                    (*it)->mHitpoints -= (*it2)->mDamage;
                    if ((*it)->mHitpoints <= 0)
                    {
                        _effects.emplace_back(
                            std::make_unique<Bloodsplat>(Type::ID::Bloodsplat, _textures,
                                                         (*it)->x + 7.f, (*it)->y, 48, 24)
                        );
                    }
                    
                    (*it2)->mLife = false;
                }

                /// If faced two objects: DwarfArcher and Bullet...
                if (entityBodyRect.intersects(objectRect) &&
                    (*it)->mTypeID == Type::ID::DwarfArcher &&
                    (*it2)->mTypeID == Type::ID::AlliedBullet && (*it)->mHitpoints > 0 &&
                    (*it)->mIsEnd)
                {
                    (*it)->mHitpoints -= (*it2)->mDamage;
                    if ((*it)->mHitpoints <= 0)
                    {
                        _effects.emplace_back(
                            std::make_unique<Bloodsplat>(Type::ID::Bloodsplat, _textures,
                                                         (*it)->x, (*it)->y, 48, 24)
                        );
                    }

                    (*it2)->mLife = false;
                }

                /// If faced two objects: DwarvenCommander and Bullet...
                if (entityBodyRect.intersects(objectRect) &&
                    (*it)->mTypeID == Type::ID::DwarvenCommander &&
                    (*it2)->mTypeID == Type::ID::AlliedBullet && (*it)->mHitpoints > 0 &&
                    (*it)->mIsEnd)
                {
                    (*it)->mHitpoints -= (*it2)->mDamage;
                    if ((*it)->mHitpoints <= 0)
                    {
                        _effects.emplace_back(
                            std::make_unique<Bloodsplat>(Type::ID::Bloodsplat, _textures,
                                                         (*it)->x, (*it)->y, 48, 24)
                        );
                    }

                    (*it2)->mLife = false;

                    if ((*it)->mIsEnabling && (*it)->mHitpoints <= 0)
                    {
                        _playerInfo->mQuests.at(PlayerInfo::Quest::KillDwarvenCommanderM) = true;
                        _playerHero->mDialogNumber = 4;
                    }
                }

                /// If faced two objects: DarkArcher and Bullet...
                if (entityBodyRect.intersects(objectRect) &&
                    (*it)->mTypeID == Type::ID::DarkArcher &&
                    (*it2)->mTypeID == Type::ID::AlliedBullet && (*it)->mHitpoints > 0)
                {
                    (*it)->mHitpoints -= (*it2)->mDamage;
                    if ((*it)->mHitpoints <= 0)
                    {
                        _effects.emplace_back(
                            std::make_unique<Bloodsplat>(Type::ID::Bloodsplat, _textures,
                                                         (*it)->x + 11.f, (*it)->y + 22.f, 48, 24)
                        );
                    }

                    (*it2)->mLife = false;
                }
            }
        }
    }
}

void World::handleEvent()
{
        
}
