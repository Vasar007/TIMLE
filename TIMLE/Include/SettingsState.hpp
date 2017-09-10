#ifndef SETTINGSSTATE_HPP
#define SETTINGSSTATE_HPP

#include "State.hpp"
#include "Player.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Label.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>


class SettingsState : public State
{
	private:
		sf::Sprite												mBackgroundSprite;
		GUI::Container											mGUIContainer;
		std::array<GUI::Button::Ptr, PlayerInfo::ActionCount>	mBindingButtons;
		std::array<GUI::Label::Ptr, PlayerInfo::ActionCount> 	mBindingLabels;
		sf::Sound												mSound;
	
	
	private:
		void							updateLabels();
		void							addButtonLabel(PlayerInfo::Action action, float y, const sf::String& text, Context context);


	public:
										SettingsState(StateStack& stack, Context context);

		virtual void					draw();
		virtual bool					update(sf::Time dt);
		virtual bool					handleEvent(const sf::Event& event);
};

#endif // SETTINGSSTATE_HPP
