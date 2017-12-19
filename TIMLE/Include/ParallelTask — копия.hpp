#ifndef PARALLELTASK_HPP
#define PARALLELTASK_HPP

#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "PlayerInfo.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>


class ParallelTask
{
	private:
		std::thread			mThread;
		std::thread			mThreadClock;
		std::atomic<bool>	mFinished;
		std::atomic<bool>	mFinishedLoading;
		sf::Clock			mElapsedTime;
		std::mutex			mMutex;
		mutable std::shared_mutex mutex_;

		TextureHolder&		mTextures;
		SoundBufferHolder&	mSounds;

		PlayerInfo*			mPlayerInfo;
	
	
	private:
		void			runTask();
		void			loadTextures();


	public:
		explicit 		ParallelTask(TextureHolder& textures, SoundBufferHolder& sounds,
									 PlayerInfo* playerInfo);
		void			execute();
		bool			isFinished();
		float			getCompletion();
};

#endif // PARALLELTASK_HPP
