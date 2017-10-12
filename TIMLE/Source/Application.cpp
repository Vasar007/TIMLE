#include "../Include/Application.hpp"
#include "../Include/Utility.hpp"
#include "../Include/State.hpp"
#include "../Include/StateIdentifiers.hpp"
#include "../Include/TitleState.hpp"
#include "../Include/GameState.hpp"
#include "../Include/MenuState.hpp"
#include "../Include/PauseState.hpp"
#include "../Include/SettingsState.hpp"
#include "../Include/GameOverState.hpp"
#include "../Include/DialogState.hpp"
#include "../Include/LoadingState.hpp"
#include "../Include/ChoosingState.hpp"
#include "../Include/TitreState.hpp"
#include "../Include/ChangingState.hpp"


const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

const std::vector<sf::Color> Application::mColorConstants = { sf::Color(85, 170, 255), 
															  sf::Color(86, 97, 104) };

Application::Application()
: mWindow(sf::VideoMode(1280, 720), "TIMLE", sf::Style::Close)
, mTextures()
, mFonts()
, mSounds()
, mPlayerInfo()
, mCurrentSettings(mWindow.getSize(), State::WindowStyle::Close, 100.f, 
				   Fonts::Main, State::ActualLanguage::Russian)
, mStateStack(State::Context(mWindow, mTextures, mFonts, mSounds, mPlayerInfo, mCurrentSettings))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);

	mFonts.load(Fonts::Main, 	"Media/Other/Noto Serif.ttf");

	mTextures.load(Textures::TitleScreen,		"Media/Textures/Menu/back.png");
	mTextures.load(Textures::ButtonNormal,		"Media/Textures/Menu/ButtonNormal.png");
	mTextures.load(Textures::ButtonSelected,	"Media/Textures/Menu/ButtonSelected.png");
	mTextures.load(Textures::ButtonPressed,		"Media/Textures/Menu/ButtonPressed.png");
	mTextures.load(Textures::DialogBox,			"Media/Textures/Interface/DialogBox.png");

	mSounds.load(Sounds::ButtonCLick, "Media/Sounds/MenuSelectionClick.wav");

	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::Title);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call.
			if (mStateStack.isEmpty())
			{
				mWindow.close();
			}
		}

		updateStatistics(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
		}
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	switch (mPlayerInfo.getLevelNumber())
	{
		case 1:
			mWindow.clear(mColorConstants[0]);
			break;
		case 2:
			mWindow.clear(mColorConstants[1]);
			break;
		case 3:
			mWindow.clear(mColorConstants[0]);
			break;
		default:
			mWindow.clear(mColorConstants[0]);
			break;
	}
	//mWindow.clear(sf::Color(85, 170, 255));

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<LoadingState>(States::Loading);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<SettingsState>(States::Settings);
	mStateStack.registerState<GameOverState>(States::GameOver);
	mStateStack.registerState<DialogState>(States::Dialog);
	mStateStack.registerState<ChoosingState>(States::Choosing);
	mStateStack.registerState<TitreState>(States::Titre);
	mStateStack.registerState<ChanginState>(States::Changing);
}
