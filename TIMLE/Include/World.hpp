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
#include "AudioManager.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio.hpp>

#include <array>
#include <queue>
#include <list>


// Forward declaration.

namespace sf
{
	class RenderWindow;
}

/**
 * \brief One of the main classes of this app. 
 *		  Contains all data about this level and context of the app.
 */
class World : private sf::NonCopyable
{
	private:
		/**
		 * \brief Data-structure that keeping coordinate for spawning entities with current type.
		 */
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

		/**
	 	 * \brief Additional data-structure that contains info about boss of the first level.
		 */
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

			const size_t			mNumberOfTentacles;

			bool					mIsActive;
			bool					mIsFinished;

			Shadow*					mShadow;
			LifeBar*				mShadowLifeBar;
			std::list<Tentacle*>	mTentacles;
			std::list<Tentacle*>	mTentaclesStatic;
		};

		/**
		 * \brief Additional data-structure that contains info about mini-boss of the first level.
		 */
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
		SoundBufferHolder&		mSounds;
		AudioManager&			mAudioManager;
		Level*					mLevel;

	
		sf::FloatRect			mWorldBounds;
		sf::Vector2f			mSpawnPosition;
		size_t					mLevelNumber;
		sf::Vector2f			mPosition;
		float					mScrollSpeed;
		Player*					mPlayerHero;
		PlayerInfo*				mPlayerInfo;
		LifeBar*				mLifeBar;


		ShadowBoss				mShadowBoss;
		GolemBoss				mGolemBoss;

	
		std::list<Entity*>		mEntities;
		std::vector<SpawnPoint>	mEnemySpawnPoints;	// Not using now.
		std::vector<Enemy*>		mEnemies;	// Not using now.
		std::vector<Object>		mObjects;


		/**
		 * \brief Need to loading different sounds in this variable and play them.
		 */
		sf::Sound				mSound;


		/**
		 * \brief Boolean flag for enabling debug-mode.
		 */
		bool					mDebug;
	

	private:
		void					setPlayerCoordinateForView(float x, float y, size_t levelNumber);
		
		void					handleCollisions(float dt);
	
		void					buildScene();
		
		void					addObjects();
		
		void					addEnemy(Type::ID type, float relX, float relY);
		
		void					spawnEnemies();
		
		void					destroyEntitiesOutsideView();
		
		void					guideMissiles();
		
		sf::FloatRect			getViewBounds() const;
		
		sf::FloatRect			getBattlefieldBounds() const;


	public:
		/**
		 * \brief				Default explicit constructor.
		 * \param window		Active window for rendering all stuff.
		 * \param textures		Textures holder for extracting necessary textures.
		 * \param fonts			Fonts holder for extracting necessary textures.
		 * \param sounds		Sounds holder for extracting necessary textures.
		 * \param playerInfo	Class that contains player's data.
		 * \param audioManager	Audio manager for manipulating app's music.
		 */
		explicit				World(sf::RenderWindow& window, TextureHolder& textures,
									  FontHolder& fonts, SoundBufferHolder& sounds, 
									  PlayerInfo* playerInfo, AudioManager& audioManager);

		void					loadLevel(size_t levelNumber);
		
		void					update(sf::Time dt);
		
		void					draw();
		
		void					handleEvent();
								
		bool 					hasAlivePlayer() const;
		
		bool 					hasPlayerReachedEnd() const;

		size_t					getLevelNumber() const;
};

#endif // WORLD_HPP
