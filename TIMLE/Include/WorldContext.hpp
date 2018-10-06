#ifndef WORLDCONTEXT_HPP
#define WORLDCONTEXT_HPP

#include <list>
#include <memory>

#include <SFML/System/Time.hpp>

#include "AudioManager.hpp"
#include "GolemDark.hpp"
#include "Level.hpp"
#include "Lifebar.hpp"
#include "PlayerInfo.hpp"
#include "Shadow.hpp"
#include "Tentacle.hpp"


class WorldContext final
{
    public:
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
        std::size_t    _currentLevelNumber;
        Level&         _level;
        TextureHolder& _textures;
        FontHolder&    _fonts;

        ShadowBoss     _shadowBoss;
        GolemBoss      _golemBoss;

        bool           _debug;


    public:
            WorldContext(TextureHolder& textures, FontHolder& fonts,
                          const std::size_t currentLevelNumber, Level& lvl, const bool debug);

        void buildBossData();

        void draw(sf::RenderTarget& target) const;

        void update(const sf::Time dt);

        void processGolemDarkEvents(sf::View& view, std::list<std::unique_ptr<Entity>>& entities);

        void processMainEvents(PlayerInfo& playerInfo, AudioManager& audioManager);

        void handleCollisions(Entity& entity);

        bool isGolemDarkActive() const;
};

#endif // WORLDCONTEXT_HPP
