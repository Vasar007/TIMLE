#include <SFML/Graphics/RenderWindow.hpp>

#include "resource_holder.hpp"
#include "utility.hpp"

#include "title_state.hpp"


TitleState::TitleState(StateStack& stack, Context context)
: State(stack, context)
, _showText(true)
, _textEffectTime(sf::Time::Zero)
{
    _backgroundSprite.setTexture(context.mTextures->get(Textures::ID::TitleScreen));

    _text.setFont(context.mFonts->get(Fonts::ID::Main));
    _text.setString(L"Нажмите любую кнопку для продолжения");
    centerOrigin(_text);
    _text.setPosition(sf::Vector2f(context.mWindow->getSize() / 2u));
}

void TitleState::draw()
{
    auto& window = *getContext().mWindow;
    window.draw(_backgroundSprite);

    if (_showText)
    {
        window.draw(_text);
    }
}

bool TitleState::update(const sf::Time dt)
{
    _textEffectTime += dt;

    if (_textEffectTime >= sf::seconds(0.5f))
    {
        _showText = !_showText;
        _textEffectTime = sf::Time::Zero;
    }

    return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
    // If any key is pressed, trigger the next screen.
    if (event.type == sf::Event::KeyReleased)
    {
        requestStackPop();
        requestStackPush(States::ID::Menu);
    }

    return true;
}
