#pragma once

#include "Player/player_info.hpp"
#include "Mechanics/world.hpp"
#include "States/state.hpp"


class GameState final : public State
{
    private:
        World       _world;
        PlayerInfo& _playerInfo;


    public:
             GameState(StateStack& stack, const Context context);

        void draw() override;
        bool update(const sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;
};
