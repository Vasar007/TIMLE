#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "State.hpp"
#include "World.hpp"


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

#endif // GAMESTATE_HPP
