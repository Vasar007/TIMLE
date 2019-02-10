#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Interface/container.hpp"
#include "Player/player_info.hpp"
#include "States/state.hpp"


class PauseState final : public State
{
    private:
        sf::Sprite     _backgroundSprite;
        sf::Text       _pausedText;
        GUI::Container _guiContainer;
        sf::Sound      _sound;

        PlayerInfo&    _playerInfo;


    public:
             PauseState(StateStack& stack, Context context);

        void draw() override;
        bool update(const sf::Time dt) noexcept override;
        bool handleEvent(const sf::Event& event) override;
};
