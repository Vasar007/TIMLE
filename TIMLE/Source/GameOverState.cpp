#include "../Include/GameOverState.hpp"
#include "../Include/Utility.hpp"
#include "../Include/Player.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


GameOverState::GameOverState(StateStack& stack, Context context)
: State(stack, context)
, mOpacity(0)
, mGameOverText()
, mElapsedTime(sf::Time::Zero)
, mPlayerInfo(context.mPlayerInfo)
{
	sf::Font& font = context.mFonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.mWindow->getSize());

	mGameOverText.setFont(font);
	if (context.mPlayerInfo->getGameStatus() == PlayerInfo::GameOver)
	{
		mGameOverText.setString(L"Игра окончена!");
	}
	else
	{
		mGameOverText.setString(L"Уровень пройден!");
	}

	mGameOverText.setCharacterSize(70);
	centerOrigin(mGameOverText);
	mGameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().mWindow;
	window.setView(window.getDefaultView());

	if (mOpacity < 255)
	{
		mOpacity += 5;
	}

	// Create dark, semitransparent background.
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, mOpacity));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mGameOverText);
}

bool GameOverState::update(sf::Time dt)
{
	// Show state for 5 seconds, after return to menu.
	mElapsedTime += dt;
	if (mElapsedTime > sf::seconds(5))
	{
		mPlayerInfo->mShowedDialogs.clear();
		requestStateClear();
		requestStackPush(States::Menu);
	}
	return false;
}

bool GameOverState::handleEvent(const sf::Event&)
{
	return false;
}
