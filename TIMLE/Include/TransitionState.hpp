#ifndef TRANSITIONSTATE_HPP
#define TRANSITIONSTATE_HPP

#include <SFML/Graphics/Text.hpp>

#include "State.hpp"


class TransitionState final : public State
{
    private:
        int         _opacity;
        bool        _appearence;

        sf::Time    _elapsedTime;

        PlayerInfo& _playerInfo;


    public:
             TransitionState(StateStack& stack, Context context);

        void draw() override;
        bool update(const sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;
};

#endif // TRANSITIONSTATE_HPP
