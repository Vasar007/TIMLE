#include "../Include/MenuState.hpp"
#include "../Include/Button.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
, mBackgroundSprite()
, mGUIContainer()
, mSound()
{
	// Define some variables for convenience.
	sf::Vector2f windowSize(context.mWindow->getView().getSize());
	sf::Vector2f windowCenter(context.mWindow->getView().getCenter());

	if (mAudioManager.isPlaying())
	{
		mAudioManager.stopAllMusics();
	}
	mAudioManager.setMusic(AudioManager::MainMenuTheme);

	mBackgroundSprite.setTexture(context.mTextures->get(Textures::TitleScreen));
	mSound.setBuffer(context.mSounds->get(Sounds::ButtonCLick));

	// Create the buttons for main menu.
	auto playButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
													*context.mSounds);
	playButton->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, 
							windowCenter.y - (windowSize.y / 2.f) + 300.f);
	playButton->setText(L"Играть");
	playButton->setCallback([this] ()
	{
		if (mAudioManager.isPlaying())
		{
			mAudioManager.stopAllMusics();
		}
		requestStackPop();
		requestStackPush(States::Loading);
	});

	auto settingsButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
														*context.mSounds);
	settingsButton->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, 
								windowCenter.y - (windowSize.y / 2.f) + 350.f);
	settingsButton->setText(L"Настройки");
	settingsButton->setCallback([this] ()
	{
		requestStackPush(States::Settings);
	});

	auto titreButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, 
													 *context.mSounds);
	titreButton->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, 
							 windowCenter.y - (windowSize.y / 2.f) + 400.f);
	titreButton->setText(L"Титры");
	titreButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::Titre);
	});

	auto exitButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, 
													*context.mSounds);
	exitButton->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, 
							windowCenter.y - (windowSize.y / 2.f) + 450.f);
	exitButton->setText(L"Выйти");
	exitButton->setCallback([this] ()
	{
		requestStackPop();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(titreButton);
	mGUIContainer.pack(exitButton);
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().mWindow;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
	{
		mSound.play();
	}
	
	mGUIContainer.handleEvent(event);
	return false;
}
