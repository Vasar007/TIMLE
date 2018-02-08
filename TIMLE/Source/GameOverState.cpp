#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "../Include/GameOverState.hpp"
#include "../Include/Utility.hpp"
#include "../Include/Player.hpp"
#include "../Include/ResourceHolder.hpp"


GameOverState::GameOverState(StateStack& stack, Context context)
: State(stack, context)
, _opacity(0)
, _elapsedTime(sf::Time::Zero)
, _playerInfo(*context.mPlayerInfo)
{
	auto& font = context.mFonts->get(Fonts::ID::Main);
	const sf::Vector2f windowSize(context.mWindow->getSize());

	_gameOverText.setFont(font);
	if (context.mPlayerInfo->getGameStatus() == PlayerInfo::GameOver)
	{
		_gameOverText.setString(L"Игра окончена!");
	}
	else
	{
		_gameOverText.setString(L"Уровень пройден!");
	}

	_gameOverText.setCharacterSize(70);
	centerOrigin(_gameOverText);
	_gameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void GameOverState::draw()
{
	auto& window = *getContext().mWindow;
	window.setView(window.getDefaultView());

	if (_opacity < 255)
	{
		_opacity += 5;
	}

	_playerInfo.getPlayer()->draw(window);

	// Create dark, semitransparent background.
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(_opacity)));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(_gameOverText);
}

bool GameOverState::update(const sf::Time dt)
{
	// Show state for 5 seconds, after return to menu.
	_elapsedTime += dt;

	_playerInfo.getPlayer()->dx = 0.09f;
	_playerInfo.getPlayer()->dy = 0.f;
	_playerInfo.getPlayer()->mState = Player::State::Right;
	_playerInfo.getPlayer()->update(static_cast<float>(_elapsedTime.asMilliseconds()));

	if (_elapsedTime > sf::seconds(5))
	{
		_playerInfo.mShowedDialogs.clear();
		requestStateClear();
		requestStackPush(States::ID::Menu);
	}

	return false;
}

bool GameOverState::handleEvent(const sf::Event&)
{
	return false;
}
