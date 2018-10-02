#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "../Include/TransitionState.hpp"
#include "../Include/Utility.hpp"
#include "../Include/Player.hpp"
#include "../Include/ResourceHolder.hpp"


TransitionState::TransitionState(StateStack& stack, Context context)
: State(stack, context)
, _opacity(0)
, _appearence(false)
, _elapsedTime(sf::Time::Zero)
, _playerInfo(*context.mPlayerInfo)
{
}

void TransitionState::draw()
{
    auto& window = *getContext().mWindow;

    if (!_appearence)
    {
        window.setView(window.getDefaultView());
    }

    if (!_appearence && _opacity < 255)
    {
        _opacity += 5;
    }

    // Create dark, semitransparent background.
    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(_opacity)));
    backgroundShape.setSize(window.getDefaultView().getSize());

    window.draw(backgroundShape);
}

bool TransitionState::update(const sf::Time dt)
{
    // Show state for 5 seconds, after move hero to transit position.
    _elapsedTime += dt;

    _playerInfo.getPlayer()->mSpeed = 0.f;
    _playerInfo.getPlayer()->dx = 0.f;
    _playerInfo.getPlayer()->dy = 0.f;

    if (!_appearence && _elapsedTime > sf::seconds(1))
    {
        _appearence = true;
        _playerInfo.getPlayer()->x = _playerInfo.mTransitPos.first;
        _playerInfo.getPlayer()->y = _playerInfo.mTransitPos.second;
        _playerInfo.getPlayer()->update(static_cast<float>(_elapsedTime.asMilliseconds()));

    }
    else if (_appearence)
    {
        requestStackPop();
    }

    return true;
}

bool TransitionState::handleEvent(const sf::Event&)
{
    return false;
}
