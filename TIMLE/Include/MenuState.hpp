#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>


class MenuState final : public State
{
	private:
		sf::Sprite		mBackgroundSprite;
		GUI::Container	mGUIContainer;
		sf::Sound		mSound;


	public:
						MenuState(StateStack& stack, Context context);

		void			draw() override;
		bool			update(sf::Time dt) override;
		bool			handleEvent(const sf::Event& event) override;
};

#endif // MENUSTATE_HPP
