#include <SFML/Graphics/RenderWindow.hpp>

#include "resource_holder.hpp"
#include "utility.hpp"

#include "titre_state.hpp"


TitreState::TitreState(StateStack& stack, Context context)
: State(stack, context)
, _textEffectTime(sf::Time::Zero)
, _textAppearenceTime(sf::Time::Zero)
{
    _backgroundSprite.setTexture(context.mTextures->get(Textures::ID::TitleScreen));

    _text.setFont(context.mFonts->get(Fonts::ID::Main));
    _text.setString(L"\t\t\t\t\t\t\t\t\tSoftForAll\nАвторы:\nВасильев Василий\nЗыков Артём\nГусев Данила\n\n\n\n\n\n\nНажмите любую кнопку для продолжения");
    setText();
    centerOrigin(_text);
    _text.setPosition(sf::Vector2f(context.mWindow->getSize().x / 2.f, 
                                   context.mWindow->getSize().y / 2.f + 350.f));
    _text.setOutlineColor(sf::Color::Black);
}

void TitreState::draw()
{
    auto& window = *getContext().mWindow;
    window.draw(_backgroundSprite);

    window.draw(_text);
}

bool TitreState::update(const sf::Time dt)
{
    _textEffectTime += dt;
    _textAppearenceTime += dt;

    if (_textEffectTime >= sf::seconds(0.01f))
    {
        _text.setPosition(_text.getPosition().x, _text.getPosition().y - 
                          static_cast<float>(_textEffectTime.asSeconds()) * 100.f);
        _textEffectTime = sf::Time::Zero;
    }

    if (_textAppearenceTime > sf::seconds(10.f))
    {
        requestStackPop();
        requestStackPush(States::ID::Menu);
    }

    return true;
}

bool TitreState::handleEvent(const sf::Event& event)
{
    // If any key is pressed, trigger the next screen.
    if (event.type == sf::Event::KeyReleased)
    {
        requestStackPop();
        requestStackPush(States::ID::Menu);
    }

    return true;
}

void TitreState::setText()
{
    sf::String text = _text.getString();
    for (std::size_t i = 0; i < text.getSize(); ++i)
    {
        if (i % static_cast<int>(_backgroundSprite.getGlobalBounds().width /
            (_text.getCharacterSize() - 5.f)) == 0 && i > 0)
        {
            //text.insert(i, "\n");
        }
    }
    _text.setString(text);
}
