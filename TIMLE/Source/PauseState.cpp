#include "../Include/PauseState.hpp"
#include "../Include/Button.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context)
, mBackgroundSprite()
, mPausedText()
, mGUIContainer()
, mSound()
, mPlayerInfo(context.mPlayerInfo)
{
	sf::Font& font = context.mFonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.mWindow->getSize());

	mSound.setBuffer(context.mSounds->get(Sounds::ButtonCLick));

	mPausedText.setFont(font);
	mPausedText.setString(L"Игра приостановлена");	
	mPausedText.setCharacterSize(70);
	centerOrigin(mPausedText);
	mPausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

	auto returnButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
													  *context.mSounds);
	returnButton->setPosition(0.5f * windowSize.x - 100.f, 0.4f * windowSize.y + 75.f);
	returnButton->setText(L"Вернуться");
	returnButton->setCallback([this] ()
	{
		requestStackPop();
	});

	auto backToMenuButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
														  *context.mSounds);
	backToMenuButton->setPosition(0.5f * windowSize.x - 100.f, 0.4f * windowSize.y + 125.f);
	backToMenuButton->setText(L"Выйти в меню");
	backToMenuButton->setCallback([this] ()
	{
		mPlayerInfo->mShowedDialogs.clear();
		mPlayerInfo->resetData();
		requestStateClear();
		requestStackPush(States::Menu);
	});

	mGUIContainer.pack(returnButton);
	mGUIContainer.pack(backToMenuButton);
}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().mWindow;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mPausedText);
	window.draw(mGUIContainer);
}

bool PauseState::update(sf::Time)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
	{
		mSound.play();
	}

	mGUIContainer.handleEvent(event);
	return false;
}