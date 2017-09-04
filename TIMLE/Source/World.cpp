#include "../Include/World.hpp"
#include "../Include/Projectile.hpp"
#include "../Include/Pickup.hpp"
#include "../Include/Foreach.hpp"
#include "../Include/Ghost.hpp"
#include "../Include/Golem.hpp"
#include "../Include/DarkSoldier.hpp"
#include "../Include/Shadow.hpp"
#include "../Include/Goblin.hpp"
#include "../Include/MinotaurMage.hpp"
#include "../Include/Dwarf.hpp"
#include "../Include/MovingPlatform.hpp"
#include "../Include/Bullet.hpp"
#include "../Include/Utility.hpp"
#include "../Include/Rock.hpp"
#include "../Include/Gate.hpp"
#include "../Include/DialogPerson.hpp"
#include "../Include/Tentacle.hpp"
#include "../Include/DeadMan.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <limits>


World::World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, PlayerInfo* playerInfo)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures(textures)
, mFonts(fonts)
, mLevel(nullptr)
, mWorldBounds(0.f, 0.f, 13072.f, 3200.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mLevelNumber(1)
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
, mTempObject(nullptr)
, mMusic()
, mSoundBuffer()
, mCasualSoundBuffer()
, mDarkSoundBuffer()
, mSound()
, mDebug(false)
{
	srand(static_cast<unsigned int>(time(NULL)));

	mSoundBuffer.loadFromFile("Media/Sounds/firebol3.ogg");
	mCasualSoundBuffer.loadFromFile("Media/Sounds/music.ogg");
	mDarkSoundBuffer.loadFromFile("Media/Sounds/DarkMusic.ogg");

	mSound.setBuffer(mSoundBuffer);

	mMusic.openFromFile("Media/Sounds/music.ogg");
	mMusic.play();
	mMusic.setLoop(true);

	buildScene();
	mWorldView.reset(sf::FloatRect(0, 0, 1280, 720));
	mWorldView.zoom(0.5f);

	//mObjects = mLevel->getObjects("solid"); // Strange line
}

void World::loadLevel(int levelNumber)
{
	mLevelNumber = levelNumber;
	switch (mLevelNumber)
	{
		case 1:
			mLevel->loadFromFile("Level1.tmx"); // Нужно будет положить в Levels/
			break;
		case 2:
			mLevel->loadFromFile("Level2.tmx");
			break;
		case 3:
			mLevel->loadFromFile("Level3.tmx");
			break;
		case 4:
			mLevel->loadFromFile("test_map.tmx");
			break;
		case 5:
			mLevel->loadFromFile("supertest_map.tmx");
			break;
		default:
			std::cout << "Error! Out of range level." << std::endl;
			mLevelNumber = 1;
			break;
	}
}

void World::update(sf::Time time)
{
	// Передаём координаты игрока в функцию управления камерой
	setPlayerCoordinateForView(mPlayerHero->x, mPlayerHero->y, mLevelNumber);

	for (std::list<Entity*>::iterator it = mEntities.begin(); it != mEntities.end();)
	{
		// Вызываем функцию update для объектов
		(*it)->update(static_cast<float>(time.asMilliseconds()));
		if ((*it)->mLife)
		{
			++it;
		}
		// Если объект "мёртв"
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
		// Если выстрелили, то появляется пуля, enum передаем как int
		mEntities.push_back(new Bullet(Type::ID::AlliedBullet, mTextures, mFonts, *mLevel, 
			mPlayerHero->x + (mPlayerHero->mWidth / 2.f), mPlayerHero->y + 6.f, 7, 7, mPlayerHero->mState));
		mSound.play();	// Играем звук пули
	}

	// Обновляем действия босса-тени
	if (mShadowBoss.mIsActive)
	{
		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentaclesStatic.begin(); it != mShadowBoss.mTentaclesStatic.end();)
		{
			(*it)->update(static_cast<float>(time.asMilliseconds()));
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

		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentacles.begin(); it != mShadowBoss.mTentacles.end();)
		{
			(*it)->update(static_cast<float>(time.asMilliseconds()));
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
		mShadowBoss.mShadow->update(static_cast<float>(time.asMilliseconds()));
	}

	// Обновляем действия мини-босса голема
	if (mGolemBoss.mIsActive)
	{
		mGolemBoss.mGolemLifeBar->update(mGolemBoss.mGolem->mHitpoints);
		mGolemBoss.mGolem->update(static_cast<float>(time.asMilliseconds()));
	}

	// Обновляем полосу здоровья
	mLifeBar->update(mPlayerHero->mHitpoints);

	mPlayerHero->update(static_cast<float>(time.asMilliseconds()));

	// Проверка столкновений
	handleCollisions(static_cast<float>(time.asMilliseconds()));

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

	// Рисуем новую карту
	mLevel->draw(mWindow);

	sf::Vector2f center = mWindow.getView().getCenter();
	sf::Vector2f size = mWindow.getView().getSize();

	sf::FloatRect viewRect = sf::FloatRect(center.x - (size.x / 2.f) - 25.f, center.y - (size.y / 2.f) - 25.f, size.x + 25.f, size.y + 25.f);

	for (std::list<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		if (mDebug)
		{
			switch ((*it)->mTypeID)
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
					sf::FloatRect rect = (*it)->getRect();
					sf::RectangleShape shape;
					shape.setPosition(sf::Vector2f(rect.left, rect.top));
					shape.setSize(sf::Vector2f(rect.width, rect.height));
					shape.setFillColor(sf::Color::Transparent);
					shape.setOutlineColor(sf::Color::Red);
					shape.setOutlineThickness(1.f);
					mWindow.draw(shape);
					break;
				}
				case Type::Rock:
				case Type::OpeningGate:
				case Type::ClosingGate:
				case Type::OpenClosingGate:
				{
					sf::FloatRect rect = (*it)->getRect();
					sf::RectangleShape shape;
					shape.setPosition(sf::Vector2f(rect.left, rect.top));
					shape.setSize(sf::Vector2f(rect.width, rect.height));
					shape.setFillColor(sf::Color::Transparent);
					shape.setOutlineColor(sf::Color::Yellow);
					shape.setOutlineThickness(1.f);
					mWindow.draw(shape);
					break;
				}
				case Type::Oswald:
				case Type::Heinrich:
				case Type::MovingPlatform:
				case Type::DeadJuggernaut:
				case Type::DeadDwarf:
				{
					sf::FloatRect rect = (*it)->getRect();
					sf::RectangleShape shape;
					shape.setPosition(sf::Vector2f(rect.left, rect.top));
					shape.setSize(sf::Vector2f(rect.width, rect.height));
					shape.setFillColor(sf::Color::Transparent);
					shape.setOutlineColor(sf::Color::Magenta);
					shape.setOutlineThickness(1.f);
					mWindow.draw(shape);
					break;
				}
				case Type::AlliedBullet:
				case Type::EnemyBullet:
				case Type::FireBall:
				{
					sf::FloatRect rect = (*it)->getRect();
					sf::RectangleShape shape;
					shape.setPosition(sf::Vector2f(rect.left, rect.top));
					shape.setSize(sf::Vector2f(rect.width, rect.height));
					shape.setFillColor(sf::Color::Transparent);
					shape.setOutlineColor(sf::Color::Red);
					shape.setOutlineThickness(1.f);
					mWindow.draw(shape);
					break;
				}
				default:
				{
					sf::FloatRect rect = (*it)->getRect();
					sf::RectangleShape shape;
					shape.setPosition(sf::Vector2f(rect.left, rect.top));
					shape.setSize(sf::Vector2f(rect.width, rect.height));
					shape.setFillColor(sf::Color::Transparent);
					shape.setOutlineColor(sf::Color::White);
					shape.setOutlineThickness(1.f);
					mWindow.draw(shape);
					break;
				}
			}
		}
		
		if (viewRect.contains((*it)->x, (*it)->y))
			mWindow.draw((*it)->mSprite);
	}

	if (mShadowBoss.mIsActive)
	{
		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentaclesStatic.begin(); it != mShadowBoss.mTentaclesStatic.end(); ++it)
		{
			if (mDebug)
			{
				sf::FloatRect rect = (*it)->getRect();
				sf::RectangleShape shape;
				shape.setPosition(sf::Vector2f(rect.left, rect.top));
				shape.setSize(sf::Vector2f(rect.width, rect.height));
				shape.setFillColor(sf::Color::Transparent);
				shape.setOutlineColor(sf::Color::Red);
				shape.setOutlineThickness(1.f);
				mWindow.draw(shape);
			}
			
			mWindow.draw((*it)->mSprite);
		}

		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentacles.begin(); it != mShadowBoss.mTentacles.end(); ++it)
		{
			if (mDebug)
			{
				sf::FloatRect rect = (*it)->getRect();
				sf::RectangleShape shape;
				shape.setPosition(sf::Vector2f(rect.left, rect.top));
				shape.setSize(sf::Vector2f(rect.width, rect.height));
				shape.setFillColor(sf::Color::Transparent);
				shape.setOutlineColor(sf::Color::Red);
				shape.setOutlineThickness(1.f);
				mWindow.draw(shape);
			}
			
			mWindow.draw((*it)->mSprite);
		}
		
		if (mDebug)
		{
			sf::FloatRect srect = mShadowBoss.mShadow->getRect();
			sf::RectangleShape sshape;
			sshape.setPosition(sf::Vector2f(srect.left, srect.top));
			sshape.setSize(sf::Vector2f(srect.width, srect.height));
			sshape.setFillColor(sf::Color::Transparent);
			sshape.setOutlineColor(sf::Color::Red);
			sshape.setOutlineThickness(1.f);
			mWindow.draw(sshape);
		}

		mShadowBoss.mShadowLifeBar->draw(mWindow);

		mWindow.draw(mShadowBoss.mShadow->mSprite);
	}

	if (mGolemBoss.mIsActive)
	{
		if (mDebug)
		{
			sf::FloatRect srect = mGolemBoss.mGolem->getRect();
			sf::RectangleShape sshape;
			sshape.setPosition(sf::Vector2f(srect.left, srect.top));
			sshape.setSize(sf::Vector2f(srect.width, srect.height));
			sshape.setFillColor(sf::Color::Transparent);
			sshape.setOutlineColor(sf::Color::Red);
			sshape.setOutlineThickness(1.f);
			mWindow.draw(sshape);
		}

		mGolemBoss.mGolemLifeBar->draw(mWindow);

		mWindow.draw(mGolemBoss.mGolem->mSprite);
	}
	
	if (mDebug)
	{
		for (size_t i = 0; i < mObjects.size(); i++)
		{
			if (mObjects[i].mName == "solid")
			{
				sf::FloatRect rect = mObjects[i].mRect;
				sf::RectangleShape shape;
				shape.setPosition(sf::Vector2f(rect.left, rect.top));
				shape.setSize(sf::Vector2f(rect.width, rect.height));
				shape.setFillColor(sf::Color::Transparent);
				shape.setOutlineColor(sf::Color::Black);
				shape.setOutlineThickness(1.f);
				mWindow.draw(shape);
			}
			else if (mObjects[i].mName == "enemyBorder")
			{
				sf::FloatRect rect = mObjects[i].mRect;
				sf::RectangleShape shape;
				shape.setPosition(sf::Vector2f(rect.left, rect.top));
				shape.setSize(sf::Vector2f(rect.width, rect.height));
				shape.setFillColor(sf::Color::Transparent);
				shape.setOutlineColor(sf::Color::Red);
				shape.setOutlineThickness(1.f);
				mWindow.draw(shape);
			}
			else if (mObjects[i].mName == "death")
			{
				sf::FloatRect rect = mObjects[i].mRect;
				sf::RectangleShape shape;
				shape.setPosition(sf::Vector2f(rect.left, rect.top));
				shape.setSize(sf::Vector2f(rect.width, rect.height));
				shape.setFillColor(sf::Color::Transparent);
				shape.setOutlineColor(sf::Color::Magenta);
				shape.setOutlineThickness(1.f);
				mWindow.draw(shape);
			}
			else if (mObjects[i].mName == "end")
			{
				sf::FloatRect rect = mObjects[i].mRect;
				sf::RectangleShape shape;
				shape.setPosition(sf::Vector2f(rect.left, rect.top));
				shape.setSize(sf::Vector2f(rect.width, rect.height));
				shape.setFillColor(sf::Color::Transparent);
				shape.setOutlineColor(sf::Color::White);
				shape.setOutlineThickness(1.f);
				mWindow.draw(shape);
			}
			else if (mObjects[i].mName == "boss")
			{
				sf::FloatRect rect = mObjects[i].mRect;
				sf::RectangleShape shape;
				shape.setPosition(sf::Vector2f(rect.left, rect.top));
				shape.setSize(sf::Vector2f(rect.width, rect.height));
				shape.setFillColor(sf::Color::Transparent);
				shape.setOutlineColor(sf::Color::Red);
				shape.setOutlineThickness(1.f);
				mWindow.draw(shape);
			}
			else if (mObjects[i].mName == "dialogMessage")
			{
				sf::FloatRect rect = mObjects[i].mRect;
				sf::RectangleShape shape;
				shape.setPosition(sf::Vector2f(rect.left, rect.top));
				shape.setSize(sf::Vector2f(rect.width, rect.height));
				shape.setFillColor(sf::Color::Transparent);
				shape.setOutlineColor(sf::Color::Blue);
				shape.setOutlineThickness(1.f);
				mWindow.draw(shape);
			}
			else
			{
				sf::FloatRect rect = mObjects[i].mRect;
				sf::RectangleShape shape;
				shape.setPosition(sf::Vector2f(rect.left, rect.top));
				shape.setSize(sf::Vector2f(rect.width, rect.height));
				shape.setFillColor(sf::Color::Transparent);
				shape.setOutlineColor(sf::Color::White);
				shape.setOutlineThickness(1.f);
				mWindow.draw(shape);
			}
		}

		sf::FloatRect rect = mPlayerHero->getRect();
		sf::RectangleShape shape;
		shape.setPosition(sf::Vector2f(rect.left, rect.top));
		shape.setSize(sf::Vector2f(rect.width, rect.height));
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color::Green);
		shape.setOutlineThickness(1.f);
		mWindow.draw(shape);
	}
	
	mLifeBar->draw(mWindow);

	mWindow.draw(mPlayerHero->mSprite);
}

bool World::hasAlivePlayer() const
{
	return mPlayerHero->mLife;
}

bool World::hasPlayerReachedEnd() const
{
	return mPlayerHero->mIsRichedEnd;
}

void World::setPlayerCoordinateForView(float x, float y, int levelNumber)
{
	float tempX = x, tempY = y;

	switch (levelNumber)
	{
		case 1:
			if (x < 400.f)
				tempX = 400.f;

			if (y < 100.f)
				tempY = 100.f;

			if (y > 3000.f)
				tempY = 3000.f;

			if (x > 12800.f)
				tempX = 12800.f;
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			if (x < 320.f)
				tempX = 320.f;

			if (y < 100.f)
				tempY = 100.f;

			if (y > 620.f)
				tempY = 620.f;

			if (x > 1280.f)
				tempX = 1280.f;
			break;
		case 5:
			if (x < 320.f)
				tempX = 320.f;

			if (y < 100.f)
				tempY = 100.f;

			if (y > 620.f)
				tempY = 620.f;

			if (x > 1280.f)
				tempX = 1280.f;
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
				Entity* rock = new Rock(Type::Rock, mTextures, mFonts, *mLevel, mGolemBoss.mRocks[mRandom]->x, mGolemBoss.mRocks[mRandom]->y, 16, "3");
				rock->mIsAttacked = true;
				mEntities.push_back(rock);

				break;
			}
		}
	}
}

void World::buildScene()
{
	// Инициализация уровня
	mLevel = new Level();
	loadLevel(mLevelNumber);

	// Добавление игрока
	Object playerObj = mLevel->getObject("player");
	mPlayerHero = new Player(Type::Archer, mTextures, mFonts, *mLevel, playerObj.mRect.left, playerObj.mRect.top, 20, 30, mPlayerInfo);
	mPlayerInfo->setPlayer(mPlayerHero);
	mLifeBar = new LifeBar(Type::HealthBar, mTextures, mFonts, mPlayerHero->mHitpoints);

	Object shadowObj = mLevel->getObject("bossShadow");
	mShadowBoss.mShadow = new Shadow(Type::Shadow, mTextures, mFonts, *mLevel, shadowObj.mRect.left, shadowObj.mRect.top, 40, 35, shadowObj.mType);
	mShadowBoss.mShadowLifeBar = new LifeBar(Type::ShadowBossBar, mTextures, mFonts, mShadowBoss.mShadow->mHitpoints);
	for (int i = 0; i < mShadowBoss.mNumberOfTentacles; i++)
	{
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, 12013.f + 13.f * i, 994.f, 13, 45, "0"));
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, 12061.f + 13.f * i, 1073.f, 13, 45, "0"));
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, 12285.f + 13.f * i, 1073.f, 13, 45, "0"));
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, 12333.f + 13.f * i, 994.f, 13, 45, "0"));
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, 12173.f + 13.f * i, 946.f, 13, 45, "0"));
	}

	Object golemObj = mLevel->getObject("bossGolemDark");
	mGolemBoss.mGolem = new GolemDark(Type::GolemDark, mTextures, mFonts, *mLevel, golemObj.mRect.left, golemObj.mRect.top, 70, 60, golemObj.mType);
	mGolemBoss.mGolemLifeBar = new LifeBar(Type::GolemDarkBossBar, mTextures, mFonts, mGolemBoss.mGolem->mHitpoints);
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8192, 2272));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8272, 2240));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8336, 2192));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8384, 2160));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8464, 2160));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8560, 2144));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8656, 2144));
	mGolemBoss.mRocks.push_back(new SpawnPoint(Type::Rock, 8736, 2160));

	// Добавление врагов
	addObjects();
}

void World::addObjects()
{
	// Все объкты врага на tmx-карте будут храниться в векторе
	std::vector<Object> e = mLevel->getObjects("enemyGhost");

	// Проходимся по элементам вектора с врагами
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new Ghost(Type::Ghost, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 51, 36, e[i].mType));
	}

	e = mLevel->getObjects("enemyGolem");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new Golem(Type::Golem, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 45, 38, e[i].mType));
	}

	e = mLevel->getObjects("enemySoldier");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new DarkSoldier(Type::DarkSoldier,  mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 20, 30, e[i].mType));
	}

	e = mLevel->getObjects("enemyGoblin");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new Goblin(Type::Goblin,  mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 25, 28, e[i].mType));
	}

	e = mLevel->getObjects("enemyMinotaurMage");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new MinotaurMage(Type::MinotaurMage, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 40, 40, e[i].mType));
	}

	e = mLevel->getObjects("enemyDwarf");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new Dwarf(Type::Dwarf, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 40, 27, e[i].mType, 0));
	}

	e = mLevel->getObjects("enemyDwarvenArcher");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new Dwarf(Type::DwarfArcher, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 40, 27, e[i].mType, 1));
	}

	e = mLevel->getObjects("enemyDwarvenCommander");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new Dwarf(Type::DwarvenCommander, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 40, 27, e[i].mType, 2));
	}

	e = mLevel->getObjects("enemyDwarfA");
	for (size_t i = 0; i < e.size(); i++)
	{
		Entity* dwarf = new Dwarf(Type::Dwarf, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 40, 27, e[i].mType, 0);
		dwarf->mIsStarted = true;
		mEntities.push_back(dwarf);
	}

	e = mLevel->getObjects("enemyDwarvenArcherA");
	for (size_t i = 0; i < e.size(); i++)
	{
		Entity* dwarf = new Dwarf(Type::DwarfArcher, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 40, 27, e[i].mType, 1);
		dwarf->mIsStarted = true;
		mEntities.push_back(dwarf);
	}

	e = mLevel->getObjects("enemyDwarvenCommanderA");
	for (size_t i = 0; i < e.size(); i++)
	{
		Entity* dwarf = new Dwarf(Type::DwarvenCommander, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 40, 27, e[i].mType, 2);
		dwarf->mIsStarted = true;
		mEntities.push_back(dwarf);
	}

	e = mLevel->getObjects("enemyDwarvenCommanderM");
	for (size_t i = 0; i < e.size(); i++)
	{
		Entity* dwarf = new Dwarf(Type::DwarvenCommander, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 40, 27, e[i].mType, 2);
		dwarf->mIsEnabling = true;
		mEntities.push_back(dwarf);
	}

	e = mLevel->getObjects("enemyTentacle");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 13, 45, e[i].mType));
	}

	// Добавляем падающие камни, не враг
	e = mLevel->getObjects("rock");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new Rock(Type::Rock, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 16, e[i].mType));
	}

	// Добавляем врата, не враг
	e = mLevel->getObjects("gateO");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new Gate(Type::OpeningGate, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 16, e[i].mType));
	}

	// Добавляем врата, не враг
	e = mLevel->getObjects("gateC");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new Gate(Type::ClosingGate, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 16, e[i].mType));
	}

	// Добавляем врата, не враг
	e = mLevel->getObjects("gateOC");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new Gate(Type::OpenClosingGate, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 16, e[i].mType));
	}

	// Добавляем диалоговых персонажей, не враг
	e = mLevel->getObjects("dialogPerson");
	for (size_t i = 0; i < e.size(); i++)
	{
		switch(std::stoi(e[i].mType))
		{
			case 1:
				mEntities.push_back(new DialogPerson(Type::Oswald, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 16, 20, "2"));
				break;
			case 2:
				mEntities.push_back(new DialogPerson(Type::Heinrich, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 29, 32, "6"));
				break;
			default:
				std::cout << "Invalid type dialog person" << std::endl;
				break;
		}
	}

	// Добавляем платформы, не враг
	e = mLevel->getObjects("movingPlatform");
	for (size_t i = 0; i < e.size(); i++)
	{
		mEntities.push_back(new MovingPlatform(Type::MovingPlatform, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 48, 11));
	}

	// Добавляем трупы, не враг
	e = mLevel->getObjects("deadMan");
	for (size_t i = 0; i < e.size(); i++)
	{
		switch (std::stoi(e[i].mType))
		{
			case 1:
				mEntities.push_back(new DeadMan(Type::DeadJuggernaut, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 35, 15, e[i].mType));
				break;
			case 2:
				mEntities.push_back(new DeadMan(Type::DeadDwarf, mTextures, mFonts, *mLevel, e[i].mRect.left, e[i].mRect.top, 25, 15, e[i].mType));
				break;
			default:
				std::cout << "Invalid type dead person" << std::endl;
				break;
		}
	}

	// Добавляем области взаимодействия
	e = mLevel->getObjects("solid");
	for (size_t i = 0; i < e.size(); i++)
	{
		mObjects.push_back(e[i]);
	}
	e = mLevel->getObjects("enemyBorder");
	for (size_t i = 0; i < e.size(); i++)
	{
		mObjects.push_back(e[i]);
	}
	e = mLevel->getObjects("death");
	for (size_t i = 0; i < e.size(); i++)
	{
		mObjects.push_back(e[i]);
	}
	e = mLevel->getObjects("end");
	for (size_t i = 0; i < e.size(); i++)
	{
		mObjects.push_back(e[i]);
	}
	e = mLevel->getObjects("boss");
	for (size_t i = 0; i < e.size(); i++)
	{
		mObjects.push_back(e[i]);
	}
	e = mLevel->getObjects("dialogMessage");
	for (size_t i = 0; i < e.size(); i++)
	{
		mObjects.push_back(e[i]);
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
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void World::handleCollisions(float time)
{
	mPlayerHero->mOnPlatform = 0.f;

	sf::FloatRect playersRect = mPlayerHero->getRect();

	/// Если активировали первого босса тень
	if (mPlayerHero->mHasStartedFirstBoss && !mShadowBoss.mIsFinished)
	{
		mShadowBoss.mIsActive = true;

		if (mShadowBoss.mShadow->mTypeID == Type::ID::Shadow)
		{
			// Если тень активирована
			if (!mShadowBoss.mShadow->mIsStarted)
			{
				mShadowBoss.mShadow->mIsStarted = true;
				//mMusic.pause();
				//mDarkMusic.play();
			}
			else if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
			{
				if (!mShadowBoss.mShadow->mIsBack && ((mPlayerHero->x - mShadowBoss.mShadow->x > 0.f && mShadowBoss.mShadow->dx < 0.f)
					|| (mPlayerHero->x - mShadowBoss.mShadow->x < 0.f && mShadowBoss.mShadow->dx > 0.f)))
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
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, mPlayerHero->x - 50.f, mShadowBoss.mShadow->y - 10.f, 13, 45, "1"));
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, mPlayerHero->x - 25.f, mShadowBoss.mShadow->y - 10.f, 13, 45, "1"));
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, mPlayerHero->x, mShadowBoss.mShadow->y - 10.f, 13, 45, "1"));
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, mPlayerHero->x + 25.f, mShadowBoss.mShadow->y - 10.f, 13, 45, "1"));
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, mPlayerHero->x + 50.f, mShadowBoss.mShadow->y - 10.f, 13, 45, "1"));
					mShadowBoss.mShadow->mIsHitted = false;
					std::cout << "Shadow ATTACK\n";
				}
			}
		}

		if (mShadowBoss.mShadow->getRect().intersects(playersRect) && mShadowBoss.mShadow->mIsDelay)
		{
			mPlayerHero->mHitpoints = 0;
		}

		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentaclesStatic.begin(); it != mShadowBoss.mTentaclesStatic.end(); ++it)
		{
			if (mShadowBoss.mShadow->mIsCalling)
			{
				if (!(*it)->mIsStarted)
					(*it)->mIsStarted = true;
				else
					(*it)->mIsEnabled = true;
				(*it)->mIsEnabling = true;
			}
			else if (mShadowBoss.mShadow->mIsWithdrawing)
			{
				(*it)->mIsDisabled = true;
				(*it)->mIsEnabling = false;
			}

			if (mShadowBoss.mIsFinished)
			{
				(*it)->mHitpoints = 0;
			}

			if ((*it)->getRect().intersects(playersRect) && mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
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

		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentacles.begin(); it != mShadowBoss.mTentacles.end(); ++it)
		{
			if (mShadowBoss.mShadow->mIsWithdrawing)
				(*it)->mHitpoints = 0;
			else
			{
				(*it)->mIsStarted = true;
				(*it)->mIsEnabling = true;

				if ((*it)->getRect().intersects(playersRect) && mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
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
		}
	}
	else if (mShadowBoss.mIsFinished && !mShadowBoss.mShadow->mLife)
	{
		mShadowBoss.mIsActive = false;
		mPlayerInfo->mQuests[3] = true;
		//mDarkMusic.pause();
		//mMusic.play();
	}


	/// Если активировали мини-босса голема
	if (mPlayerHero->mHasStartedSecondBoss && !mGolemBoss.mIsFinished)
	{
		mGolemBoss.mIsActive = true;

		if (mGolemBoss.mGolem->mTypeID == Type::ID::GolemDark)
		{
			if (!mGolemBoss.mIsWeakened && (mPlayerInfo->mChosenSolution[1] == 1))
			{
				mGolemBoss.mGolem->mHitpoints -= 100;
				mGolemBoss.mIsWeakened = true;
			}

			// Если голем активирован
			if (!mGolemBoss.mGolem->mIsStarted)
			{
				mGolemBoss.mGolem->mIsStarted = true;
				mGolemBoss.mGolem->mCurrentDeath = 5.f;
				//mMusic.pause();
				//mDarkMusic.play();
			}
			else if (mGolemBoss.mGolem->getRect().intersects(playersRect))
			{
				/*// Если голем не активирован
				if (!mGolemBoss.mGolem->isStarted)
				{
					mGolemBoss.mGolem->isStarted = true;
				}
				else */if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!mGolemBoss.mGolem->mIsBack && ((mPlayerHero->x - mGolemBoss.mGolem->x > 0.f && mGolemBoss.mGolem->dx < 0.f)
						|| (mPlayerHero->x - mGolemBoss.mGolem->x < 0.f && mGolemBoss.mGolem->dx > 0.f)))
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
				mGolemBoss.mIsShaked = true;
		}
	}
	else if (mGolemBoss.mIsFinished && !mGolemBoss.mGolem->mLife)
	{
		mGolemBoss.mIsActive = false;
		mPlayerInfo->mQuests[4] = true;
		
		mTempObject = new Object();
		mTempObject->mName = "dialogMessage";
		mTempObject->mType = "9";

		sf::Rect <float> objectRect;
		objectRect.top = 2388;
		objectRect.left = 8352;
		objectRect.height = 60;
		objectRect.width = 80;
		mTempObject->mRect = objectRect;
		mLevel->mObjects.push_back(*mTempObject);
		mPlayerHero->mLevelObjects.push_back(*mTempObject);

		//mDarkMusic.pause();
		//mMusic.play();
	}

	for (std::list<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		/// Если игрок столкнулся с объектом movingPlatform
		if (((*it)->mTypeID == Type::ID::MovingPlatform) && ((*it)->getRect().intersects(playersRect)))
		{
			// При этом игрок находится в состоянии после прыжка, т.е падает вниз
			if ((mPlayerHero->dy > 0.f) || !mPlayerHero->mOnGround)
				// Если игрок находится выше платформы, т.е это его ноги минимум (т.к. мы уже проверяли что он столкнулся с платформой)
				if (mPlayerHero->y + mPlayerHero->mHeight < (*it)->y + (*it)->mHeight)
				{
					mPlayerHero->y = (*it)->y - mPlayerHero->mHeight + 3.f;
					mPlayerHero->x += (*it)->dx * time;
					mPlayerHero->dy = 0;
					mPlayerHero->mOnPlatform = (*it)->dx;
					// То выталкиваем игрока так, чтобы он как бы стоял на платформе
					mPlayerHero->mOnGround = true;
				}
		}

		/// Особые атаки врагов
		/// Особая атака минотавра-мага
		if (((*it)->mTypeID == Type::ID::MinotaurMage) && ((*it)->mHitpoints > 0))
		{
			//isFired = false;
			sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), static_cast<float>((*it)->mHeight));
			int distance = 0;
			//(*it)->isAttacked = false;
			while (distance < 10)
			{
				if (findPlayer.intersects(playersRect) && mPlayerHero->mLife)
				{
					(*it)->mIsAttacked = true;
					break;
				}
				else
				{
					if ((*it)->dx > 0.f)
					{
						findPlayer.left += 30.f;
					}
					else
					{
						findPlayer.left -= 30.f;
					}
				}
				distance++;
			}
			if ((*it)->mIsAttacked)
			{
				(*it)->mCurrentFrame = 0.f;
				(*it)->mMoveTimer += 0.005f * time;
				if ((*it)->mMoveTimer > 24.f)
					(*it)->mMoveTimer -= 24.f;
				//fireDamage.setPosition(mPlayerHero->x - 49.f, mPlayerHero->y - 185.f);
				//fireDamage.setTextureRect(sf::IntRect(128 * (int((*it)->mMoveTimer) % 8), 256 * (int((*it)->mMoveTimer) / 8), 128, 256));
				//fireDamage.setScale(0.5f, 0.5f);
				//isFired = true;
				if ((*it)->mIsHitted)
				{
					// Нанесение урона
					mPlayerHero->mHitpoints -= 5;
					//p.dx = 3.f * (*it)->dx;
					//p.dy = -0.5f;	// Игрок отскакивает от врага
					//(*it)->isBack = false;
					(*it)->mIsHitted = false;
					(*it)->mMoveTimer = 0.f;
					std::cout << "Hit\n";
				}
			}
		}

		/// Особая атака Гнома-лучника
		if (((*it)->mTypeID == Type::ID::DwarfArcher) && ((*it)->mHitpoints > 0))
		{
			sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), static_cast<float>((*it)->mHeight));
			int distance = 0;
			while (distance < 10) {
				if (findPlayer.intersects(playersRect) && mPlayerHero->mLife)
				{
					(*it)->mIsAttacked = true;
					break;
				}
				else
				{
					if ((*it)->dx > 0.f)
					{
						findPlayer.left += 30.f;
					}
					else
					{
						findPlayer.left -= 30.f;
					}
				}
				distance++;
			}
			if ((*it)->mIsAttacked)
			{
				(*it)->mCurrentFrame = 0.f;
				if ((*it)->mIsHitted)
				{
					mEntities.push_back(new Bullet(Type::ID::EnemyBullet, mTextures, mFonts, *mLevel,
						(*it)->x + ((*it)->mWidth / 2.f), (*it)->y + ((*it)->mHeight / 2.f) - 3.f, 7, 7, (*it)->dx > 0.f ? 1 : 0));
					mSound.play();	// Играем звук пули
					(*it)->mIsHitted = false;
					std::cout << "Shoot\n";
				}
			}
		}

		/// Падение камней
		if (((*it)->mTypeID == Type::ID::Rock) && (!(*it)->mIsAttacked))
		{
			sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), static_cast<float>((*it)->mHeight));
			int distance = 0;
			while (distance < 20) {
				if (findPlayer.intersects(playersRect) && mPlayerHero->mLife)
				{
					(*it)->mIsAttacked = true;
					break;
				}
				else
				{
					findPlayer.top += 30.f;

				}
				distance++;
			}
		}

		/// Закрытие врат
		if (((*it)->mTypeID == Type::ID::ClosingGate) && (!(*it)->mIsAttacked))
		{
			sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), static_cast<float>((*it)->mHeight));
			int distance = 0;
			while (distance < 10) {
				if (findPlayer.intersects(playersRect) && mPlayerHero->mLife)
				{
					(*it)->mIsAttacked = true;
					break;
				}
				else
				{
					findPlayer.top += 30.f;
				}
				distance++;
			}
		}

		/// Открытие врат
		if (((*it)->mTypeID == Type::ID::OpeningGate) && (!(*it)->mIsStarted))
		{
			if (((*it)->mType == "3") && mPlayerInfo->mQuests[0])
				(*it)->mIsStarted = true;

			if (((*it)->mType == "4") && mPlayerHero->mActivatedGate)
				(*it)->mIsStarted = true;

			if (((*it)->mType == "5") && mPlayerInfo->mQuests[3])
				(*it)->mIsStarted = true;
		}

		/// Урон по боссу тени
		if (mShadowBoss.mShadow->getRect().intersects((*it)->getRect()) && ((*it)->mTypeID == Type::ID::AlliedBullet)
			&& mShadowBoss.mShadow->mLife && (mShadowBoss.mShadow->mHitpoints > 0) && mShadowBoss.mIsActive && mShadowBoss.mShadow->mIsStay)
		{
			mShadowBoss.mShadow->mHitpoints -= (*it)->mDamage;
			if (mShadowBoss.mShadow->mHitpoints <= 0)
				mShadowBoss.mIsFinished = true;
			(*it)->mLife = false;
		}

		/// Урон по мини-боссу голему
		if (mGolemBoss.mGolem->getRect().intersects((*it)->getRect()) && ((*it)->mTypeID == Type::ID::AlliedBullet)
			&& mGolemBoss.mGolem->mLife && (mGolemBoss.mGolem->mHitpoints > 0) && mGolemBoss.mIsActive)
		{
			mGolemBoss.mGolem->mHitpoints -= (*it)->mDamage;
			if (mGolemBoss.mGolem->mHitpoints <= 0)
				mGolemBoss.mIsFinished = true;
			(*it)->mLife = false;
		}

		/// Если прямоугольник спрайта объекта пересекается с игроком
		if ((*it)->getRect().intersects(playersRect))
		{
			/// Если столкнулись с призраком
			if ((*it)->mTypeID == Type::ID::Ghost)
			{
				if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
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

			/// Если столкнулись с големом
			if ((*it)->mTypeID == Type::ID::Golem)
			{
				// Если голем активирован
				if (!(*it)->mIsStarted)
				{
					(*it)->mIsStarted = true;
				}
				else if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
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

			/// Если мы столкнулись с тёмным солдатом
			if ((*it)->mTypeID == Type::ID::DarkSoldier)
			{
				if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
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

			/// Если столкнулись с гоблином
			if ((*it)->mTypeID == Type::ID::Goblin)
			{
				if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
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

			/// Если столкнулись с минотавром-магом
			if ((*it)->mTypeID == Type::ID::MinotaurMage)
			{
				if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
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

			/// Если столкнулись с дварфом
			if ((*it)->mTypeID == Type::ID::Dwarf)
			{
				// Если дварф активирован
				if (!(*it)->mIsStarted)
				{
					(*it)->mIsStarted = true;
				}
				else if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
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

			/// Если столкнулись с дварфом-лучником
			if ((*it)->mTypeID == Type::ID::DwarfArcher)
			{
				// Если дварф-лучник активирован
				if (!(*it)->mIsStarted)
				{
					(*it)->mIsStarted = true;
				}
				else if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
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

			/// Если столкнулись с командиром дварфов
			if ((*it)->mTypeID == Type::ID::DwarvenCommander)
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
							Entity* dwarf = new Dwarf(Type::ID::Dwarf, mTextures, mFonts, *mLevel, (*it)->x - 30.f, (*it)->y, 40, 27, (*it)->mType, 0);
							dwarf->mIsStarted = true;
							mEntities.push_back(dwarf);
							dwarf = new Dwarf(Type::ID::DwarfArcher, mTextures, mFonts, *mLevel, (*it)->x + 30.f, (*it)->y, 40, 27, (*it)->mType, 1);
							dwarf->mIsStarted = true;
							dwarf->mIsEnabling = false;
							dwarf->dx *= -1.f;
							dwarf->mSprite.scale(-1.f, 1.f);
							mEntities.push_back(dwarf);
						}
						else
						{
							Entity* dwarf = new Dwarf(Type::ID::Dwarf, mTextures, mFonts, *mLevel, (*it)->x - 30.f, (*it)->y, 40, 27, (*it)->mType, 0);
							dwarf->mIsStarted = true;
							dwarf->dx *= -1.f;
							dwarf->mSprite.scale(-1.f, 1.f);
							mEntities.push_back(dwarf);
							dwarf = new Dwarf(Type::ID::DwarfArcher, mTextures, mFonts, *mLevel, (*it)->x + 30.f, (*it)->y, 40, 27, (*it)->mType, 1);
							dwarf->mIsStarted = true;
							dwarf->mIsEnabling = false;
							mEntities.push_back(dwarf);
						}
						(*it)->mIsSpawn = true;
					}
				}
				else if (mPlayerHero->mLife && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->mIsBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
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

			/// Если столкнулись с щупальцем
			if ((*it)->mTypeID == Type::ID::Tentacle)
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
		}

		/// Если столкнулись два объекта: герой и пуля
		if ((playersRect.intersects((*it)->getRect()))
			&& ((*it)->mTypeID == Type::ID::EnemyBullet) && (mPlayerHero->mHitpoints > 0))
		{
			mPlayerHero->mHitpoints -= (*it)->mDamage;
			(*it)->mLife = false;
		}

		/// Если столкнулись два объекта: герой и камень
		if ((playersRect.intersects((*it)->getRect()))
			&& ((*it)->mTypeID == Type::ID::Rock) && (*it)->mIsAttacked && !(*it)->mIsEnd && (mPlayerHero->mHitpoints > 0))
		{
			mPlayerHero->mHitpoints = 0;
		}

		/// Если столкнулись два объекта: герой и врата
		if ((playersRect.intersects((*it)->getRect()))
			&& ((*it)->mTypeID == Type::ID::OpeningGate || (*it)->mTypeID == Type::ID::ClosingGate || (*it)->mTypeID == Type::ID::OpenClosingGate) 
			&& (mPlayerHero->mHitpoints > 0))
		{
			mPlayerHero->mHitpoints = 0;
		}

		/// Если столкнулись два объекта: герой и диалоговый персонаж
		if ((playersRect.intersects((*it)->getRect()))
			&& ((*it)->mTypeID == Type::ID::Oswald || (*it)->mTypeID == Type::ID::Heinrich) && !(*it)->mIsEnd && (mPlayerHero->mHitpoints > 0))
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

		/// Добавляем упавший камень в твёрдные объекты для проверки коллизии
		if (((*it)->mTypeID == Type::ID::Rock) && (*it)->mIsEnd && !(*it)->mIsSpawn && (mPlayerHero->mHitpoints > 0) && !mGolemBoss.mIsActive)
		{
			(*it)->mIsSpawn = true;

			mTempObject = new Object();
			mTempObject->mName = "solid";
			mTempObject->mType = (*it)->mType;
			mTempObject->mSprite = (*it)->mSprite;

			sf::Rect <float> objectRect;
			objectRect.top = (*it)->y;
			objectRect.left = (*it)->x;
			objectRect.height = static_cast<float>((*it)->mHeight);
			objectRect.width = static_cast<float>((*it)->mWidth);
			mTempObject->mRect = objectRect;
			mLevel->mObjects.push_back(*mTempObject);
			mPlayerHero->mLevelObjects.push_back(*mTempObject);
		}

		/// Добавляем закрытые врата в смертельные объекты для проверки коллизии
		if (((*it)->mTypeID == Type::ID::ClosingGate) && (*it)->mIsEnd && !(*it)->mIsSpawn && (mPlayerHero->mHitpoints > 0))
		{
			(*it)->mIsSpawn = true;

			mTempObject = new Object();
			mTempObject->mName = "death";
			mTempObject->mType = (*it)->mType;
			mTempObject->mSprite = (*it)->mSprite;

			sf::Rect <float> objectRect;
			objectRect.top = (*it)->y;
			objectRect.left = (*it)->x;
			objectRect.height = static_cast<float>((*it)->mHeight);
			objectRect.width = static_cast<float>((*it)->mWidth);
			mTempObject->mRect = objectRect;
			mLevel->mObjects.push_back(*mTempObject);
			mPlayerHero->mLevelObjects.push_back(*mTempObject);
		}

		/// Проверка столкновений объектов между собой
		for (std::list<Entity*>::iterator it2 = it; it2 != mEntities.end(); ++it2)
		{
			// При этом это должны быть разные объекты
			if ((*it) != (*it2))
			{
				sf::FloatRect entitysRect = (*it)->getRect();
				/// Если столкнулись два объекта: враг и камень
				if ((entitysRect.intersects((*it2)->getRect())) && ((*it)->mTypeID != Type::ID::Rock)
					&& ((*it2)->mTypeID == Type::ID::Rock) && (*it2)->mIsAttacked && !(*it2)->mIsEnd && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints = 0;
					(*it)->mIsStarted = true;
					(*it)->mIsEnd = true;
					(*it)->mIsSpawn = true;
				}

				/// Если столкнулись два объекта: пуля и врата
				if ((entitysRect.intersects((*it2)->getRect())) && ((*it2)->mTypeID == Type::ID::AlliedBullet || (*it2)->mTypeID == Type::ID::EnemyBullet)
					&& ((*it)->mTypeID == Type::ID::OpeningGate || (*it)->mTypeID == Type::ID::ClosingGate || (*it)->mTypeID == Type::ID::OpenClosingGate))
				{
					(*it2)->mLife = false;
				}

				/// Если столкнулись два объекта: пуля и камень
				if ((entitysRect.intersects((*it2)->getRect())) && ((*it2)->mTypeID == Type::ID::AlliedBullet || (*it2)->mTypeID == Type::ID::EnemyBullet)
					&& ((*it)->mTypeID == Type::ID::Rock) && !mGolemBoss.mIsActive)
				{
					(*it2)->mLife = false;
				}

				/// Если столкнулись два объекта: призрак и пуля
				if ((entitysRect.intersects((*it2)->getRect())) && ((*it)->mTypeID == Type::ID::Ghost)
					&& ((*it2)->mTypeID == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// Если столкнулись два объекта: голем и пуля
				if ((entitysRect.intersects((*it2)->getRect())) && ((*it)->mTypeID == Type::ID::Golem)
					&& ((*it2)->mTypeID == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0) && ((*it)->mIsEnd))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// Если столкнулись два объекта: тёмный солдат и пуля
				if ((entitysRect.intersects((*it2)->getRect())) && ((*it)->mTypeID == Type::ID::DarkSoldier)
					&& ((*it2)->mTypeID == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// Если столкнулись два объекта: тень и пуля
				if ((entitysRect.intersects((*it2)->getRect())) && ((*it)->mTypeID == Type::ID::Shadow)
					&& ((*it2)->mTypeID == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0) && ((*it)->mIsEnd))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// Если столкнулись два объекта: гоблин и пуля
				if ((entitysRect.intersects((*it2)->getRect())) && ((*it)->mTypeID == Type::ID::Goblin)
					&& ((*it2)->mTypeID == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// Если столкнулись два объекта: минотавр-маг и пуля
				if ((entitysRect.intersects((*it2)->getRect())) && ((*it)->mTypeID == Type::ID::MinotaurMage)
					&& ((*it2)->mTypeID == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// Если столкнулись два объекта: дварф и пуля
				if ((entitysRect.intersects((*it2)->getRect())) && ((*it)->mTypeID == Type::ID::Dwarf)
					&& ((*it2)->mTypeID == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0) && ((*it)->mIsEnd))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// Если столкнулись два объекта: дварф-лучник и пуля
				if ((entitysRect.intersects((*it2)->getRect())) && ((*it)->mTypeID == Type::ID::DwarfArcher)
					&& ((*it2)->mTypeID == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0) && ((*it)->mIsEnd))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->mLife = false;
				}

				/// Если столкнулись два объекта: командир дварфов и пуля
				if ((entitysRect.intersects((*it2)->getRect())) && ((*it)->mTypeID == Type::ID::DwarvenCommander)
					&& ((*it2)->mTypeID == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0) && ((*it)->mIsEnd))
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
