﻿#pragma once

#include <vector>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Interface/container.hpp"
#include "Player/player_info.hpp"
#include "States/state.hpp"


class DialogState final : public State
{
    private:
        std::size_t             _textNumber;

        bool                    _choosing;

        sf::Texture             _texture;
        sf::Sprite              _sprite;
        sf::Text                _text;
        sf::Text                _talking;
        GUI::Container          _guiContainer;
        sf::Sound               _sound;

        std::vector<sf::String> _dialogText;
        std::vector<sf::String> _dialogTalking;

        PlayerInfo&             _playerInfo;


    public:
             DialogState(StateStack& stack, Context context);

        void addText(const sf::String& text, const sf::String& talking);
        void setText(const std::size_t number);
        void draw() override;
        bool update(const sf::Time dt) noexcept override;
        bool handleEvent(const sf::Event& event) override;
};
