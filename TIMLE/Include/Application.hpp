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
		/**
		 * \brief Constant for time per frame limits.
		 */
		static const sf::Time				TimePerFrame;

		/**
		 * \brief Array of constant for backgrounds in different levels.
		 */
		static const std::vector<sf::Color>	mColorConstants;
	
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
		PlayerInfo				mPlayerInfo;

		/**
		 * \brief Structure for containing all settings.
		 */
		State::CurrentSettings	mCurrentSettings;
	
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
		/**
		 * \brief App loop that processes all input events.
		 */
		void	processInput();
		
		/**
		 * \brief		Update stack of the states.
		 * \param dt	Delta time parameter.
		 */
		void	update(sf::Time dt);
		
		/**
		 * \brief Manipulate with window (such as drawing and changing view).
		 */
		void	render();
	
		/**
		 * \brief		Update statictics information of the app.
		 * \param dt	Delta time parameter.
		 */
		void	updateStatistics(sf::Time dt);
		/**
		 * \brief Register states for working with it.
		 */
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