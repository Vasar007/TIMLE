#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "state.hpp"


class TitreState final : public State
{
    private:
        sf::Sprite _backgroundSprite;
        sf::Text   _text;
    
        sf::Time   _textEffectTime;
        sf::Time   _textAppearenceTime;


    private:
        void setText();


    public:
             TitreState(StateStack& stack, Context context);

        void draw() override;
        bool update(const sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;
};
