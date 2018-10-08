#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "player_info.hpp"
#include "state.hpp"
#include "world.hpp"


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

#endif // GAME_STATE_HPP
