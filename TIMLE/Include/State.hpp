#ifndef STATE_HPP
#define STATE_HPP

#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include "PlayerInfo.hpp"
#include "AudioManager.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>
#include <SFML/Window/WindowStyle.hpp>


namespace sf
{
	class RenderWindow;
}

class StateStack;
class Player;

class State
{
	public:
		typedef std::unique_ptr<State> Ptr;

		/**
		 * \brief Enumeration of the window styles.
		 */
		enum WindowStyle
		{
			None = 0,      ///< No border / title bar (this flag and all others are mutually exclusive).
			Titlebar = 1 << 0, ///< Title bar + fixed border.
			Resize = 1 << 1, ///< Title bar + resizable border + maximize button.
			Close = 1 << 2, ///< Title bar + close button.
			Fullscreen = 1 << 3, ///< Fullscreen mode (this flag and all others are mutually exclusive).

			Default = Titlebar | Resize | Close ///< Default window style.
		};

		enum ActualLanguage
		{
			English,
			Russian
		};

		struct CurrentSettings
		{
							CurrentSettings(sf::Vector2u windowSize, WindowStyle windowStyle, 
											float musicVolume, Fonts::ID fontType, 
											ActualLanguage language);
			sf::Vector2u	mWindowSize;
			WindowStyle		mWindowStyle;
			float			mMusicVolume;
			Fonts::ID		mFontType;
			ActualLanguage	mLanguage;
			size_t			mPressedButton;
			bool			mHasAnyChanges;
		};

		struct Context
		{
								Context(sf::RenderWindow& window, TextureHolder& textures, 
										FontHolder& fonts, SoundBufferHolder& sounds,
										PlayerInfo& playerInfo, CurrentSettings& currentSettings);

			sf::RenderWindow*	mWindow;
			TextureHolder*		mTextures;
			FontHolder*			mFonts;
			SoundBufferHolder*	mSounds;
			PlayerInfo*			mPlayerInfo;
			CurrentSettings*	mCurrentSettings;
			
		};


	private:
		StateStack*				mStack;
		Context					mContext;


	public:
		static AudioManager		mAudioManager;


	public:
								State(StateStack& stack, Context context);
		virtual					~State();

		virtual void			draw() = 0;
		virtual bool			update(sf::Time dt) = 0;
		virtual bool			handleEvent(const sf::Event& event) = 0;


	protected:
		void					requestStackPush(States::ID stateID);
		void					requestStackPop();
		void					requestStateClear();

		Context					getContext() const;
};

#endif // STATE_HPP
