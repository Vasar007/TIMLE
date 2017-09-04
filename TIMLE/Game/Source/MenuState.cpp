#include "../Include/MenuState.hpp"
#include "../Include/Button.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
{
	sf::Texture& texture = context.mTextures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	auto playButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures);
	playButton->setPosition(100, 300);
	playButton->setText(L"Играть");
	playButton->setCallback([this] ()
	{
		requestStackPop();
		//requestStackPush(States::Game);	Убрано, ибо происходит загрузка ресурсов
		requestStackPush(States::Loading);
	});

	auto settingsButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures);
	settingsButton->setPosition(100, 350);
	settingsButton->setText(L"Настройки");
	settingsButton->setCallback([this] ()
	{
		requestStackPush(States::Settings);
	});

	auto titreButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures);
	titreButton->setPosition(100, 400);
	titreButton->setText(L"Титры");
	titreButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::Titre);
	});

	auto exitButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures);
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
	mGUIContainer.handleEvent(event);
	return false;
}
