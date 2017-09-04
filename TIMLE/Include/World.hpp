#ifndef WORLD_HPP
#define WORLD_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Level.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Lifebar.hpp"
#include "PlayerInfo.hpp"
#include "Shadow.hpp"
#include "Tentacle.hpp"
#include "GolemDark.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio.hpp>

#include <array>
#include <queue>
#include <list>


// Forward declaration.
namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
	private:
		struct SpawnPoint
		{
			SpawnPoint(Type::ID type, float x, float y)
			: mType(type)
			, x(x)
			, y(y)
			{
			}
	
			Type::ID	mType;
			float		x;
			float		y;
		};

		struct ShadowBoss
		{
			ShadowBoss()
			: mNumberOfTentacles(4)
			, mIsActive(false)
			, mIsFinished(false)
			, mShadow(nullptr)
			, mShadowLifeBar(nullptr)
			, mTentacles()
			, mTentaclesStatic()
			{
			}

			const int				mNumberOfTentacles;

			bool					mIsActive;
			bool					mIsFinished;

			Shadow*					mShadow;
			LifeBar*				mShadowLifeBar;
			std::list<Tentacle*>	mTentacles;
			std::list<Tentacle*>	mTentaclesStatic;
		};

		struct GolemBoss
		{
			GolemBoss()
			: mIsActive(false)
			, mIsFinished(false)
			, mIsShaked(false)
			, mIsWeakened(false)
			, mCameraCounter(0)
			, mGolem(nullptr)
			, mGolemLifeBar(nullptr)
			, mRocks()
			{
			}

			bool						mIsActive;
			bool						mIsFinished;
			bool						mIsShaked;
			bool						mIsWeakened;
			int							mCameraCounter;

			GolemDark*					mGolem;
			LifeBar*					mGolemLifeBar;
			std::vector<SpawnPoint*>	mRocks;
		};
	
	
	private:
		sf::RenderWindow&		mWindow;
		sf::View				mWorldView;
		TextureHolder&			mTextures;
		FontHolder&				mFonts;
		Level*					mLevel;
	
		sf::FloatRect			mWorldBounds;
		sf::Vector2f			mSpawnPosition;
		int						mLevelNumber;
		sf::Vector2f			mPosition;
		float					mScrollSpeed;
		Player*					mPlayerHero;
		PlayerInfo*				mPlayerInfo;
		LifeBar*				mLifeBar;

		ShadowBoss				mShadowBoss;
		GolemBoss				mGolemBoss;
	
		std::list<Entity*>		mEntities;
		std::vector<SpawnPoint>	mEnemySpawnPoints;
		std::vector<Enemy*>		mEnemies;
		std::vector<Object>		mObjects;

		Object*					mTempObject;

		sf::Music				mMusic;
		sf::SoundBuffer			mSoundBuffer;
		sf::SoundBuffer			mCasualSoundBuffer;
		sf::SoundBuffer			mDarkSoundBuffer;
		sf::Sound				mSound;

		bool					mDebug;
	

	private:
		void					setPlayerCoordinateForView(float x, float y, int levelNumber);
		void					handleCollisions(float time);
	
		void					buildScene();
		void					addObjects();
		void					addEnemy(Type::ID type, float relX, float relY);
		void					spawnEnemies();
		void					destroyEntitiesOutsideView();
		void					guideMissiles();
		sf::FloatRect			getViewBounds() const;
		sf::FloatRect			getBattlefieldBounds() const;


	public:
		explicit				World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, PlayerInfo* playerInfo);
		void					loadLevel(int levelNumber);
		void					update(sf::Time dt);
		void					draw();
		void					handleEvent();
								
		bool 					hasAlivePlayer() const;
		bool 					hasPlayerReachedEnd() const;
};

#endif // WORLD_HPP
