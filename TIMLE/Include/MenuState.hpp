#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/Sound.hpp>

#include "State.hpp"
#include "Container.hpp"


class MenuState final : public State
{
    private:
        sf::Sprite     _backgroundSprite;
        GUI::Container _guiContainer;
        sf::Sound      _sound;


    public:
             MenuState(StateStack& stack, Context context);

        void draw() override;
        bool update(const sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;
};

#endif // MENUSTATE_HPP
