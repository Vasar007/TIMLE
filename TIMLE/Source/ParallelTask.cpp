#include "../Include/ParallelTask.hpp"


ParallelTask::ParallelTask(TextureHolder& textures, SoundBufferHolder& sounds, PlayerInfo* playerInfo)
: mThread(&ParallelTask::loadTextures, this)
, mThreadClock(&ParallelTask::runTask, this)
, mFinished(false)
, mFinishedLoading(false)
, mElapsedTime()
, mMutex()
, mTextures(textures)
, mSounds(sounds)
, mPlayerInfo(playerInfo)
{
}

void ParallelTask::execute()
{
	mFinished = false;
	mFinishedLoading = false;
	mElapsedTime.restart();
	mThread.launch();
	mThreadClock.launch();
}

bool ParallelTask::isFinished()
{
	sf::Lock lock(mMutex);
	return mFinished && mFinishedLoading;
}

float ParallelTask::getCompletion()
{
	sf::Lock lock(mMutex);

	// 100% at 3 seconds of elapsed time.
	return mElapsedTime.getElapsedTime().asSeconds() / 3.f;
}

void ParallelTask::runTask()
{
	// Dummy task - stall 3 seconds.
	bool ended = false;
	while (!ended)
	{
		sf::Lock lock(mMutex); // Protect the clock.
		if (mElapsedTime.getElapsedTime().asSeconds() >= 3.f)
			ended = true;
	}

	{ // mFinished may be accessed from multiple threads, protect it.
		sf::Lock lock(mMutex);
		mFinished = true;
	}	
}

void ParallelTask::loadTextures()
{
	sf::Lock lock(mMutex);
	if (!mPlayerInfo->mLoaded)
	{
		mTextures.load(Textures::Archer,			"Media/Textures/Player/male_lightT.png");

		mTextures.load(Textures::Swordsman,			"Media/Textures/Player/male_heavy.png");

		mTextures.load(Textures::Ghost,				"Media/Textures/Enemies/ghost.png");
		mTextures.load(Textures::GhostDeath,		"Media/Textures/Enemies/lightning.png");

		mTextures.load(Textures::Golem,				"Media/Textures/Enemies/golem-walk.png");
		mTextures.load(Textures::GolemAttack,		"Media/Textures/Enemies/golem-atk.png");
		mTextures.load(Textures::GolemDeath,		"Media/Textures/Enemies/golem-die.png");

		mTextures.load(Textures::GolemDark,			"Media/Textures/Enemies/golemdark-walk.png");
		mTextures.load(Textures::GolemDarkAttack,	"Media/Textures/Enemies/golemdark-atk.png");
		mTextures.load(Textures::GolemDarkDeath,	"Media/Textures/Enemies/golemdark-die.png");

		mTextures.load(Textures::DarkSoldier,		"Media/Textures/Enemies/darksoldiersheetupdate.png");

		mTextures.load(Textures::Goblin,			"Media/Textures/Enemies/goblinsword.png");

		mTextures.load(Textures::MinotaurMage,		"Media/Textures/Enemies/gnu-120x100.png");

		mTextures.load(Textures::Dwarf,				"Media/Textures/Enemies/dwarves.png");

		mTextures.load(Textures::Shadow,			"Media/Textures/Enemies/shadow-80x70.png");
		mTextures.load(Textures::Tentacle,			"Media/Textures/Enemies/tentacles-25x90.png");

		mTextures.load(Textures::Bullet,			"Media/Textures/Objects/mage-bullet-13x13.png");
		mTextures.load(Textures::Fire,				"Media/Textures/Objects/Fire01.png");
		mTextures.load(Textures::MovingPlatform,	"Media/Textures/Objects/MovingPlatform.png");
		mTextures.load(Textures::Quest,				"Media/Textures/Interface/missionbg.jpg");

		mTextures.load(Textures::HealthBar,			"Media/Textures/Interface/RedBar.png");
		mTextures.load(Textures::ManaBar,			"Media/Textures/Interface/BlueBar.png");
		mTextures.load(Textures::EmptyBar,			"Media/Textures/Interface/EmptyBar.png");
		mTextures.load(Textures::BossBar,			"Media/Textures/Interface/healthbar.png");

		mTextures.load(Textures::RockDirt,			"Media/Textures/Objects/fallStoneDirt.png");
		mTextures.load(Textures::RockCastle,		"Media/Textures/Objects/fallingStoneCastle.png");
		mTextures.load(Textures::RockCristal,		"Media/Textures/Objects/fallingStoneCristal.png");
		mTextures.load(Textures::RockDarkStone,		"Media/Textures/Objects/fallingStoneDarkRock.png");
		mTextures.load(Textures::RockHellStone,		"Media/Textures/Objects/fallingStoneHellRock.png");

		mTextures.load(Textures::GateDirt,			"Media/Textures/Objects/gateDirt.png");
		mTextures.load(Textures::GateCristal,		"Media/Textures/Objects/gateCristal.png");
		mTextures.load(Textures::GateDarkRock,		"Media/Textures/Objects/gateDarkRock.png");
		mTextures.load(Textures::GateHellRock,		"Media/Textures/Objects/gateHellRock.png");
		mTextures.load(Textures::GateWood,			"Media/Textures/Objects/gateWood.png");

		mTextures.load(Textures::Knight,			"Media/Textures/Objects/Knights.png");


		mSounds.load(Sounds::Bullet,		"Media/Sounds/firebol3.ogg");

		mPlayerInfo->mLoaded = true;
	}

	mFinishedLoading = true;
}
