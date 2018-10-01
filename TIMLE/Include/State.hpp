#ifndef STATE_HPP
#define STATE_HPP

#include <memory>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include "PlayerInfo.hpp"
#include "AudioManager.hpp"


namespace sf
{
    class RenderWindow;
}

class StateStack;
class Player;

class State
{
    public:
        using unPtr = std::unique_ptr<State>;

        /**
         * \brief Enumeration of the window styles.
         */
        enum WindowStyle
        {
            None       = 0,      ///< No border / title bar (this flag and all others are mutually exclusive).
            Titlebar   = 1 << 0, ///< Title bar + fixed border.
            Resize     = 1 << 1, ///< Title bar + resizable border + maximize button.
            Close      = 1 << 2, ///< Title bar + close button.
            Fullscreen = 1 << 3, ///< Fullscreen mode (this flag and all others are mutually exclusive).

            Default    = Titlebar | Resize | Close ///< Default window style.
        };

        enum class ActualLanguage
        {
            English,
            Russian
        };

        enum class DebugMode
        {
            DebugOff,
            DebugOn
        };

        struct CurrentSettings
        {
                           CurrentSettings(const sf::Vector2u windowSize, 
                                           const WindowStyle windowStyle, const float musicVolume,
                                           const Fonts::ID fontType, 
                                           const ActualLanguage language, 
                                           const DebugMode debugMode);
            sf::Vector2u   mWindowSize;
            WindowStyle    mWindowStyle;
            float          mMusicVolume;
            Fonts::ID      mFontType;
            ActualLanguage mLanguage;
            DebugMode      mDebugMode;

            std::size_t    mPressedButton;
            bool           mHasAnyChanges;
        };

        struct Context
        {
                               Context(sf::RenderWindow& window, TextureHolder& textures, 
                                       FontHolder& fonts, SoundBufferHolder& sounds,
                                       PlayerInfo& playerInfo, CurrentSettings& currentSettings);

            sf::RenderWindow*  mWindow;
            TextureHolder*     mTextures;
            FontHolder*        mFonts;
            SoundBufferHolder* mSounds;
            PlayerInfo*        mPlayerInfo;
            CurrentSettings*   mCurrentSettings;
            
        };


    private:
        StateStack* _stack;
        Context     _context;


    public:
        static AudioManager mAudioManager;


    public:
                     State(StateStack& stack, const Context context);

        virtual      ~State() = default;

                     State(const State& other) = default;

                     State(State&& other) = default;

        State&       operator=(const State& other) = default;

        State&       operator=(State&& other) = default;

        virtual void draw() = 0;
        virtual bool update(const sf::Time dt) = 0;
        virtual bool handleEvent(const sf::Event& event) = 0;


    protected:
        void    requestStackPush(const States::ID stateID) const;
        void    requestStackPop() const;
        void    requestStateClear() const;

        Context getContext() const;
};

#endif // STATE_HPP
