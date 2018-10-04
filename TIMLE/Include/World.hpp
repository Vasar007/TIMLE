#ifndef WORLD_HPP
#define WORLD_HPP

#include <list>
#include <memory>
#include <vector>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio.hpp>

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "Lifebar.hpp"
#include "PlayerInfo.hpp"
#include "Shadow.hpp"
#include "Tentacle.hpp"
#include "GolemDark.hpp"
#include "AudioManager.hpp"
#include "Projectile.hpp"
#include "Effect.hpp"
#include "State.hpp"


// Forward declaration.

namespace sf
{
    class RenderWindow;
}

/**
 * \brief One of the main classes of this app. 
 *        Contains all data about this level and context of the app.
 */
class World : private sf::NonCopyable
{
    private:
        /**
         * \brief Data-structure that keeping coordinate for spawning entities with current type.
         */
        struct SpawnPoint
        {
            SpawnPoint(const Type::ID type, const float x, const float y)
            : mType(type)
            , x(x)
            , y(y)
            {
            }

            Type::ID mType;
            float    x;
            float    y;
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
            {
            }

            const std::size_t        mNumberOfTentacles;

            bool                     mIsActive;
            bool                     mIsFinished;

            std::unique_ptr<Shadow>  mShadow;
            std::unique_ptr<LifeBar> mShadowLifeBar;
            std::list<Tentacle>      mTentacles;
            std::list<Tentacle>      mTentaclesStatic;
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
            {
            }

            bool                       mIsActive;
            bool                       mIsFinished;
            bool                       mIsShaked;
            bool                       mIsWeakened;
            int                        mCameraCounter;

            std::unique_ptr<GolemDark> mGolem;
            std::unique_ptr<LifeBar>   mGolemLifeBar;
            std::vector<SpawnPoint>    mRocks;
        };


    private:
        sf::RenderWindow&                  _window;
        sf::View                           _worldView;
        TextureHolder&                     _textures;
        FontHolder&                        _fonts;
        SoundBufferHolder&                 _sounds;
        AudioManager&                      _audioManager;
        std::unique_ptr<Level>             _level;

        std::size_t                        _currentLevelNumber;
        sf::Vector2f                       _position;
        std::unique_ptr<Player>            _playerHero;
        PlayerInfo*                        _playerInfo;
        std::unique_ptr<LifeBar>           _lifeBar;

        ShadowBoss                         _shadowBoss;
        GolemBoss                          _golemBoss;

        std::list<std::unique_ptr<Entity>> _entities;
        std::list<std::unique_ptr<Effect>> _effects;
        std::vector<SpawnPoint>            _enemySpawnPoints;    // Not using now.
        std::vector<Projectile*>           _guidedProjectiles;
        std::vector<Object>                _objects;
        std::vector<Object>                _doors;

        std::vector<sf::RectangleShape>    _debugRectsToDraw;

        /**
         * \brief Need to loading different sounds in this variable and play them.
         */
        sf::Sound                          _sound;

        /**
         * \brief Boolean flag for enabling debug-mode.
         */
        bool                               _debug;
    

    private:
        void          setPlayerCoordinateForView(const float x, const float y, 
                                                 const std::size_t levelNumber);

        void          handleCollisions(const float dt);

        void          buildScene();

        void          addObjects();

        void          addEnemy(const Type::ID type, const float relX, const float relY);

        void          spawnEnemies();

        void          destroyEntitiesOutsideView();

        void          guideMissiles();

        sf::FloatRect getViewBounds() const;

        sf::FloatRect getBattlefieldBounds() const;

        bool          loadObjects(const std::string_view objectName);


    public:
        /**
         * \brief                   Default explicit constructor.
         * \param[out] window       Active window for rendering all stuff.
         * \param[out] textures     Textures holder for extracting necessary textures.
         * \param[out] fonts        Fonts holder for extracting necessary textures.
         * \param[out] sounds       Sounds holder for extracting necessary textures.
         * \param[out] playerInfo   Class that contains player's data.
         * \param[out] audioManager Audio manager for manipulating app's music.
         * \param[in] debugMode     Parameter used to define debug mode.
         * \param[in] levelNumer    Define number of loaded level.
         */
        explicit    World(sf::RenderWindow& window, TextureHolder& textures,
                          FontHolder& fonts, SoundBufferHolder& sounds,
                          PlayerInfo* playerInfo, AudioManager& audioManager,
                          const State::DebugMode debugMode,
                          const std::size_t levelNumber);

        void        loadLevel(const std::size_t levelNumber);

        void        update(sf::Time dt);

        void        draw();

        void        handleEvent();
                       
        bool        hasAlivePlayer() const;

        bool        hasPlayerReachedEnd() const;

        std::size_t getLevelNumber() const;
};

#endif // WORLD_HPP
