#pragma once

#include <vector>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Text.hpp>

#include "container.hpp"
#include "player_info.hpp"
#include "state.hpp"


class ChoosingState final : public State
{
private:
    int                     _textNumber;

    sf::Text                _text;
    GUI::Container          _guiContainer;
    sf::Sound               _sound;

    std::vector<sf::String> _dialogText;
    std::vector<sf::String> _dialogTalking;

    PlayerInfo&             _playerInfo;


public:
    ChoosingState(StateStack& stack, Context context);

    void addText(const sf::String& text);
    void setText(const std::size_t number);
    void draw() override;
    bool update(const sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;
};
