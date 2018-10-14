#include "changing_state.hpp"
#include "choosing_state.hpp"
#include "dialog_state.hpp"
#include "game_over_state.hpp"
#include "game_state.hpp"
#include "loading_state.hpp"
#include "menu_state.hpp"
#include "pause_state.hpp"
#include "state.hpp"
#include "state_identifiers.hpp"
#include "settings_state.hpp"
#include "title_state.hpp"
#include "titre_state.hpp"
#include "transition_state.hpp"
#include "utility.hpp"

#include "application.hpp"


const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

const std::vector<sf::Color> Application::mColorConstants = { sf::Color(85, 170, 255), 
                                                              sf::Color(86, 97, 104) };

Application::Application()
: _window(sf::VideoMode(1280, 720), "TIMLE", sf::Style::Close)
, _currentSettings(_window.getSize(), State::WindowStyle::Close, 100.f, 
                   Fonts::ID::Main, State::ActualLanguage::Russian, State::DebugMode::DebugOff)
, _stateStack(State::Context(_window, _textures, _fonts, _sounds, _playerInfo, _currentSettings))
, _statisticsNumFrames(0)
{
    _window.setKeyRepeatEnabled(false);
    _window.setVerticalSyncEnabled(true);

    _fonts.load(Fonts::ID::Main,                 "Media/Other/Noto Serif.ttf");

    _textures.load(Textures::ID::TitleScreen,    "Media/Textures/Menu/Back.png");
    _textures.load(Textures::ID::ButtonNormal,   "Media/Textures/Menu/ButtonNormal.png");
    _textures.load(Textures::ID::ButtonSelected, "Media/Textures/Menu/ButtonSelected.png");
    _textures.load(Textures::ID::ButtonPressed,  "Media/Textures/Menu/ButtonPressed.png");
    _textures.load(Textures::ID::DialogBox,      "Media/Textures/Interface/DialogBox.png");

    _sounds.load(Sounds::ID::ButtonCLick,        "Media/Sounds/MenuSelectionClick.wav");

    _statisticsText.setFont(_fonts.get(Fonts::ID::Main));
    _statisticsText.setPosition(5.f, 5.f);
    _statisticsText.setCharacterSize(10);

    registerStates();
    _stateStack.pushState(States::ID::Title);
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (_window.isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            update(TimePerFrame);

            // Check inside this loop, because stack might be empty before update() call.
            if (_stateStack.isEmpty())
            {
                _window.close();
            }
        }

        updateStatistics(dt);
        render();
    }
}

void Application::processInput()
{
    sf::Event event{};
    while (_window.pollEvent(event))
    {
        _stateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
        {
            _window.close();
        }
    }
}

void Application::update(const sf::Time dt)
{
    _stateStack.update(dt);
}

void Application::render()
{
    switch (_playerInfo.getLevelNumber())
    {
        case 1:
            _window.clear(mColorConstants.at(0));
            break;
        case 2:
            _window.clear(mColorConstants.at(1));
            break;
        case 3:
            _window.clear(mColorConstants.at(0));
            break;
        default:
            _window.clear(mColorConstants.at(0));
            break;
    }
    //mWindow.clear(sf::Color(85, 170, 255));

    _stateStack.draw();

    _window.setView(_window.getDefaultView());
    _window.draw(_statisticsText);

    _window.display();
}

void Application::updateStatistics(const sf::Time dt)
{
    _statisticsUpdateTime += dt;
    _statisticsNumFrames += 1;
    if (_statisticsUpdateTime >= sf::seconds(1.0f))
    {
        _statisticsText.setString("FPS: " + utils::toString(_statisticsNumFrames));

        _statisticsUpdateTime -= sf::seconds(1.0f);
        _statisticsNumFrames = 0;
    }
}

void Application::registerStates()
{
    _stateStack.registerState<TitleState>(States::ID::Title);
    _stateStack.registerState<MenuState>(States::ID::Menu);
    _stateStack.registerState<GameState>(States::ID::Game);
    _stateStack.registerState<LoadingState>(States::ID::Loading);
    _stateStack.registerState<PauseState>(States::ID::Pause);
    _stateStack.registerState<SettingsState>(States::ID::Settings);
    _stateStack.registerState<GameOverState>(States::ID::GameOver);
    _stateStack.registerState<DialogState>(States::ID::Dialog);
    _stateStack.registerState<ChoosingState>(States::ID::Choosing);
    _stateStack.registerState<TitreState>(States::ID::Titre);
    _stateStack.registerState<ChanginState>(States::ID::ChangingSettings);
    _stateStack.registerState<TransitionState>(States::ID::Transit);
}
