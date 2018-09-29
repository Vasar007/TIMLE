#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include <SFML/Graphics/Text.hpp>

#include "State.hpp"


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
