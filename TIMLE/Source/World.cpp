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

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <limits>


World::World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
			 SoundBufferHolder& sounds, PlayerInfo* playerInfo, AudioManager& audioManager)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures(textures)
, mFonts(fonts)
, mSounds(sounds)
, mAudioManager(audioManager)
, mLevel(nullptr)
, mWorldBounds(0.f, 0.f, 13072.f, 3200.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mLevelNumber(4)
, mPosition()
, mScrollSpeed(0.f)
, mPlayerHero(nullptr)
, mPlayerInfo(playerInfo)
, mLifeBar(nullptr)
, mShadowBoss()
, mGolemBoss()
, mEnemySpawnPoints()
, mEnemies()
, mObjects()
, mSound()
, mDebug(true)
{
	srand(static_cast<unsigned int>(time(nullptr))); // Need to replace with RandomEndigne.

	mSound.setBuffer(mSounds.get(Sounds::Bullet));

	mAudioManager.setMusic(AudioManager::FirstMainMusic);

	buildScene();
	mWorldView.reset(sf::FloatRect(0, 0, 1280, 720));
	mWorldView.zoom(0.5f);
}

void World::loadLevel(size_t levelNumber)
{
	mLevelNumber = levelNumber;

	switch (mLevelNumber)
	{
		case 1:
			mLevel->loadFromFile("Level1.tmx"); // Need to reload into Levels/
			break;
		
		case 2:
			mLevel->loadFromFile("Level2.tmx");
			break;
		
		case 3:
			mLevel->loadFromFile("Level3.tmx");
			break;
		
		case 4:
			mLevel->loadFromFile("test_map.tmx");	// Test level for Vasilyev.
			break;
		
		case 5:
			mLevel->loadFromFile("supertest_map.tmx");	// Test level for Gusev.
			break;
		
		default:
			std::cout << "Error! Out of range level." << std::endl;
			mLevelNumber = 1;
			break;
	}
}

size_t World::getLevelNumber() const
{
	return mLevelNumber;
}

void World::update(sf::Time dt)
{
	// Pass the coordinates of the player in the camera control feature.
	setPlayerCoordinateForView(mPlayerHero->x, mPlayerHero->y, mLevelNumber);

	for (std::list<Entity*>::iterator it = mEntities.begin(); it != mEntities.end();)
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
			it = mEntities.erase(it);
			//delete (*it);
		}
	}

	if (mPlayerHero->mIsShoot && !mPlayerHero->mShooted)
	{
		mPlayerHero->mIsShoot = false;
		mPlayerHero->mShooted = true;
	}
	else if (mPlayerHero->mIsShooting)
	{
		mPlayerHero->mIsShooting = false;
		// If shot, it appears the bullet, pass the enum as an int.
		mEntities.push_back(new Bullet(Type::AlliedBullet, mTextures, mFonts, *mLevel, 
									   mPlayerHero->x + (mPlayerHero->mWidth / 2.f), 
									   mPlayerHero->y + 6.f, 7, 7, mPlayerHero->mState));
		// Play bullet's sound.
		mSound.play();
	}

	// Updates ShadowBoss actions.
	if (mShadowBoss.mIsActive)
	{
		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentaclesStatic.begin(); 
			 it != mShadowBoss.mTentaclesStatic.end();)
		{
			(*it)->update(static_cast<float>(dt.asMilliseconds()));
			if ((*it)->mLife)
			{
				++it;
			}
			else
			{
				it = mShadowBoss.mTentaclesStatic.erase(it);
				//delete (*it);
			}
		}

		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentacles.begin(); 
			 it != mShadowBoss.mTentacles.end();)
		{
			(*it)->update(static_cast<float>(dt.asMilliseconds()));
			if ((*it)->mLife)
			{
				++it;
			}
			else
			{
				it = mShadowBoss.mTentacles.erase(it);
				//delete (*it);
			}
		}

		mShadowBoss.mShadowLifeBar->update(mShadowBoss.mShadow->mHitpoints);
		mShadowBoss.mShadow->update(static_cast<float>(dt.asMilliseconds()));
	}

	// Updates GolemDark actions.
	if (mGolemBoss.mIsActive)
	{
		mGolemBoss.mGolemLifeBar->update(mGolemBoss.mGolem->mHitpoints);
		mGolemBoss.mGolem->update(static_cast<float>(dt.asMilliseconds()));
	}

	// Updates player's lifebar.
	mLifeBar->update(mPlayerHero->mHitpoints);

	mPlayerHero->update(static_cast<float>(dt.asMilliseconds()));

	// Checks collision between different objects.
	handleCollisions(static_cast<float>(dt.asMilliseconds()));

	mPlayerInfo->mDialogNumber = 0;
	if (mPlayerHero->mDialogNumber == 0)
	{
		mPlayerInfo->mDialogNumber = 0;
	}
	else
	{
		mPlayerInfo->showDialog(mPlayerHero->mDialogNumber);
	}
}

void World::draw()
{
	mWindow.setView(mWorldView);

	// Draw a level.
	mLevel->draw(mWindow);

	sf::Vector2f center = mWindow.getView().getCenter();
	sf::Vector2f size = mWindow.getView().getSize();

	sf::FloatRect viewRect = sf::FloatRect(center.x - (size.x / 2.f) - 50.f, 
										   center.y - (size.y / 2.f) - 50.f, 
										   size.x + 50.f, size.y + 50.f);

	for (const auto& entity : mEntities)
	{
		if (mDebug)
		{
			switch (entity->mTypeID)
			{
				case Type::Ghost:
				case Type::Golem:
				case Type::DarkSoldier:
				case Type::Goblin:
				case Type::MinotaurMage:
				case Type::Dwarf:
				case Type::DwarfArcher:
				case Type::DwarvenCommander:
				case Type::Tentacle:
				{
					sf::RectangleShape shape = buildBorderLines(entity->getRect(),
																sf::Color::Transparent,
																sf::Color::Red, 1.f);
					mWindow.draw(shape);
					break;
				}

				case Type::Rock:
				case Type::OpeningGate:
				case Type::ClosingGate:
				case Type::OpenClosingGate:
				{
					sf::RectangleShape shape = buildBorderLines(entity->getRect(),
																sf::Color::Transparent,
																sf::Color::Yellow, 1.f);
					mWindow.draw(shape);
					break;
				}

				case Type::Oswald:
				case Type::Heinrich:
				case Type::MovingPlatform:
				case Type::DeadJuggernaut:
				case Type::DeadDwarf:
				{
					sf::RectangleShape shape = buildBorderLines(entity->getRect(),
																sf::Color::Transparent,
																sf::Color::Magenta, 1.f);
					mWindow.draw(shape);
					break;
				}

				case Type::AlliedBullet:
				case Type::EnemyBullet:
				case Type::Flamestrike:
				{
					sf::RectangleShape shape = buildBorderLines(entity->getRect(),
																sf::Color::Transparent,
																sf::Color::Red, 1.f);
					mWindow.draw(shape);
					break;
				}

				default:
				{
					sf::RectangleShape shape = buildBorderLines(entity->getRect(),
																sf::Color::Transparent,
																sf::Color::White, 1.f);
					mWindow.draw(shape);
					break;
				}
			}
		}
		
		if (viewRect.contains(entity->x, entity->y))
			entity->draw(mWindow);
	}

	if (mShadowBoss.mIsActive)
	{
		for (const auto& tentacle : mShadowBoss.mTentaclesStatic)
		{
			if (mDebug)
			{
				sf::RectangleShape shape = buildBorderLines(tentacle->getRect(),
															sf::Color::Transparent,
															sf::Color::Red, 1.f);
				mWindow.draw(shape);
			}
			
			tentacle->draw(mWindow);
		}

		for (const auto& tentacle : mShadowBoss.mTentacles)
		{
			if (mDebug)
			{
				sf::RectangleShape shape = buildBorderLines(tentacle->getRect(),
															sf::Color::Transparent,
															sf::Color::Red, 1.f);
				mWindow.draw(shape);
			}
			
			tentacle->draw(mWindow);
		}
		
		if (mDebug)
		{
			sf::RectangleShape shape = buildBorderLines(mShadowBoss.mShadow->getRect(),
														sf::Color::Transparent,
														sf::Color::Red, 1.f);
			mWindow.draw(shape);
		}

		mShadowBoss.mShadowLifeBar->draw(mWindow);

		mShadowBoss.mShadow->draw(mWindow);
	}

	if (mGolemBoss.mIsActive)
	{
		if (mDebug)
		{
			sf::RectangleShape shape = buildBorderLines(mGolemBoss.mGolem->getRect(),
														sf::Color::Transparent,
														sf::Color::Red, 1.f);
			mWindow.draw(shape);
		}

		mGolemBoss.mGolemLifeBar->draw(mWindow);

		mGolemBoss.mGolem->draw(mWindow);
	}
	
	if (mDebug)
	{
		for (size_t i = 0; i < mObjects.size(); i++)
		{
			if (mObjects[i].mName == "solid")
			{
				sf::RectangleShape shape = buildBorderLines(mObjects[i].mRect, 
															sf::Color::Transparent, 
															sf::Color::Black, 1.f);
				mWindow.draw(shape);
			}
			else if (mObjects[i].mName == "enemyBorder")
			{
				sf::RectangleShape shape = buildBorderLines(mObjects[i].mRect,
															sf::Color::Transparent,
															sf::Color::Red, 1.f);
				mWindow.draw(shape);
			}
			else if (mObjects[i].mName == "death")
			{
				sf::RectangleShape shape = buildBorderLines(mObjects[i].mRect,
															sf::Color::Transparent,
															sf::Color::Magenta, 1.f);
				mWindow.draw(shape);
			}
			else if (mObjects[i].mName == "end")
			{
				sf::RectangleShape shape = buildBorderLines(mObjects[i].mRect,
															sf::Color::Transparent,
															sf::Color::White, 1.f);
				mWindow.draw(shape);
			}
			else if (mObjects[i].mName == "boss")
			{
				sf::RectangleShape shape = buildBorderLines(mObjects[i].mRect,
															sf::Color::Transparent,
															sf::Color::Red, 1.f);
				mWindow.draw(shape);
			}
			else if (mObjects[i].mName == "dialogMessage")
			{
				sf::RectangleShape shape = buildBorderLines(mObjects[i].mRect,
															sf::Color::Transparent,
															sf::Color::Blue, 1.f);
				mWindow.draw(shape);
			}
			else
			{
				sf::RectangleShape shape = buildBorderLines(mObjects[i].mRect,
															sf::Color::Transparent,
															sf::Color::White, 1.f);
				mWindow.draw(shape);
			}
		}

		sf::RectangleShape shape = buildBorderLines(mPlayerHero->getRect(), sf::Color::Transparent,
													sf::Color::Green, 1.f);
		mWindow.draw(shape);
	}
	
	mLifeBar->draw(mWindow);

	mPlayerHero->draw(mWindow);
}

bool World::hasAlivePlayer() const
{
	return mPlayerHero->mLife;
}

bool World::hasPlayerReachedEnd() const
{
	return mPlayerHero->mIsRichedEnd;
}

void World::setPlayerCoordinateForView(float x, float y, size_t levelNumber)
{
	float tempX = x, tempY = y;

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
			std::cout << "Error! Out of range level." << std::endl;
			break;
	}

	mWorldView.setCenter(tempX, tempY);

	if (mGolemBoss.mIsActive && mGolemBoss.mIsShaked)
	{
		mGolemBoss.mCameraCounter++;
		switch (mGolemBoss.mCameraCounter)
		{
			case 1:
				mWorldView.move(10, 10);
				break;
			
			case 2:
				mWorldView.move(-20, 0);
				break;
			
			case 3:
				mWorldView.move(0, -20);
				break;
			
			case 4:
				mWorldView.move(0, 20);
				break;
			
			default:
			{
				mWorldView.move(10, -10);
				mGolemBoss.mCameraCounter = 0;
				mGolemBoss.mIsShaked = false;

				int mRandom = rand() % 8;
				Entity* rock = new Rock(Type::Rock, mTextures, mFonts, *mLevel, 
										mGolemBoss.mRocks[mRandom]->x,
										mGolemBoss.mRocks[mRandom]->y, 16, "3");
				rock->mIsAttacked = true;
				mEntities.push_back(rock);

				break;
			}
		}
	}
}

void World::buildScene()
{
	// Initialization of level.
	mLevel = new Level();
	loadLevel(mLevelNumber);

	// Add player.
	Object playerObj = mLevel->getObject("player");
	mPlayerHero = new Player(Type::Archer, mTextures, mFonts, *mLevel, 
							 playerObj.mRect.left, playerObj.mRect.top, 20, 30, mPlayerInfo);
	mPlayerInfo->setPlayer(mPlayerHero);
	mLifeBar = new LifeBar(Type::HealthBar, mTextures, mFonts, mPlayerHero->mHitpoints);

	mPlayerInfo->mLastSavePoint.x = mPlayerHero->x;
	mPlayerInfo->mLastSavePoint.y = mPlayerHero->y;

	// Create and initialize first boss Shadow.
	Object shadowObj = mLevel->getObject("bossShadow");
	mShadowBoss.mShadow = new Shadow(Type::Shadow, mTextures, mFonts, *mLevel,
									 shadowObj.mRect.left, shadowObj.mRect.top, 
									 40, 35, shadowObj.mType);
	mShadowBoss.mShadowLifeBar = new LifeBar(Type::ShadowBossBar, mTextures, mFonts,
											 mShadowBoss.mShadow->mHitpoints);
	for (size_t i = 0; i < mShadowBoss.mNumberOfTentacles; i++)
	{
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts,
															*mLevel, 12013.f + 13.f * i, 994.f,
															13, 45, "0"));
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, 
															*mLevel, 12061.f + 13.f * i, 1073.f, 
															13, 45, "0"));
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, 
															*mLevel, 12285.f + 13.f * i, 1073.f, 
															13, 45, "0"));
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, 
															*mLevel, 12333.f + 13.f * i, 994.f, 
															13, 45, "0"));
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, 
															*mLevel, 12173.f + 13.f * i, 946.f, 
															13, 45, "0"));
	}

	// Create and initialize first mini-boss GolemDark.
	Object golemObj = mLevel->getObject("bossGolemDark");
	mGolemBoss.mGolem = new GolemDark(Type::GolemDark, mTextures, mFonts, *mLevel, 
									  golemObj.mRect.left, golemObj.mRect.top, 70, 60, 
									  golemObj.mType);
	mGolemBoss.mGolemLifeBar = new LifeBar(Type::GolemDarkBossBar, mTextures, mFonts, 
										   mGolemBoss.mGolem->mHitpoints);
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8192, 2272));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8272, 2240));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8336, 2192));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8384, 2160));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8464, 2160));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8560, 2144));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8656, 2144));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8736, 2160));

	// Add objects and enemies.
	addObjects();
}

void World::addObjects()
{
	// All objects, which contains tmx-file, program read into temporary vector.
	std::vector<Object> objects = mLevel->getObjects("enemyGhost");

	// Add all objects with current name to list of objects.
	for (const auto& object : objects)
	{
		mEntities.push_back(new Ghost(Type::Ghost, mTextures, mFonts, *mLevel,
									  object.mRect.left, object.mRect.top, 
									  static_cast<int>(object.mRect.width), 
									  static_cast<int>(object.mRect.height), object.mType));
	}

	objects = mLevel->getObjects("enemyGolem");
	for (const auto& object : objects)
	{
		mEntities.push_back(new Golem(Type::Golem, mTextures, mFonts, *mLevel,
									  object.mRect.left, object.mRect.top, 
									  static_cast<int>(object.mRect.width),
									  static_cast<int>(object.mRect.height), object.mType));
	}

	objects = mLevel->getObjects("enemySoldier");
	for (const auto& object : objects)
	{
		mEntities.push_back(new DarkSoldier(Type::DarkSoldier,  mTextures, mFonts, *mLevel, 
											object.mRect.left, object.mRect.top, 
											static_cast<int>(object.mRect.width),
											static_cast<int>(object.mRect.height), object.mType));
	}

	objects = mLevel->getObjects("enemyGoblin");
	for (const auto& object : objects)
	{
		mEntities.push_back(new Goblin(Type::Goblin,  mTextures, mFonts, *mLevel, 
									   object.mRect.left, object.mRect.top, 
									   static_cast<int>(object.mRect.width),
									   static_cast<int>(object.mRect.height), object.mType));
	}

	objects = mLevel->getObjects("enemyMinotaurMage");
	for (const auto& object : objects)
	{
		mEntities.push_back(new MinotaurMage(Type::MinotaurMage, mTextures, mFonts, *mLevel, 
											 object.mRect.left, object.mRect.top, 
											 static_cast<int>(object.mRect.width),
											 static_cast<int>(object.mRect.height),
											 object.mType));
	}

	objects = mLevel->getObjects("enemyDwarf");
	for (const auto& object : objects)
	{
		mEntities.push_back(new Dwarf(Type::Dwarf, mTextures, mFonts, *mLevel, 
									  object.mRect.left, object.mRect.top, 
									  static_cast<int>(object.mRect.width),
									  static_cast<int>(object.mRect.height), object.mType, 0));
	}

	objects = mLevel->getObjects("enemyDwarvenArcher");
	for (const auto& object : objects)
	{
		mEntities.push_back(new Dwarf(Type::DwarfArcher, mTextures, mFonts, *mLevel, 
									  object.mRect.left, object.mRect.top, 
									  static_cast<int>(object.mRect.width),
									  static_cast<int>(object.mRect.height), object.mType, 1));
	}

	objects = mLevel->getObjects("enemyDwarvenCommander");
	for (const auto& object : objects)
	{
		mEntities.push_back(new Dwarf(Type::DwarvenCommander, mTextures, mFonts, *mLevel,
									  object.mRect.left, object.mRect.top, 
									  static_cast<int>(object.mRect.width),
									  static_cast<int>(object.mRect.height), object.mType, 2));
	}

	objects = mLevel->getObjects("enemyDwarfA");
	for (const auto& object : objects)
	{
		Entity* dwarf = new Dwarf(Type::Dwarf, mTextures, mFonts, *mLevel, 
								  object.mRect.left, object.mRect.top, 
								  static_cast<int>(object.mRect.width),
								  static_cast<int>(object.mRect.height), object.mType, 0);
		dwarf->mIsStarted = true;
		mEntities.push_back(dwarf);
	}

	objects = mLevel->getObjects("enemyDwarvenArcherA");
	for (const auto& object : objects)
	{
		Entity* dwarf = new Dwarf(Type::DwarfArcher, mTextures, mFonts, *mLevel, 
								  object.mRect.left, object.mRect.top, 
								  static_cast<int>(object.mRect.width),
								  static_cast<int>(object.mRect.height), object.mType, 1);
		dwarf->mIsStarted = true;
		mEntities.push_back(dwarf);
	}

	objects = mLevel->getObjects("enemyDwarvenCommanderA");
	for (const auto& object : objects)
	{
		Entity* dwarf = new Dwarf(Type::DwarvenCommander, mTextures, mFonts, *mLevel, 
								  object.mRect.left, object.mRect.top, 
								  static_cast<int>(object.mRect.width),
								  static_cast<int>(object.mRect.height), object.mType, 2);
		dwarf->mIsStarted = true;
		mEntities.push_back(dwarf);
	}

	objects = mLevel->getObjects("enemyDwarvenCommanderM");
	for (const auto& object : objects)
	{
		Entity* dwarf = new Dwarf(Type::DwarvenCommander, mTextures, mFonts, *mLevel, 
								  object.mRect.left, object.mRect.top, 
								  static_cast<int>(object.mRect.width),
								  static_cast<int>(object.mRect.height), object.mType, 2);
		dwarf->mIsEnabling = true;
		mEntities.push_back(dwarf);
	}

	objects = mLevel->getObjects("enemyTentacle");
	for (const auto& object : objects)
	{
		mEntities.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, 
										 object.mRect.left, object.mRect.top, 
										 static_cast<int>(object.mRect.width),
										 static_cast<int>(object.mRect.height), object.mType));
	}

	// Add falling rocks, not enemy.
	objects = mLevel->getObjects("rock");
	for (const auto& object : objects)
	{
		mEntities.push_back(new Rock(Type::Rock, mTextures, mFonts, *mLevel, 
									 object.mRect.left, object.mRect.top, 16, object.mType));
	}

	// Add opening gates, not enemy.
	objects = mLevel->getObjects("gateO");
	for (const auto& object : objects)
	{
		mEntities.push_back(new Gate(Type::OpeningGate, mTextures, mFonts, *mLevel, 
									 object.mRect.left, object.mRect.top, 16, object.mType));
	}

	// Add closing gates, not enemy.
	objects = mLevel->getObjects("gateC");
	for (const auto& object : objects)
	{
		mEntities.push_back(new Gate(Type::ClosingGate, mTextures, mFonts, *mLevel, 
									 object.mRect.left, object.mRect.top, 16, object.mType));
	}

	// Add open-closing gates, not enemy.
	objects = mLevel->getObjects("gateOC");
	for (const auto& object : objects)
	{
		mEntities.push_back(new Gate(Type::OpenClosingGate, mTextures, mFonts, *mLevel, 
									 object.mRect.left, object.mRect.top, 16, object.mType));
	}

	// Add dialog persons, not enemy.
	objects = mLevel->getObjects("dialogPerson");
	for (const auto& object : objects)
	{
		switch(stoi(object.mType))
		{
			case 1:
				mEntities.push_back(new DialogPerson(Type::Oswald, mTextures, mFonts, *mLevel, 
													 object.mRect.left, object.mRect.top, 
													 static_cast<int>(object.mRect.width),
													 static_cast<int>(object.mRect.height), "2"));
				break;
			
			case 2:
				mEntities.push_back(new DialogPerson(Type::Heinrich, mTextures, mFonts, *mLevel, 
													 object.mRect.left, object.mRect.top, 
													 static_cast<int>(object.mRect.width),
													 static_cast<int>(object.mRect.height), "6"));
				break;
			
			default:
				std::cout << "Invalid type dialog person" << std::endl;
				break;
		}
	}

	// Add moving platforms, not enemy.
	objects = mLevel->getObjects("movingPlatform");
	for (const auto& object : objects)
	{
		mEntities.push_back(new MovingPlatform(Type::MovingPlatform, mTextures, mFonts, *mLevel, 
											   object.mRect.left, object.mRect.top, 
											   static_cast<int>(object.mRect.width),
											   static_cast<int>(object.mRect.height)));
	}

	// Add dead men, not enemy.
	objects = mLevel->getObjects("deadMan");
	for (const auto& object : objects)
	{
		switch (stoi(object.mType))
		{
			case 1:
				mEntities.push_back(new DeadMan(Type::DeadJuggernaut, mTextures, mFonts, *mLevel, 
												object.mRect.left, object.mRect.top, 
											    static_cast<int>(object.mRect.width),
											    static_cast<int>(object.mRect.height),
												object.mType));
				break;
			
			case 2:
				mEntities.push_back(new DeadMan(Type::DeadDwarf, mTextures, mFonts, *mLevel, 
												object.mRect.left, object.mRect.top, 
											    static_cast<int>(object.mRect.width),
											    static_cast<int>(object.mRect.height),
												object.mType));
				break;
			
			default:
				std::cout << "Invalid type dead person" << std::endl;
				break;
		}
	}

	// Add interaction objects, not enemy.
	objects = mLevel->getObjects("solid");
	for (const auto& object : objects)
	{
		mObjects.push_back(object);
	}
	objects = mLevel->getObjects("enemyBorder");
	for (const auto& object : objects)
	{
		mObjects.push_back(object);
	}
	objects = mLevel->getObjects("death");
	for (const auto& object : objects)
	{
		mObjects.push_back(object);
	}
	objects = mLevel->getObjects("end");
	for (const auto& object : objects)
	{
		mObjects.push_back(object);
	}
	objects = mLevel->getObjects("boss");
	for (const auto& object : objects)
	{
		mObjects.push_back(object);
	}
	objects = mLevel->getObjects("dialogMessage");
	for (const auto& object : objects)
	{
		mObjects.push_back(object);
	}
}

void World::addEnemy(Type::ID type, float relX, float relY)
{
	SpawnPoint spawn(type, relX, relY);
	mEnemySpawnPoints.push_back(spawn);
}

void World::guideMissiles()
{
	
}


void World::spawnEnemies()
{

}

void World::destroyEntitiesOutsideView()
{

}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, 
						 mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn.
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void World::handleCollisions(float dt)
{
	mPlayerHero->mOnPlatform = 0.f;

	sf::FloatRect playersRect = mPlayerHero->getRect();

	/// If we activated first boss Shadow...
	if (mPlayerHero->mHasStartedFirstBoss && !mShadowBoss.mIsFinished)
	{
		mShadowBoss.mIsActive = true;

		if (mShadowBoss.mShadow->mTypeID == Type::Shadow)
		{
			// If Shadow was activated first time...
			if (!mShadowBoss.mShadow->mIsStarted)
			{
				mShadowBoss.mShadow->mIsStarted = true;
				mAudioManager.setMusic(AudioManager::FirstBossMusic);
			}
			else if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
			{
				if (!mShadowBoss.mShadow->mIsBack &&
					((mPlayerHero->x - mShadowBoss.mShadow->x > 0.f &&
						mShadowBoss.mShadow->dx < 0.f) ||
						(mPlayerHero->x - mShadowBoss.mShadow->x < 0.f &&
							mShadowBoss.mShadow->dx > 0.f)))
				{
					mShadowBoss.mShadow->mIsBack = true;
					mShadowBoss.mShadow->dx = -mShadowBoss.mShadow->dx;
					mShadowBoss.mShadow->mSprite.scale(-1.f, 1.f);
				}
				else
				{
					mShadowBoss.mShadow->mIsBack = false;
				}
				mShadowBoss.mShadow->mIsAttacked = true;
				if (mShadowBoss.mShadow->mIsHitted && !mShadowBoss.mShadow->mIsStay)
				{
					mPlayerHero->mHitpoints -= mShadowBoss.mShadow->mDamage;
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts,
																  *mLevel, mPlayerHero->x - 50.f, 
																  mShadowBoss.mShadow->y - 10.f,
																  13, 45, "1"));
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts,
																  *mLevel, mPlayerHero->x - 25.f, 
																  mShadowBoss.mShadow->y - 10.f,
																  13, 45, "1"));
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts,
																  *mLevel, mPlayerHero->x, 
																  mShadowBoss.mShadow->y - 10.f, 
																  13, 45, "1"));
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts,
																  *mLevel, mPlayerHero->x + 25.f, 
																  mShadowBoss.mShadow->y - 10.f, 
																  13, 45, "1"));
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts,
																  *mLevel, mPlayerHero->x + 50.f, 
																  mShadowBoss.mShadow->y - 10.f,
																  13, 45, "1"));
					mShadowBoss.mShadow->mIsHitted = false;
					std::cout << "Shadow ATTACK\n";
				}
			}
		}

		if (mShadowBoss.mShadow->getRect().intersects(playersRect) &&
			mShadowBoss.mShadow->mIsDelay)
		{
			mPlayerHero->mHitpoints = 0;
		}

		for (auto& tentacle : mShadowBoss.mTentaclesStatic)
		{
			if (mShadowBoss.mShadow->mIsCalling)
			{
				if (!tentacle->mIsStarted)
				{
					tentacle->mIsStarted = true;
				}
				else
				{
					tentacle->mIsEnabled = true;
				}
				tentacle->mIsEnabling = true;
			}
			else if (mShadowBoss.mShadow->mIsWithdrawing)
			{
				tentacle->mIsDisabled = true;
				tentacle->mIsEnabling = false;
			}

			if (mShadowBoss.mIsFinished)
			{
				tentacle->mHitpoints = 0;
			}

			if (tentacle->getRect().intersects(playersRect) && mPlayerHero->mLife &&
				(mPlayerHero->mHitpoints > 0))
			{
				tentacle->mIsAttacked = true;
				if (tentacle->mIsHitted)
				{
					mPlayerHero->mHitpoints -= tentacle->mDamage;
					tentacle->mIsHitted = false;
					std::cout << "Hit\n";
				}
			}
		}

		for (auto& tentacle : mShadowBoss.mTentacles)
		{
			if (mShadowBoss.mShadow->mIsWithdrawing)
			{
				tentacle->mHitpoints = 0;
			}
			else
			{
				tentacle->mIsStarted = true;
				tentacle->mIsEnabling = true;

				if (tentacle->getRect().intersects(playersRect) && mPlayerHero->mLife &&
					(mPlayerHero->mHitpoints > 0))
				{
					tentacle->mIsAttacked = true;
					if (tentacle->mIsHitted)
					{
						mPlayerHero->mHitpoints -= tentacle->mDamage;
						tentacle->mIsHitted = false;
						std::cout << "Hit\n";
					}
				}
			}
		}
	}
	else if (mShadowBoss.mIsFinished && !mShadowBoss.mShadow->mLife)
	{
		mShadowBoss.mIsActive = false;
		mPlayerInfo->mQuests[3] = true;
		mAudioManager.setMusic(AudioManager::FirstMainMusic);
	}


	/// If we activated firs mini-boss GolemDark...
	if (mPlayerHero->mHasStartedSecondBoss && !mGolemBoss.mIsFinished)
	{
		mGolemBoss.mIsActive = true;

		if (mGolemBoss.mGolem->mTypeID == Type::GolemDark)
		{
			if (!mGolemBoss.mIsWeakened && (mPlayerInfo->mChosenSolution[1] == 1))
			{
				mGolemBoss.mGolem->mHitpoints -= 100;
				mGolemBoss.mIsWeakened = true;
			}

			// If GolemDark was activated first time.
			if (!mGolemBoss.mGolem->mIsStarted)
			{
				mGolemBoss.mGolem->mIsStarted = true;
				mGolemBoss.mGolem->mCurrentDeath = 5.f;
				mAudioManager.setMusic(AudioManager::FirstBossMusic);
			}
			else if (mGolemBoss.mGolem->getRect().intersects(playersRect))
			{
				if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!mGolemBoss.mGolem->mIsBack &&
						(((mPlayerHero->x - mGolemBoss.mGolem->x > 0.f) &&
						(mGolemBoss.mGolem->dx < 0.f)) ||
						((mPlayerHero->x - mGolemBoss.mGolem->x) < 0.f &&
							(mGolemBoss.mGolem->dx > 0.f))))
					{
						mGolemBoss.mGolem->mIsBack = true;
						mGolemBoss.mGolem->dx = -mGolemBoss.mGolem->dx;
					}
					mGolemBoss.mGolem->mIsAttacked = true;
					mGolemBoss.mGolem->mCurrentFrame = 0.f;
					if (mGolemBoss.mGolem->mIsHitted)
					{
						mPlayerHero->mHitpoints -= mGolemBoss.mGolem->mDamage;
						mGolemBoss.mGolem->mIsHitted = false;
						mGolemBoss.mIsShaked = true;
						std::cout << "GolemDark ATTACK\n";
					}
				}
			}

			if (mGolemBoss.mGolem->mIsHitted)
			{
				mGolemBoss.mIsShaked = true;
			}
		}
	}
	else if (mGolemBoss.mIsFinished && !mGolemBoss.mGolem->mLife)
	{
		mGolemBoss.mIsActive = false;
		mPlayerInfo->mQuests[4] = true;

		// Temporary object.
		Object*	tempObject = new Object();
		
		tempObject->mName = "dialogMessage";
		tempObject->mType = "9";

		sf::Rect <float> objectRect;
		objectRect.top = 2388;
		objectRect.left = 8352;
		objectRect.height = 60;
		objectRect.width = 80;
		tempObject->mRect = objectRect;
		mLevel->mObjects.push_back(*tempObject);
		mPlayerHero->mLevelObjects.push_back(*tempObject);

		delete tempObject;

		mAudioManager.setMusic(AudioManager::FirstMainMusic);
	}

	for (std::list<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		/// If player collided with MovingPlatform...
		if (((*it)->mTypeID == Type::MovingPlatform) &&
			((*it)->getRect().intersects(playersRect)))
		{
			// If the player is in a state after the jump, i.e. falls down...
			if ((mPlayerHero->dy > 0.f) || !mPlayerHero->mOnGround)
				// If the player is above the platform, i.e. it's his feet at least 
				// (since we already checked that he was confronted with platform)...
				if (mPlayerHero->y + mPlayerHero->mHeight < (*it)->y + (*it)->mHeight)
				{
					mPlayerHero->y = (*it)->y - mPlayerHero->mHeight + 3.f;
					mPlayerHero->x += (*it)->dx * dt;
					mPlayerHero->dy = 0;
					mPlayerHero->mOnPlatform = (*it)->dx;
					// It pushes the player so that he was standing on the platform.
					mPlayerHero->mOnGround = true;
				}
		}

		/// Special attacks enemies.
		/// Special attack of MinotaurMage.
		if (((*it)->mTypeID == Type::MinotaurMage) && ((*it)->mHitpoints > 0))
		{
			//isFired = false;
			sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), 
									 static_cast<float>((*it)->mHeight));
			size_t distance = 0;
			//(*it)->isAttacked = false;
			while (distance < 10)
			{
				if (findPlayer.intersects(playersRect) && mPlayerHero->mLife)
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
				distance++;
			}
			if ((*it)->mIsAttacked)
			{
				(*it)->mCurrentFrame = 0.f;

				if ((*it)->mIsHitted)
				{
					// Нанесение урона
					//mPlayerHero->mHitpoints -= 5;
					
					mEntities.push_back(new Flamestrike(Type::Flamestrike, mTextures, mFonts, *mLevel,
														mPlayerHero->x + (mPlayerHero->mWidth / 4),
														mPlayerHero->y - (mPlayerHero->mHeight / 2),
														13, 45));

					//(*it)->isBack = false;
					(*it)->mIsHitted = false;
					std::cout << "Hit\n";
				}
			}
		}

		/// Special attack of DwarfArcher.
		if (((*it)->mTypeID == Type::DwarfArcher) && ((*it)->mHitpoints > 0))
		{
			sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), 
									 static_cast<float>((*it)->mHeight));
			size_t distance = 0;
			while (distance < 10) {
				if (findPlayer.intersects(playersRect) && mPlayerHero->mLife)
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
				distance++;
			}
			if ((*it)->mIsAttacked)
			{
				(*it)->mCurrentFrame = 0.f;
				if ((*it)->mIsHitted)
				{
					mEntities.push_back(new Bullet(Type::EnemyBullet, mTextures, mFonts, 
												   *mLevel, (*it)->x + ((*it)->mWidth / 2.f), 
												   (*it)->y + ((*it)->mHeight / 2.f) - 3.f, 7, 7, 
												   (*it)->dx > 0.f ? 1 : 0));
					mSound.play();	// Играем звук пули
					(*it)->mIsHitted = false;
					std::cout << "Shoot\n";
				}
			}
		}

		/// The fall of the stones.
		if (((*it)->mTypeID == Type::Rock) && (!(*it)->mIsAttacked))
		{
			sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), 
									 static_cast<float>((*it)->mHeight));
			size_t distance = 0;
			while (distance < 20) {
				if (findPlayer.intersects(playersRect) && mPlayerHero->mLife)
				{
					(*it)->mIsAttacked = true;
					break;
				}
				
				findPlayer.top += 30.f;
				distance++;
			}
		}

		/// Closing the gates.
		if (((*it)->mTypeID == Type::ClosingGate) && (!(*it)->mIsAttacked))
		{
			sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), 
									 static_cast<float>((*it)->mHeight));
			size_t distance = 0;
			while (distance < 10) {
				if (findPlayer.intersects(playersRect) && mPlayerHero->mLife)
				{
					(*it)->mIsAttacked = true;
					break;
				}

				findPlayer.top += 30.f;
				distance++;
			}
		}

		/// Opening of the gates.
		if (((*it)->mTypeID == Type::OpeningGate) && (!(*it)->mIsStarted))
		{
			if (((*it)->mType == "3") && mPlayerInfo->mQuests[0])
				(*it)->mIsStarted = true;

			if (((*it)->mType == "4") && mPlayerHero->mActivatedGate)
				(*it)->mIsStarted = true;

			if (((*it)->mType == "5") && mPlayerInfo->mQuests[3])
				(*it)->mIsStarted = true;
		}

		///	Damage on the first boss Shadow.
		if (mShadowBoss.mShadow->getRect().intersects((*it)->getRect()) &&
			((*it)->mTypeID == Type::AlliedBullet) && mShadowBoss.mShadow->mLife &&
			(mShadowBoss.mShadow->mHitpoints > 0) && mShadowBoss.mIsActive &&
			mShadowBoss.mShadow->mIsStay)
		{
			mShadowBoss.mShadow->mHitpoints -= (*it)->mDamage;
			if (mShadowBoss.mShadow->mHitpoints <= 0)
			{
				mShadowBoss.mIsFinished = true;
			}
			(*it)->mLife = false;
		}

		/// Damage on the first mini-boss GolemDark.
		if (mGolemBoss.mGolem->getRect().intersects((*it)->getRect()) &&
			((*it)->mTypeID == Type::AlliedBullet) && mGolemBoss.mGolem->mLife &&
			(mGolemBoss.mGolem->mHitpoints > 0) && mGolemBoss.mIsActive)
		{
			mGolemBoss.mGolem->mHitpoints -= (*it)->mDamage;
			if (mGolemBoss.mGolem->mHitpoints <= 0)
			{
				mGolemBoss.mIsFinished = true;
			}
			(*it)->mLife = false;
		}

		/// If the rectangle of the sprite of the object intersects with the player...
		if ((*it)->getRect().intersects(playersRect))
		{
			/// If faced with Ghost...
			if ((*it)->mTypeID == Type::Ghost)
			{
				if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
						(mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
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
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						// Игрок отскакивает от врага
						mPlayerHero->dy = -0.1f;
						//p.mSpeed = 0.5f * (*it)->dx;
						//(*it)->isBack = false;
						(*it)->mIsHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// If faced with Golem...
			if ((*it)->mTypeID == Type::Golem)
			{
				// Если голем активирован
				if (!(*it)->mIsStarted)
				{
					(*it)->mIsStarted = true;
				}
				else if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
						(mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->mIsBack = true;
						(*it)->dx = -(*it)->dx;
					}
					(*it)->mIsAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->mIsHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->mIsHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// If faced with DarkSoldier...
			if ((*it)->mTypeID == Type::DarkSoldier)
			{
				if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
						(mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
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
						mPlayerHero->x = ((*it)->dx > 0 ? 1.2f : 0.8f) * (*it)->x;
						// Игрок отскакивает от врага
						mPlayerHero->y = 0.9f * (*it)->y;
						mPlayerHero->dy = -0.25f;
						(*it)->mIsHitted = false;
						std::cout << "Hit\n";
					}
					//p.x = 0.97f * ((*it)->dx > 0? (*it)->x + 60.f : (*it)->x);
					//p.dy = -0.5f;	// Игрок отскакивает от врага
				}
			}

			/// If faced with Goblin...
			if ((*it)->mTypeID == Type::Goblin)
			{
				if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
						(mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->mIsBack = true;
						(*it)->dx = -(*it)->dx;
					}
					(*it)->mIsAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->mIsHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->mIsHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// If faced with MinotaurMage...
			if ((*it)->mTypeID == Type::MinotaurMage)
			{
				if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
						(mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->mIsBack = true;
						(*it)->dx = -(*it)->dx;
						(*it)->mSprite.scale(-1.f, 1.f);
					}
					(*it)->mIsAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->mIsHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->mIsHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// If faced with Dwarf...
			if ((*it)->mTypeID == Type::Dwarf)
			{
				// Если дварф активирован
				if (!(*it)->mIsStarted)
				{
					(*it)->mIsStarted = true;
				}
				else if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
						(mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->mIsBack = true;
						(*it)->dx = -(*it)->dx;
						(*it)->mSprite.scale(-1.f, 1.f);
					}
					(*it)->mIsAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->mIsHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->mIsHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// If faced with DwarfArcher...
			if ((*it)->mTypeID == Type::DwarfArcher)
			{
				// Если дварф-лучник активирован
				if (!(*it)->mIsStarted)
				{
					(*it)->mIsStarted = true;
				}
				else if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
						(mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->mIsBack = true;
						(*it)->dx = -(*it)->dx;
						(*it)->mSprite.scale(-1.f, 1.f);
					}
					(*it)->mIsAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->mIsHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->mIsHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// If faced with DwarvenCommander...
			if ((*it)->mTypeID == Type::DwarvenCommander)
			{
				// Если дварф не активирован
				if (!(*it)->mIsStarted)
				{
					(*it)->mIsStarted = true;
					// Создаём рядом ещё двух гномов обычного типа, если сагрили командира гномов
					if (!(*it)->mIsSpawn)
					{
						if (mPlayerHero->dx >= 0.f)
						{
							Entity* dwarf = new Dwarf(Type::Dwarf, mTextures, mFonts, *mLevel,
													  (*it)->x - 30.f, (*it)->y, 40, 27, 
													  (*it)->mType, 0);
							dwarf->mIsStarted = true;
							mEntities.push_back(dwarf);
							dwarf = new Dwarf(Type::DwarfArcher, mTextures, mFonts, *mLevel, 
											  (*it)->x + 30.f, (*it)->y, 40, 27, (*it)->mType, 1);
							dwarf->mIsStarted = true;
							dwarf->mIsEnabling = false;
							dwarf->dx *= -1.f;
							dwarf->mSprite.scale(-1.f, 1.f);
							mEntities.push_back(dwarf);
						}
						else
						{
							Entity* dwarf = new Dwarf(Type::Dwarf, mTextures, mFonts, *mLevel, 
													  (*it)->x - 30.f, (*it)->y, 40, 27, 
													  (*it)->mType, 0);
							dwarf->mIsStarted = true;
							dwarf->dx *= -1.f;
							dwarf->mSprite.scale(-1.f, 1.f);
							mEntities.push_back(dwarf);
							dwarf = new Dwarf(Type::DwarfArcher, mTextures, mFonts, *mLevel, 
											  (*it)->x + 30.f, (*it)->y, 40, 27, (*it)->mType, 1);
							dwarf->mIsStarted = true;
							dwarf->mIsEnabling = false;
							mEntities.push_back(dwarf);
						}
						(*it)->mIsSpawn = true;
					}
				}
				else if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f) ||
						(mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->mIsBack = true;
						(*it)->dx = -(*it)->dx;
						(*it)->mSprite.scale(-1.f, 1.f);
					}
					(*it)->mIsAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->mIsHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->mIsHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// If faced with Tentacle...
			if ((*it)->mTypeID == Type::Tentacle)
			{
				// Если щупальце активировано
				if (!(*it)->mIsStarted)
				{
					(*it)->mIsStarted = true;
				}
				else if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					(*it)->mIsAttacked = true;
					if ((*it)->mIsHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->mIsHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// If faced with Flamestrike...
			if ((*it)->mTypeID == Type::Flamestrike)
			{
				if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					//mPlayerHero->mHitpoints -= (*it)->mDamage;
					//std::cout << "Hit\n";

					(*it)->mIsAttacked = true;
					if ((*it)->mIsHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->mIsHitted = false;
						std::cout << "Hit\n";
					}
				}
			}
		}

		/// If faced two objects: Player and EnemyBullet...
		if (playersRect.intersects((*it)->getRect()) && 
			((*it)->mTypeID == Type::EnemyBullet) && (mPlayerHero->mHitpoints > 0))
		{
			mPlayerHero->mHitpoints -= (*it)->mDamage;
			(*it)->mLife = false;
		}

		/// If faced two objects: Player and Rock...
		if (playersRect.intersects((*it)->getRect()) && ((*it)->mTypeID == Type::Rock) &&
			(*it)->mIsAttacked && !(*it)->mIsEnd && (mPlayerHero->mHitpoints > 0))
		{
			mPlayerHero->mHitpoints = 0;
		}

		/// If faced two objects: Player and Gate...
		if (playersRect.intersects((*it)->getRect()) &&
			((*it)->mTypeID == Type::OpeningGate || (*it)->mTypeID == Type::ClosingGate ||
			(*it)->mTypeID == Type::OpenClosingGate)  && (mPlayerHero->mHitpoints > 0))
		{
			// TODO: if the player stands under the doors when they are closed, have to kill him.
			/*if (((*it)->mTypeID == Type::ClosingGate ||
			      (*it)->mTypeID == Type::OpenClosingGate) && (mPlayerHero->y >= (*it)->y) &&
				  (mPlayerHero->x >= ((*it)->x + (static_cast<float>((*it)->mWidth) / 2.f))))
			{
				mPlayerHero->mHitpoints = 0;
			}*/

			if (mPlayerHero->dx > 0.f)
			{
				mPlayerHero->x = (*it)->x - mPlayerHero->mWidth;
			}
			if (mPlayerHero->dx < 0.f)
			{
				mPlayerHero->x = (*it)-> x + (*it)->mWidth;
			}
		}

		/// If faced two objects: Player and DialogPerson...
		if (playersRect.intersects((*it)->getRect()) && ((*it)->mTypeID == Type::Oswald ||
			(*it)->mTypeID == Type::Heinrich) && !(*it)->mIsEnd &&
			(mPlayerHero->mHitpoints > 0))
		{
			(*it)->mIsEnd = true;
			mPlayerHero->mDialogNumber = std::stoi((*it)->mType);

			switch ((*it)->mTypeID)
			{
				case Type::Oswald:
					mPlayerInfo->mQuests[0] = true;
					break;
				
				case Type::Heinrich:
					mPlayerInfo->mQuests[2] = true;
					break;
				
				default:
					std::cout << "Invalid dialog person type" << std::endl;
					break;
			}
		}

		/// Add a fallen stone in solid objects to check collisions.
		if (((*it)->mTypeID == Type::Rock) && (*it)->mIsEnd && !(*it)->mIsSpawn &&
			(mPlayerHero->mHitpoints > 0) && !mGolemBoss.mIsActive)
		{
			(*it)->mIsSpawn = true;

			// Temporary object.
			Object*	tempObject = new Object();

			tempObject->mName = "solid";
			tempObject->mType = (*it)->mType;
			tempObject->mSprite = (*it)->mSprite;

			sf::Rect <float> objectRect;
			objectRect.top = (*it)->y;
			objectRect.left = (*it)->x;
			objectRect.height = static_cast<float>((*it)->mHeight);
			objectRect.width = static_cast<float>((*it)->mWidth);
			tempObject->mRect = objectRect;
			mLevel->mObjects.push_back(*tempObject);
			mPlayerHero->mLevelObjects.push_back(*tempObject);

			delete tempObject;
		}

		/// Add the closed gate in the solid objects to check collisions.
		if (((*it)->mTypeID == Type::ClosingGate) && (*it)->mIsEnd && !(*it)->mIsSpawn &&
			(mPlayerHero->mHitpoints > 0))
		{
			(*it)->mIsSpawn = true;

			// Temporary object.
			Object*	tempObject = new Object();

			tempObject->mName = "solid";
			tempObject->mType = (*it)->mType;
			tempObject->mSprite = (*it)->mSprite;

			sf::Rect <float> objectRect;
			objectRect.top = (*it)->y;
			objectRect.left = (*it)->x;
			objectRect.height = static_cast<float>((*it)->mHeight);
			objectRect.width = static_cast<float>((*it)->mWidth);
			tempObject->mRect = objectRect;
			mLevel->mObjects.push_back(*tempObject);
			mPlayerHero->mLevelObjects.push_back(*tempObject);

			delete tempObject;
		}

		/// Collision detection between objects.
		for (std::list<Entity*>::iterator it2 = it; it2 != mEntities.end(); ++it2)
		{
			// This must be different objects.
			if ((*it) != (*it2))
			{
				sf::FloatRect entitysRect = (*it)->getRect();
				/// If faced two objects: Enemy and Rock...
				if (entitysRect.intersects((*it2)->getRect()) &&
					((*it)->mTypeID != Type::Rock) && ((*it2)->mTypeID == Type::Rock) &&
					(*it2)->mIsAttacked && !(*it2)->mIsEnd && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints = 0;
					(*it)->mIsStarted = true;
					(*it)->mIsEnd = true;
					(*it)->mIsSpawn = true;
				}

				/// If faced two objects: Bullet and Gate...
				if (entitysRect.intersects((*it2)->getRect()) &&
					((*it2)->mTypeID == Type::AlliedBullet ||
					(*it2)->mTypeID == Type::EnemyBullet) &&
						((*it)->mTypeID == Type::OpeningGate ||
					(*it)->mTypeID == Type::ClosingGate ||
							(*it)->mTypeID == Type::OpenClosingGate))
				{
					(*it2)->mLife = false;
				}

				/// If faced two objects: Bullet and Rock...
				if (entitysRect.intersects((*it2)->getRect()) && 
					((*it2)->mTypeID == Type::AlliedBullet ||
					(*it2)->mTypeID == Type::EnemyBullet) &&
						((*it)->mTypeID == Type::Rock) && !mGolemBoss.mIsActive)
				{
					(*it2)->mLife = false;
				}

				/// If faced two objects: Ghost and Bullet...
				if (entitysRect.intersects((*it2)->getRect()) &&
					((*it)->mTypeID == Type::Ghost) &&
					((*it2)->mTypeID == Type::AlliedBullet) && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// If faced two objects: Golem and Bullet...
				if (entitysRect.intersects((*it2)->getRect()) &&
					((*it)->mTypeID == Type::Golem) &&
					((*it2)->mTypeID == Type::AlliedBullet) &&
					((*it)->mHitpoints > 0) && (*it)->mIsEnd)
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// If faced two objects: Golem and Bullet...
				if (entitysRect.intersects((*it2)->getRect()) &&
					((*it)->mTypeID == Type::DarkSoldier) &&
					((*it2)->mTypeID == Type::AlliedBullet) && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// If faced two objects: Shadow and Bullet...
				if (entitysRect.intersects((*it2)->getRect()) &&
					((*it)->mTypeID == Type::Shadow) &&
					((*it2)->mTypeID == Type::AlliedBullet) &&
					((*it)->mHitpoints > 0) && (*it)->mIsEnd)
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// If faced two objects: Goblin and Bullet...
				if (entitysRect.intersects((*it2)->getRect()) &&
					((*it)->mTypeID == Type::Goblin) &&
					((*it2)->mTypeID == Type::AlliedBullet) && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// If faced two objects: MinotaurMage and Bullet...
				if (entitysRect.intersects((*it2)->getRect()) &&
					((*it)->mTypeID == Type::MinotaurMage) &&
					((*it2)->mTypeID == Type::AlliedBullet) && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// If faced two objects: Dwarf and Bullet...
				if (entitysRect.intersects((*it2)->getRect()) &&
					((*it)->mTypeID == Type::Dwarf) &&
					((*it2)->mTypeID == Type::AlliedBullet) && ((*it)->mHitpoints > 0) &&
					(*it)->mIsEnd)
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// If faced two objects: DwarfArcher and Bullet...
				if (entitysRect.intersects((*it2)->getRect()) && 
					((*it)->mTypeID == Type::DwarfArcher) && 
					((*it2)->mTypeID == Type::AlliedBullet) && ((*it)->mHitpoints > 0) &&
					(*it)->mIsEnd)
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// If faced two objects: DwarvenCommander and Bullet...
				if (entitysRect.intersects((*it2)->getRect()) &&
					((*it)->mTypeID == Type::DwarvenCommander) &&
					((*it2)->mTypeID == Type::AlliedBullet) && ((*it)->mHitpoints > 0) &&
					(*it)->mIsEnd)
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;

					if ((*it)->mIsEnabling && (*it)->mHitpoints <= 0)
					{
						mPlayerInfo->mQuests[1] = true;
						mPlayerHero->mDialogNumber = 4;
					}
				}
			}
		}
	}
}

void World::handleEvent()
{
		
}
