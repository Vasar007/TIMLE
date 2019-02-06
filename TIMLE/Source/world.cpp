#include <limits>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "bloodsplat.hpp"
#include "bullet.hpp"
#include "dark_archer.hpp"
#include "dark_soldier.hpp"
#include "dead_man.hpp"
#include "dialog_person.hpp"
#include "dwarf.hpp"
#include "flamestrike.hpp"
#include "gate.hpp"
#include "ghost.hpp"
#include "golem.hpp"
#include "goblin.hpp"
#include "magic_arrow.hpp"
#include "minotaur_mage.hpp"
#include "moving_platform.hpp"
#include "projectile.hpp"
#include "rock.hpp"
#include "shadow.hpp"
#include "tentacle.hpp"
#include "utility.hpp"

#include "world.hpp"


World::World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
             SoundBufferHolder& sounds, PlayerInfo& playerInfo, AudioManager& audioManager, 
             const State::DebugMode debugMode, const std::size_t levelNumber)
: _window(window)
, _worldView(window.getDefaultView())
, _textures(textures)
, _fonts(fonts)
, _sounds(sounds)
, _audioManager(audioManager)
, _level(std::make_unique<Level>())
, _currentLevelNumber(levelNumber)
, _playerHero(nullptr)
, _playerInfo(playerInfo)
, _lifeBar(nullptr)
, _debug(debugMode == State::DebugMode::DebugOn)
, _worldContext(textures, fonts, levelNumber, *_level, _debug)
{
    _sound.setBuffer(sounds.get(Sounds::ID::Bullet));

    audioManager.setMusic(Music::ID::FirstMainMusic);

    buildScene();
    _worldView.reset(sf::FloatRect(0, 0, 1280, 720));
    _worldView.zoom(0.5f);
}

void World::loadLevel()
{
    std::string level_path;
    std::string tile_sheet_path;
    std::string error_message;

    switch (_currentLevelNumber)
    {
        case 1:
            level_path = "Levels/Level1.tmx";
            tile_sheet_path = "Levels/";
            error_message = "Error ocured during level 1 loading.";
            break;

        case 2:
            level_path = "Levels/Level2.tmx";
            tile_sheet_path = "Levels/";
            error_message = "Error ocured during level 2 loading.";
            break;

        case 3:
            level_path = "Levels/Level3.tmx";
            tile_sheet_path = "Levels/";
            error_message = "Error ocured during level 3 loading.";
            break;

        case 4:
            level_path = "Levels/test_map.tmx"; // Test level for Vasilyev.
            tile_sheet_path = "Levels/";
            error_message = "Error ocured during test level loading.";
            break;

        case 5:
            level_path = "Levels/supertest_map.tmx"; // Test level for Gusev.
            tile_sheet_path = "Levels/";
            error_message = "Error ocured during AI test level loading.";
            break;

        default:
            std::cerr << "Error! Out of range level.\n";
            throw std::invalid_argument(std::to_string(_currentLevelNumber));
    }

    if (!_level->loadFromFile(level_path, tile_sheet_path))
    {
        throw std::invalid_argument(error_message);
    }
}

std::size_t World::getLevelNumber() const
{
    return _currentLevelNumber;
}

void World::update(sf::Time dt)
{
    // Pass the coordinates of the player in the camera control feature.
    setPlayerCoordinateForView(_playerHero->x, _playerHero->y);

    for (auto it = _entities.begin(); it != _entities.end();)
    {
        // Call the update function for objects.
        (*it)->update(static_cast<float>(dt.asMilliseconds()));
        if ((*it)->mLife)
        {
            ++it;
        }
        else
        {
            it = _entities.erase(it);
        }
    }

    for (auto it = _effects.begin(); it != _effects.end();)
    {
        (*it)->update(static_cast<float>(dt.asMilliseconds()));
        if ((*it)->alive)
        {
            ++it;
        }
        else
        {
            it = _effects.erase(it);
        }
    }

    for (auto it = _guidedProjectiles.begin(); it != _guidedProjectiles.end();)
    {
        (*it)->update(static_cast<float>(dt.asMilliseconds()));
        if ((*it)->mLife)
        {
            ++it;
        }
        else
        {
            it = _guidedProjectiles.erase(it);
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

    // Process player's transition.
    if (_playerInfo.mNeedTransit)
    {
        _playerInfo.mDoTransit = true;
        _playerInfo.mNeedTransit = false;

        Object otherDoor;
        for (const auto& door : _doors)
        {
            if (door.mId != _playerInfo.mNumberOfDoor.first &&
                door.mType == _playerInfo.mNumberOfDoor.second)
            {
                otherDoor = door;
                break;
            }
        }
        _playerInfo.mTransitPos = { otherDoor.mRect.left, otherDoor.mRect.top };
        _playerInfo.mTransiting = true;
    }

    _worldContext.update(dt);

    // Updates player's lifebar.
    _lifeBar->update(_playerHero->mHitpoints);

    _playerHero->update(static_cast<float>(dt.asMilliseconds()));

    // Checks collision between different objects.
    handleCollisions(static_cast<float>(dt.asMilliseconds()));
    guideMissiles();

    _playerInfo.mDialogNumber = 0;
    if (_playerHero->mDialogNumber == 0)
    {
        _playerInfo.mDialogNumber = 0;
    }
    else
    {
        _playerInfo.showDialog(_playerHero->mDialogNumber);
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
                    const auto shape = utils::buildBorderLines(
                        entity->getRect(), sf::Color::Transparent, sf::Color::Red, 1.f
                    );
                    _window.draw(shape);

                    const auto bodyShape = utils::buildBorderLines(
                        entity->getBodyRect(), sf::Color::Transparent, sf::Color::Black, 1.f
                    );
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
                    const auto shape = utils::buildBorderLines(
                        entity->getRect(), sf::Color::Transparent, sf::Color::Yellow, 1.f
                    );
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
                    const auto shape = utils::buildBorderLines(
                        entity->getRect(), sf::Color::Transparent, sf::Color::Magenta, 1.f
                    );
                    _window.draw(shape);
                    break;
                }

                case Type::ID::AlliedBullet:
                    [[fallthrough]];
                case Type::ID::EnemyBullet:
                    [[fallthrough]];
                case Type::ID::Flamestrike:
                {
                    const auto shape = utils::buildBorderLines(
                        entity->getRect(), sf::Color::Transparent, sf::Color::Red, 1.f
                    );
                    _window.draw(shape);
                    break;
                }

                default:
                {
                    const auto shape = utils::buildBorderLines(
                        entity->getRect(), sf::Color::Transparent, sf::Color::White, 1.f
                    );
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
        if (_debug)
        {
            const auto shape = utils::buildBorderLines(
                effect->get_rect(), sf::Color::Transparent, sf::Color::White, 1.f
            );
            _window.draw(shape);
        }

        if (viewRect.contains(effect->x, effect->y))
        {
            effect->draw(_window);
        }
    }

    for (const auto& missile : _guidedProjectiles)
    {
        if (_debug)
        {
            const auto shape = utils::buildBorderLines(
                missile->getRect(), sf::Color::Transparent, sf::Color::Red, 1.f);
            _window.draw(shape);
        }

        if (viewRect.contains(missile->x, missile->y))
        {
            missile->draw(_window);
        }
    }

    _worldContext.draw(_window);
    
    if (_debug)
    {
        for (const auto& object : _objects)
        {
            if (object.mName == "solid")
            {
                const auto shape = utils::buildBorderLines(
                    object.mRect, sf::Color::Transparent, sf::Color::Black, 1.f
                );
                _window.draw(shape);
            }
            else if (object.mName == "enemyBorder")
            {
                const auto shape = utils::buildBorderLines(
                    object.mRect, sf::Color::Transparent, sf::Color::Red, 1.f
                );
                _window.draw(shape);
            }
            else if (object.mName == "death")
            {
                const auto shape = utils::buildBorderLines(
                    object.mRect, sf::Color::Transparent, sf::Color::Magenta, 1.f
                );
                _window.draw(shape);
            }
            else if (object.mName == "end")
            {
                const auto shape = utils::buildBorderLines(
                    object.mRect, sf::Color::Transparent, sf::Color::White, 1.f
                );
                _window.draw(shape);
            }
            else if (object.mName == "boss")
            {
                const auto shape = utils::buildBorderLines(
                    object.mRect, sf::Color::Transparent, sf::Color::Red, 1.f
                );
                _window.draw(shape);
            }
            else if (object.mName == "dialogMessage")
            {
                const auto shape = utils::buildBorderLines(
                    object.mRect, sf::Color::Transparent, sf::Color::Blue, 1.f
                );
                _window.draw(shape);
            }
            else if (object.mName == "door")
            {
                const auto shape = utils::buildBorderLines(
                    object.mRect, sf::Color::Transparent, sf::Color::Cyan, 1.f
                );
                _window.draw(shape);
            }
        }

        const auto shape = utils::buildBorderLines(
            _playerHero->getRect(), sf::Color::Transparent, sf::Color::Green, 1.f
        );
        _window.draw(shape);
    }

    if (_debug)
    {
        for (const auto& rect : _debugRectsToDraw)
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

void World::setPlayerCoordinateForView(const float x, const float y)
{
    float tempX = x;
    float tempY = y;

    switch (_currentLevelNumber)
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
            throw std::invalid_argument(std::to_string(_currentLevelNumber));
    }

    _worldView.setCenter(tempX, tempY);

    if (_currentLevelNumber == 1)
    {
        _worldContext.processGolemDarkEvents(_worldView, _entities);
    }
}

void World::buildScene()
{
    // Initialization of level.
    loadLevel();

    _worldContext.buildBossData();

    // Add player.
    const auto playerObj = _level->getObject("player");
    _playerHero = std::make_unique<Player>(
        Type::ID::Archer, _textures, _fonts, *_level, playerObj.mRect.left, playerObj.mRect.top,
        20, 30, _playerInfo
    );

    _playerInfo.setPlayer(_playerHero.get());

    _lifeBar = std::make_unique<LifeBar>(
        Type::ID::HealthBar, _textures, _fonts, _playerHero->mHitpoints
    );

    _playerInfo.mLastSavePoint.x = _playerHero->x;
    _playerInfo.mLastSavePoint.y = _playerHero->y;

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
    utils::append(_objects, _level->getObjects("solid"));
    utils::append(_objects, _level->getObjects("enemyBorder"));  
    utils::append(_objects, _level->getObjects("death"));
    utils::append(_objects, _level->getObjects("end"));
    utils::append(_objects, _level->getObjects("boss"));
    utils::append(_objects, _level->getObjects("dialogMessage"));
    utils::append(_objects, _level->getObjects("door"));
    _doors = _level->getObjects("door");
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

    _worldContext.processMainEvents(_playerInfo, _audioManager);

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
                for (const auto& object : _objects)
                {
                    if (findPlayer.intersects(object.mRect) && object.mName == "solid")
                    {
                        isNeedFind = false;
                        break;
                    }
                }

                ++distance;
            }

            _debugRectsToDraw.emplace_back(utils::buildBorderLines(
                findPlayer, sf::Color::Transparent, sf::Color::Black, 1.f
            ));

            if ((*it)->mIsAttacked)
            {
                (*it)->mCurrentFrame = 0.f;

                if ((*it)->mIsHitted)
                {
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
                for (const auto& object :_objects)
                {
                    if (findPlayer.intersects(object.mRect) && object.mName == "solid")
                    {
                        isNeedFind = false;
                        break;
                    }
                }

                ++distance;
            }

            _debugRectsToDraw.emplace_back(utils::buildBorderLines(
                findPlayer, sf::Color::Transparent, sf::Color::Black, 1.f
            ));

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
                //for (const auto& object :_objects)
                //{
                //  if (findPlayer.intersects(object.mRect) && object.mName == "solid")
                //  {
                //      isNeedFind = false;
                //      break;
                //  }
                //

                ++distance;
            }

            _debugRectsToDraw.emplace_back(utils::buildBorderLines(
                findPlayer, sf::Color::Transparent, sf::Color::Black, 1.f
            ));

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
                    _guidedProjectiles.emplace_back(std::move(bullet));
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

            _debugRectsToDraw.emplace_back(
                utils::buildBorderLines(findPlayer, sf::Color::Transparent, sf::Color::Black, 1.f
            ));

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
            if ((*it)->mType == "3" && _playerInfo.mQuests.at(PlayerInfo::Quest::TalkWithOswald))
            {
                (*it)->mIsStarted = true;
            }

            if ((*it)->mType == "4" && _playerHero->mActivatedGate)
            {
                (*it)->mIsStarted = true;
            }

            if ((*it)->mType == "5" && _playerInfo.mQuests.at(PlayerInfo::Quest::KillShadow))
            {
                (*it)->mIsStarted = true;
            }
        }

        /// Damage on the first boss Shadow.
        if ((*it)->mTypeID == Type::ID::AlliedBullet)
        {
            _worldContext.handleCollisions(**it);
        }

        /// Damage on the first mini-boss GolemDark.
        if ((*it)->mTypeID == Type::ID::AlliedBullet)
        {
            _worldContext.handleCollisions(**it);
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
                        _playerHero->mHitpoints -= (*it)->mDamage;
                        // The player bounces off the enemy.
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
                // If the Golem is not activated.
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

                    (*it)->mIsAttacked = true;
                    (*it)->mCurrentFrame = 0.f;

                    if ((*it)->mIsHitted)
                    {
                        // Soldier teleports the player.
                        _playerHero->x = ((*it)->dx > 0 ? 1.2f : 0.8f) * (*it)->x;
                        // The player bounces off the enemy.
                        _playerHero->y = 0.9f * (*it)->y;
                        _playerHero->dy = -0.25f;
                        (*it)->mIsHitted = false;
                        std::cout << "Hit\n";
                    }
                    //p.x = 0.97f * ((*it)->dx > 0? (*it)->x + 60.f : (*it)->x);
                    //p.dy = -0.5f;    // The player bounces off the enemy.
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
                // If the Dwarf is not activated.
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
                // If the DwarfArcher is not activated.
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
                // If the DwarvenCommander is not activated.
                if (!(*it)->mIsStarted)
                {
                    (*it)->mIsStarted = true;
                    // Next create two more dwarves conventional type, if aggroes dwarven commander.
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
                // If the Tentacle is not activated.
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
                // If the DarkArcher is not activated.
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
            if (((*it)->mTypeID == Type::ID::ClosingGate || (*it)->mTypeID == Type::ID::OpenClosingGate) &&
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
                    _playerInfo.mQuests.at(PlayerInfo::Quest::TalkWithOswald) = true;
                    break;
                case Type::ID::Heinrich:
                    _playerInfo.mQuests.at(PlayerInfo::Quest::TalkWithHeinrich) = true;
                    break;
                default:
                    std::cout << "Invalid dialog person type\n";
                    break;
            }
        }

        /// Add a fallen stone in solid objects to check collisions.
        if ((*it)->mTypeID == Type::ID::Rock && (*it)->mIsEnd && !(*it)->mIsSpawn &&
            _playerHero->mHitpoints > 0 && !_worldContext.isGolemDarkActive())
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
            _playerHero->mLevelObjects.emplace_back(std::move(tempObject));
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
            _playerHero->mLevelObjects.emplace_back(std::move(tempObject));
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
                    (*it)->mTypeID == Type::ID::Rock && !_worldContext.isGolemDarkActive())
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
                            std::make_unique<Bloodsplat>(Type::ID::Bloodsplat,
                                                         _textures.get(Textures::ID::Bloodsplat),
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
                            std::make_unique<Bloodsplat>(Type::ID::Bloodsplat,
                                                         _textures.get(Textures::ID::Bloodsplat),
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
                            std::make_unique<Bloodsplat>(Type::ID::Bloodsplat,
                                                         _textures.get(Textures::ID::Bloodsplat),
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
                            std::make_unique<Bloodsplat>(Type::ID::Bloodsplat,
                                                         _textures.get(Textures::ID::Bloodsplat),
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
                            std::make_unique<Bloodsplat>(Type::ID::Bloodsplat,
                                                         _textures.get(Textures::ID::Bloodsplat),
                                                         (*it)->x, (*it)->y, 48, 24)
                        );
                    }

                    (*it2)->mLife = false;

                    if ((*it)->mIsEnabling && (*it)->mHitpoints <= 0)
                    {
                        _playerInfo.mQuests.at(PlayerInfo::Quest::KillDwarvenCommanderM) = true;
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
                            std::make_unique<Bloodsplat>(Type::ID::Bloodsplat,
                                                         _textures.get(Textures::ID::Bloodsplat),
                                                         (*it)->x + 11.f, (*it)->y + 22.f, 48, 24)
                        );
                    }

                    (*it2)->mLife = false;
                }
            }
        }
    }

    for (const auto& missile : _guidedProjectiles)
    {
        /// If faced two objects: Player and MagicArrow...
        if (playerRect.intersects(missile->getRect()) &&
            missile->mTypeID == Type::ID::MagicArrow && _playerHero->mHitpoints > 0)
        {
            _playerHero->mHitpoints -= missile->mDamage;
            missile->mLife = false;
        }
    }
}

void World::handleEvent()
{
}
