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
	if (mAudioManager.isPlaying())
		mAudioManager.stopAllMusics();
	mAudioManager.setMusic(AudioManager::MainMenuTheme);

	mBackgroundSprite.setTexture(context.mTextures->get(Textures::TitleScreen));
	mSound.setBuffer(context.mSounds->get(Sounds::ButtonCLick));

	// Make the buttons for main menu.
	auto playButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
	playButton->setPosition(100, 300);
	playButton->setText(L"Играть");
	playButton->setCallback([this] ()
	{
		if (mAudioManager.isPlaying())
			mAudioManager.stopAllMusics();
		requestStackPop();
		requestStackPush(States::Loading);
	});

	auto settingsButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
	settingsButton->setPosition(100, 350);
	settingsButton->setText(L"Настройки");
	settingsButton->setCallback([this] ()
	{
		requestStackPush(States::Settings);
	});

	auto titreButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
	titreButton->setPosition(100, 400);
	titreButton->setText(L"Титры");
	titreButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::Titre);
	});

	auto exitButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
	exitButton->setPosition(100, 450);
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
