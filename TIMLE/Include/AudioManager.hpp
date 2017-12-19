#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include <vector>
#include <iostream>

#include <SFML/Audio/Music.hpp>


/**
 * \brief Class for manipulation with musics in this app.
 */
class AudioManager
{
	private:
		/**
		 * \brief Array of music's states.
		 */
		enum class CurrentState
		{
			Play,
			Pause,
			Stop
		};


	public:
		/**
		 * \brief Contains all music types for playing.
		 */
		enum class MusicType
		{
			MainMenuTheme,
			FirstMainMusic,
			FirstBossMusic,
			FirstMiniBossMusic,
			SecondMusic,
			SecondBossMusic,
			SecondMiniBossMusic,
			ThirdMusic,
			ThirdBossMusic,
			ThirdMiniBossMusic,
			None
		};


	private:
		/**
		 * \brief Current playing music type.
		 */
		MusicType				_currentMusic;

		/**
		 * \brief Current music's state.
		 */
		CurrentState			_currentState;

		/**
		* \brief Main array with all musics.
		*/
		std::vector<sf::Music*>	_musics;


	private:
		/**
		 * \brief Initialization available musics.
		 */
		void			buildMusic();


	public:
		/**
		 * \brief Default constructor.
		 */
						AudioManager();

		/**
		 * \brief			Changes playing music for another.
		 * \param musicType New music type to play.
		 */
		void			setMusic(const MusicType musicType);

		/**
		 * \brief Stops all tracks even they aren't playing now.
		 */
		void			stopAllMusics();

		/**
		 * \brief	Gets current music type.
		 * \return	Current playing music type.
		 */
		MusicType		getMusicType() const;

		/**
		 * \brief	Gets information about current state.
		 * \return	True, if music is playing, else – false.
		 */
		bool			isPlaying() const;

		/**
		 * \brief			Set music volume.
		 * \param volume	New value to set.
		 */
		void			setMusicVolume(const float volume);


};

#endif // AUDIOMANAGER_HPP
