#pragma once

#include <SFML/Graphics/Text.hpp>

#include "Player/player_info.hpp"
#include "States/state.hpp"


class GameOverState final : public State
{
    private:
        int         _opacity;

        sf::Text    _gameOverText;
        sf::Time    _elapsedTime;

        PlayerInfo& _playerInfo;


    public:
             GameOverState(StateStack& stack, Context context);

        void draw() override;
        bool update(const sf::Time dt) override;
        bool handleEvent(const sf::Event& event) noexcept override;
};
