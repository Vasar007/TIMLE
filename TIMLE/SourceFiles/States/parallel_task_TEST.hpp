#pragma once

#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <thread>

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

#include "Mechanics/resource_identifiers.hpp"
#include "Mechanics/resource_holder.hpp"
#include "Player/player_info.hpp"


class ParallelTask
{
    private:
        std::thread               mThread;
        std::thread               mThreadClock;
        std::atomic<bool>         mFinished;
        std::atomic<bool>         mFinishedLoading;
        sf::Clock                 mElapsedTime;
        std::mutex                mMutex;
        mutable std::shared_mutex mutex_;

        TextureHolder&            mTextures;
        SoundBufferHolder&        mSounds;

        PlayerInfo*               mPlayerInfo;
    
    
    private:
        void            runTask();
        void            loadTextures();


    public:
        explicit ParallelTask(TextureHolder& textures, SoundBufferHolder& sounds,
                              PlayerInfo* playerInfo);
        void     execute();
        bool     isFinished();
        float    getCompletion();
};
