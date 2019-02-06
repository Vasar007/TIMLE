#include "parallel_task.hpp"


ParallelTask::ParallelTask(TextureHolder& textures, SoundBufferHolder& sounds,
                           PlayerInfo* playerInfo)
: mFinished(false)
, mFinishedLoading(false)
, mElapsedTime()
, mMutex()
, mutex_()
, mTextures(textures)
, mSounds(sounds)
, mPlayerInfo(playerInfo)
//, mThread(&ParallelTask::loadTextures, this)
//, mThreadClock(&ParallelTask::runTask, this)
{
}

void ParallelTask::execute()
{
    mFinished.store(false);
    mFinishedLoading.store(false);

        mThread = std::thread(&ParallelTask::loadTextures, this);
    mThreadClock = std::thread(&ParallelTask::runTask, this);

    mThread.detach();    // launch()
    mThreadClock.detach();

    mElapsedTime.restart();
}

bool ParallelTask::isFinished()
{
    //std::unique_lock<std::mutex> locker(mMutex);
    std::shared_lock<std::shared_mutex> lock(mutex_);

    return mFinished.load() && mFinishedLoading.load();
}

float ParallelTask::getCompletion()
{
    //std::lock_guard<std::mutex> locker(mMutex);
    std::shared_lock<std::shared_mutex> lock(mutex_);

    // 100% at 3 seconds of elapsed time.
    return mElapsedTime.getElapsedTime().asSeconds() / 5.f;
}

void ParallelTask::runTask()
{
    // Dummy task - stall 3 seconds.
    bool ended = false;
    while (!ended)
    {
        //std::lock_guard<std::mutex> locker(mMutex); // Protect the clock.
        std::shared_lock<std::shared_mutex> lock(mutex_);
        if (mElapsedTime.getElapsedTime().asSeconds() >= 5.f)
        {
            ended = true;
        }
    }

    // mFinished may be accessed from multiple threads, protect it.
    {
        //std::lock_guard<std::mutex> locker(mMutex);
        std::shared_lock<std::shared_mutex> lock(mutex_);
        mFinished.store(true);
    }    
}

void ParallelTask::loadTextures()
{
    //std::lock_guard<std::mutex> locker(mMutex);    //sf::Lock lock(mMutex);
    std::unique_lock<std::shared_mutex> lock(mutex_);
    if (!mPlayerInfo->mLoaded)
    {
        mTextures.load(Textures::ID::Archer,            "Media/Textures/Player/male_lightT.png");

        mTextures.load(Textures::ID::Swordsman,            "Media/Textures/Player/male_heavy.png");

        mTextures.load(Textures::ID::Ghost,                "Media/Textures/Enemies/Ghost.png");
        mTextures.load(Textures::ID::GhostDeath,        "Media/Textures/Enemies/Lightning.png");

        mTextures.load(Textures::ID::Golem,                "Media/Textures/Enemies/GolemWalk.png");
        mTextures.load(Textures::ID::GolemAttack,        "Media/Textures/Enemies/GolemAtk.png");
        mTextures.load(Textures::ID::GolemDeath,        "Media/Textures/Enemies/GolemDie.png");

        mTextures.load(Textures::ID::GolemDark,            "Media/Textures/Enemies/GolemdarkWalk.png");
        mTextures.load(Textures::ID::GolemDarkAttack,    "Media/Textures/Enemies/GolemdarkAtk.png");
        mTextures.load(Textures::ID::GolemDarkDeath,    "Media/Textures/Enemies/GolemdarkDie.png");

        mTextures.load(Textures::ID::DarkSoldier,        "Media/Textures/Enemies/DarkSoldier.png");

        mTextures.load(Textures::ID::Goblin,            "Media/Textures/Enemies/GoblinSword.png");

        mTextures.load(Textures::ID::MinotaurMage,        "Media/Textures/Enemies/MinotaurMage.png");

        mTextures.load(Textures::ID::Dwarf,                "Media/Textures/Enemies/Dwarves.png");

        mTextures.load(Textures::ID::Shadow,            "Media/Textures/Enemies/Shadow.png");
        mTextures.load(Textures::ID::Tentacle,            "Media/Textures/Enemies/Tentacles.png");

        mTextures.load(Textures::ID::DarkArcher,        "Media/Textures/Enemies/DarkArcher.png");

        mTextures.load(Textures::ID::Bullet,            "Media/Textures/Projectiles/MageBullet.png");
        mTextures.load(Textures::ID::Fire,                "Media/Textures/Projectiles/Fire01.png");
        mTextures.load(Textures::ID::MagicArrow,        "Media/Textures/Projectiles/MagicArrow.png");
        mTextures.load(Textures::ID::MovingPlatform,    "Media/Textures/Objects/MovingPlatform.png");
        mTextures.load(Textures::ID::Quest,                "Media/Textures/Interface/MissionBg.jpg");

        mTextures.load(Textures::ID::HealthBar,            "Media/Textures/Interface/RedBar.png");
        mTextures.load(Textures::ID::ManaBar,            "Media/Textures/Interface/BlueBar.png");
        mTextures.load(Textures::ID::EmptyBar,            "Media/Textures/Interface/EmptyBar.png");
        mTextures.load(Textures::ID::BossBar,            "Media/Textures/Interface/healthbar.png");

        mTextures.load(Textures::ID::RockDirt,            "Media/Textures/Objects/FallStoneDirt.png");
        mTextures.load(Textures::ID::RockCastle,        "Media/Textures/Objects/FallingStoneCastle.png");
        mTextures.load(Textures::ID::RockCristal,        "Media/Textures/Objects/FallingStoneCristal.png");
        mTextures.load(Textures::ID::RockDarkStone,        "Media/Textures/Objects/FallingStoneDarkRock.png");
        mTextures.load(Textures::ID::RockHellStone,        "Media/Textures/Objects/FallingStoneHellRock.png");

        mTextures.load(Textures::ID::GateDirt,            "Media/Textures/Objects/GateDirt.png");
        mTextures.load(Textures::ID::GateCristal,        "Media/Textures/Objects/GateCristal.png");
        mTextures.load(Textures::ID::GateDarkRock,        "Media/Textures/Objects/GateDarkRock.png");
        mTextures.load(Textures::ID::GateHellRock,        "Media/Textures/Objects/GateHellRock.png");
        mTextures.load(Textures::ID::GateWood,            "Media/Textures/Objects/GateWood.png");

        mTextures.load(Textures::ID::Knight,            "Media/Textures/Objects/Knights.png");


        mSounds.load(Sounds::ID::Bullet,        "Media/Sounds/firebol3.ogg");

        mPlayerInfo->mLoaded = true;
    }

    mFinishedLoading.store(true);
}
