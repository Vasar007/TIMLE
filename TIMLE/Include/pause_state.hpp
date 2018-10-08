#ifndef PAUSE_STATE_HPP
#define PAUSE_STATE_HPP

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "container.hpp"
#include "player_info.hpp"
#include "state.hpp"


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
        bool update(const sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;
};

#endif // PAUSE_STATE_HPP
