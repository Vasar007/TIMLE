#pragma once

#include <memory>
#include <vector>

#include <SFML/System/Time.hpp>

#include "Bosses/golem_dark.hpp"
#include "Bosses/shadow.hpp"
#include "Enemies/tentacle.hpp"
#include "Interface/level.hpp"
#include "Interface/life_bar.hpp"
#include "Mechanics/audio_manager.hpp"
#include "Player/player_info.hpp"


class WorldContext final
{
    public:
        /**
         * \brief Data-structure that keeping coordinate for spawning entities with current type.
         */
        struct SpawnPoint
        {
            SpawnPoint(const Type::ID type, const float x, const float y)
            : type(type)
            , x(x)
            , y(y)
            {
            }

            Type::ID type;
            float    x;
            float    y;
        };


    private:
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
            std::list<Tentacle>    mTentacles;
            std::list<Tentacle>    mTentaclesStatic;
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
        std::size_t    _currentLevelNumber;
        level&         _level;
        TextureHolder& _textures;
        FontHolder&    _fonts;

        ShadowBoss     _shadowBoss;
        GolemBoss      _golemBoss;

        bool           _debug;


    public:
            WorldContext(TextureHolder& textures, FontHolder& fonts,
                          const std::size_t currentLevelNumber, level& lvl, const bool debug);

        void buildBossData();

        void draw(sf::RenderTarget& target) const;

        void update(const sf::Time dt);

        void processGolemDarkEvents(sf::View& view, std::list<std::unique_ptr<Entity>>& entities);

        void processMainEvents(PlayerInfo& playerInfo, AudioManager& audioManager);

        void handleCollisions(Entity& entity);

        bool isGolemDarkActive() const noexcept;
};
