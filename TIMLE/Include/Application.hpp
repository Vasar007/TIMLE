#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Player.hpp"
#include "StateStack.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/SoundBuffer.hpp>


/**
 * \brief Main class of this project.
 */
class Application
{
	private:
		static const sf::Time	TimePerFrame;
	
		/**
		 * \brief Active window to using.
		 */
		sf::RenderWindow		mWindow;

		/**
		 * \brief Variable, which contains all textures for this app.
		 */
		TextureHolder			mTextures;

		/**
		 * \brief Variable, which contains all fonts for this app.
		 */
		FontHolder				mFonts;

		/**
		* \brief Variable, which contains all sounds for this app.
		*/
		SoundBufferHolder		mSounds;

		/**
		 * \brief Variable, which contains all player's data.
		 */
		PlayerInfo				mPlayer;
	
		/**
		 * \brief State stack, can contain and update different states (section "States").
		 */
		StateStack				mStateStack;
	
		/**
		 * \brief Text for appearing current FPS of this app.
		 */
		sf::Text				mStatisticsText;

		/**
		 * \brief Time for updating current FPS of this app.
		 */
		sf::Time				mStatisticsUpdateTime;

		/**
		 * \brief Number FPS of this app.
		 */
		std::size_t				mStatisticsNumFrames;


	private:
		void	processInput();
		void	update(sf::Time dt);
		void	render();
	
		void	updateStatistics(sf::Time dt);
		void	registerStates();


	public:
		/**
		 * \brief Default constructor.
		 */
				Application();

		
		/**
		 * \brief Main app loop.
		 */
		void	run();
};

#endif // APPLICATION_HPP
