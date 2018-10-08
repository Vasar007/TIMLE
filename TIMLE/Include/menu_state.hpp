#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "container.hpp"
#include "state.hpp"


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

#endif // MENU_STATE_HPP
