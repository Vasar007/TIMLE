#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

#include "resource_identifiers.hpp"
#include "resource_holder.hpp"
#include "player.hpp"
#include "state_stack.hpp"


/**
 * \brief Main class of this project.
 */
class Application
{
    private:
        /**
         * \brief Constant for time per frame limits.
         */
        static const sf::Time               TimePerFrame;

        /**
         * \brief Array of constant for backgrounds in different levels.
         */
        static const std::vector<sf::Color> mColorConstants;
    
        /**
         * \brief Active window to using.
         */
        sf::RenderWindow                    _window;

        /**
         * \brief Variable, which contains all textures for this app.
         */
        TextureHolder                       _textures;

        /**
         * \brief Variable, which contains all fonts for this app.
         */
        FontHolder                          _fonts;

        /**
        * \brief Variable, which contains all sounds for this app.
        */
        SoundBufferHolder                   _sounds;

        /**
         * \brief Variable, which contains all player's data.
         */
        PlayerInfo                          _playerInfo;

        /**
         * \brief Structure for containing all settings.
         */
        State::CurrentSettings              _currentSettings;
    
        /**
         * \brief State stack, can contain and update different states (section "States").
         */
        StateStack                          _stateStack;
    
        /**
         * \brief Text for appearing current FPS of this app.
         */
        sf::Text                            _statisticsText;

        /**
         * \brief Time for updating current FPS of this app.
         */
        sf::Time                            _statisticsUpdateTime;

        /**
         * \brief Number FPS of this app.
         */
        std::size_t                         _statisticsNumFrames;


    private:
        /**
         * \brief App loop that processes all input events.
         */
        void processInput();
        
        /**
         * \brief        Update stack of the states.
         * \param[in] dt Delta time parameter.
         */
        void update(const sf::Time dt);
        
        /**
         * \brief Manipulate with window (such as drawing and changing view).
         */
        void render();
    
        /**
         * \brief        Update statictics information of the app.
         * \param[in] dt Delta time parameter.
         */
        void updateStatistics(const sf::Time dt);
        /**
         * \brief Register states for working with it.
         */
        void registerStates();


    public:
        /**
         * \brief Default constructor.
         */
             Application();

        
        /**
         * \brief Main app loop.
         */
        void run();
};

#endif // APPLICATION_HPP
