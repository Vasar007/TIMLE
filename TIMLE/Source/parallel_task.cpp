#include "parallel_task.hpp"


ParallelTask::ParallelTask(TextureHolder& textures, SoundBufferHolder& sounds,
                           PlayerInfo& playerInfo)
: _thread(&ParallelTask::loadTextures, this)
, _threadClock(&ParallelTask::runTask, this)
, _finished(false)
, _finishedLoading(false)
, _textures(textures)
, _sounds(sounds)
, _playerInfo(playerInfo)
{
}

void ParallelTask::execute()
{
    _finished = false;
    _finishedLoading = false;
    _elapsedTime.restart();
    _thread.launch();
    _threadClock.launch();
}

bool ParallelTask::isFinished()
{
    sf::Lock lock(_mutex);
    return _finished && _finishedLoading;
}

float ParallelTask::getCompletion()
{
    sf::Lock lock(_mutex);

    // 100% at 3 seconds of elapsed time.
    return _elapsedTime.getElapsedTime().asSeconds() / 3.f;
}

void ParallelTask::runTask()
{
    // Dummy task - stall 3 seconds.
    bool ended = false;
    while (!ended)
    {
        sf::Lock lock(_mutex); // Protect the clock.
        if (_elapsedTime.getElapsedTime().asSeconds() >= 3.f)
        {
            ended = true;
        }
    }

    // _finished may be accessed from multiple threads, protect it.
    {
        sf::Lock lock(_mutex);
        _finished = true;
    }    
}

void ParallelTask::loadTextures()
{
    sf::Lock lock(_mutex);
    if (!_playerInfo.mLoaded)
    {
        // Load textures.
        _textures.load(Textures::ID::Archer,          "Media/Textures/Player/male_lightT.png");
                                                      
        _textures.load(Textures::ID::Swordsman,       "Media/Textures/Player/male_heavy.png");
                                                      
        _textures.load(Textures::ID::Ghost,           "Media/Textures/Enemies/Ghost.png");
        _textures.load(Textures::ID::GhostDeath,      "Media/Textures/Enemies/Lightning.png");
                                                      
        _textures.load(Textures::ID::Golem,           "Media/Textures/Enemies/GolemWalk.png");
        _textures.load(Textures::ID::GolemAttack,     "Media/Textures/Enemies/GolemAtk.png");
        _textures.load(Textures::ID::GolemDeath,      "Media/Textures/Enemies/GolemDie.png");

        _textures.load(Textures::ID::GolemDark,       "Media/Textures/Enemies/GolemdarkWalk.png");
        _textures.load(Textures::ID::GolemDarkAttack, "Media/Textures/Enemies/GolemdarkAtk.png");
        _textures.load(Textures::ID::GolemDarkDeath,  "Media/Textures/Enemies/GolemdarkDie.png");

        _textures.load(Textures::ID::DarkSoldier,     "Media/Textures/Enemies/DarkSoldier.png");

        _textures.load(Textures::ID::Goblin,          "Media/Textures/Enemies/GoblinSword.png");

        _textures.load(Textures::ID::MinotaurMage,    "Media/Textures/Enemies/MinotaurMage.png");

        _textures.load(Textures::ID::Dwarf,           "Media/Textures/Enemies/Dwarves.png");

        _textures.load(Textures::ID::Shadow,          "Media/Textures/Enemies/Shadow.png");
        _textures.load(Textures::ID::Tentacle,        "Media/Textures/Enemies/Tentacles.png");

        _textures.load(Textures::ID::DarkArcher,      "Media/Textures/Enemies/DarkArcher.png");

        _textures.load(Textures::ID::Bullet,          "Media/Textures/Projectiles/MageBullet.png");
        _textures.load(Textures::ID::Fire,            "Media/Textures/Projectiles/Fire01.png");
        _textures.load(Textures::ID::MagicArrow,      "Media/Textures/Projectiles/MagicArrow.png");
        _textures.load(Textures::ID::MovingPlatform,  "Media/Textures/Objects/MovingPlatformCastle.png");
        _textures.load(Textures::ID::Quest,           "Media/Textures/Interface/MissionBg.jpg");

        _textures.load(Textures::ID::HealthBar,       "Media/Textures/Interface/RedBar.png");
        _textures.load(Textures::ID::ManaBar,         "Media/Textures/Interface/BlueBar.png");
        _textures.load(Textures::ID::EmptyBar,        "Media/Textures/Interface/EmptyBar.png");
        _textures.load(Textures::ID::BossBar,         "Media/Textures/Interface/healthbar.png");

        _textures.load(Textures::ID::RockDirt,        "Media/Textures/Objects/FallStoneDirt.png");
        _textures.load(Textures::ID::RockCastle,      "Media/Textures/Objects/FallingStoneCastle.png");
        _textures.load(Textures::ID::RockCristal,     "Media/Textures/Objects/FallingStoneCristal.png");
        _textures.load(Textures::ID::RockDarkStone,   "Media/Textures/Objects/FallingStoneDarkRock.png");
        _textures.load(Textures::ID::RockHellStone,   "Media/Textures/Objects/FallingStoneHellRock.png");

        _textures.load(Textures::ID::GateDirt,        "Media/Textures/Objects/GateDirt.png");
        _textures.load(Textures::ID::GateCristal,     "Media/Textures/Objects/GateCristal.png");
        _textures.load(Textures::ID::GateDarkRock,    "Media/Textures/Objects/GateDarkRock.png");
        _textures.load(Textures::ID::GateHellRock,    "Media/Textures/Objects/GateHellRock.png");
        _textures.load(Textures::ID::GateWood,        "Media/Textures/Objects/GateWood.png");

        _textures.load(Textures::ID::Knight,          "Media/Textures/Objects/Knights.png");

        _textures.load(Textures::ID::Bloodsplat,      "Media/Textures/Effects/Bloodsplat.png");

        // Load sounds.
        _sounds.load(Sounds::ID::Bullet,              "Media/Sounds/firebol3.ogg");

        _playerInfo.mLoaded = true;
    }

    _finishedLoading = true;
}
