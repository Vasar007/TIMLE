﻿#ifndef GAME_OVER_STATE_HPP
#define GAME_OVER_STATE_HPP

#include <SFML/Graphics/Text.hpp>

#include "player_info.hpp"
#include "state.hpp"


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
        bool handleEvent(const sf::Event& event) override;
};

#endif // GAMEOVERSTATE_HPP