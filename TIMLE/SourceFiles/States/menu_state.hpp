#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Interface/container.hpp"
#include "States/state.hpp"


class MenuState final : public State
{
    private:
        sf::Sprite     _backgroundSprite;
        GUI::Container _guiContainer;
        sf::Sound      _sound;


    public:
             MenuState(StateStack& stack, Context context);

        void draw() override;
        bool update(const sf::Time dt) noexcept override;
        bool handleEvent(const sf::Event& event) override;
};
