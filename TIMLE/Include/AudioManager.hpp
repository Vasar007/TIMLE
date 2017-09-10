#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include <SFML/Audio/Music.hpp>

#include <vector>
#include <iostream>


/**
 * \brief Class for manipulation with musics in this app.
 */
class AudioManager
{
	private:
		/**
		 * \brief Array of music's states.
		 */
		enum CurrentState
		{
			Play,
			Pause,
			Stop
		};


	public:
		/**
		 * \brief Contains all music types for playing.
		 */
		enum MusicType
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
		MusicType				mCurrentMusic;

		/**
		 * \brief Current music's state.
		 */
		CurrentState			mCurrentState;

		/**
		* \brief Main array with all musics.
		*/
		std::vector<sf::Music*>	mMusics;


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
		 * \param musicType New music type for playing.
		 */
		void			setMusic(MusicType musicType);

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


};

#endif // AUDIOMANAGER_HPP
