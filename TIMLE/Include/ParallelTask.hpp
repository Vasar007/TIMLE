#ifndef PARALLELTASK_HPP
#define PARALLELTASK_HPP

#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "PlayerInfo.hpp"


class ParallelTask
{
	private:
		sf::Thread			_thread;
		sf::Thread			_threadClock;
		bool				_finished;
		bool				_finishedLoading;
		sf::Clock			_elapsedTime;
		sf::Mutex			_mutex;

		TextureHolder&		_textures;
		SoundBufferHolder&	_sounds;

		PlayerInfo&			_playerInfo;
	
	
	private:
		void			runTask();
		void			loadTextures();


	public:
		explicit 		ParallelTask(TextureHolder& textures, SoundBufferHolder& sounds, PlayerInfo& playerInfo);
		void			execute();
		bool			isFinished();
		float			getCompletion();
};

#endif // PARALLELTASK_HPP
