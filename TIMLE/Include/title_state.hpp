#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "state.hpp"


class TitleState final : public State
{
    private:
        sf::Sprite _backgroundSprite;
        sf::Text   _text;

        bool       _showText;
        sf::Time   _textEffectTime;


    public:
             TitleState(StateStack& stack, Context context);

        void draw() override;
        bool update(const sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;
};
