#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/NonCopyable.hpp>

#include "audio_manager.hpp"
#include "effect.hpp"
#include "level.hpp"
#include "life_bar.hpp"
#include "player.hpp"
#include "player_info.hpp"
#include "projectile.hpp"
#include "resource_identifiers.hpp"
#include "resource_holder.hpp"
#include "state.hpp"
#include "tentacle.hpp"
#include "world_context.hpp"


// Forward declaration.

namespace sf
{
    class RenderWindow;
}

/**
 * \brief One of the main classes of this app. 
 *        Contains all data about this level and context of the app.
 */
class World final : private sf::NonCopyable
{
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
        PlayerInfo&                        _playerInfo;
        std::unique_ptr<LifeBar>           _lifeBar;

        std::vector<std::unique_ptr<Entity>> _entities;
        std::vector<std::unique_ptr<Effect>> _effects;
        std::vector<std::unique_ptr<Projectile>> _guidedProjectiles;
        std::vector<Object>                _objects;
        std::vector<Object>                _doors;
        std::vector<WorldContext::SpawnPoint> _enemySpawnPoints; // Not using now.

        std::vector<sf::RectangleShape>    _debugRectsToDraw;

        /**
         * \brief Need to loading different sounds in this variable and play them.
         */
        sf::Sound                          _sound;

        /**
         * \brief Boolean flag for enabling debug-mode.
         */
        bool                               _debug;

        /**
         * \brief World context which contains level-specific data.
         */
        WorldContext                       _worldContext;
    

    private:
        void          setPlayerCoordinateForView(const float x, const float y);

        void          handleCollisions(const float dt);

        void          buildScene();

        void          addObjects();

        void          addEnemy(const Type::ID type, const float relX, const float relY);

        void          spawnEnemies();

        void          destroyEntitiesOutsideView();

        void          guideMissiles();

        sf::FloatRect getViewBounds() const;

        sf::FloatRect getBattlefieldBounds() const;


    public:
        /**
         * \brief                   Default constructor.
         * \param[out] window       Active window for rendering all stuff.
         * \param[out] textures     Textures holder for extracting necessary textures.
         * \param[out] fonts        Fonts holder for extracting necessary textures.
         * \param[out] sounds       Sounds holder for extracting necessary textures.
         * \param[out] playerInfo   Class that contains player's data.
         * \param[out] audioManager Audio manager for manipulating app's music.
         * \param[in] debugMode     Parameter used to define debug mode.
         * \param[in] levelNumer    Define number of loaded level.
         */
                    World(sf::RenderWindow& window, TextureHolder& textures,
                          FontHolder& fonts, SoundBufferHolder& sounds,
                          PlayerInfo& playerInfo, AudioManager& audioManager,
                          const State::DebugMode debugMode,
                          const std::size_t levelNumber);

        void        loadLevel();

        void        update(sf::Time dt);

        void        draw();

        void        handleEvent();
                       
        bool        hasAlivePlayer() const;

        bool        hasPlayerReachedEnd() const;

        std::size_t getLevelNumber() const;
};

#endif // WORLD_HPP
