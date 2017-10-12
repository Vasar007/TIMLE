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


class SettingsState final : public State
{
	public:
		enum Setting
		{
			WindowSize,
			WindowStyle,
			MusicVolume,
			FontType,
			Language,
			SettingCount
		};


	private:
		sf::Sprite												mBackgroundSprite;
		GUI::Container											mGUIContainer;
		std::array<GUI::Button::Ptr, PlayerInfo::ActionCount>	mBindingButtons;
		std::array<GUI::Label::Ptr, PlayerInfo::ActionCount> 	mBindingLabels;
		std::array<GUI::Button::Ptr, SettingCount>				mSettingButtons;
		std::array<GUI::Label::Ptr, SettingCount> 				mSettingLabels;
		sf::Sound												mSound;
		sf::RenderWindow&										mWindow;
		std::vector<sf::VideoMode>								mVideoModes;
		std::vector<sf::String>									mLabelsNames;
		CurrentSettings*										mCurrentSettings;
	
	
	private:
		void					updateLabels();
		void					addButtonLabel(PlayerInfo::Action action, float y, 
											   const sf::String& text, Context context);
		void					addButtonLabel(Setting setting, float y, 
											   const sf::String& text, Context context);


	public:
								SettingsState(StateStack& stack, Context context);

		void					draw() override;
		bool					update(sf::Time dt) override;
		bool					handleEvent(const sf::Event& event) override;
};

#endif // SETTINGSSTATE_HPP
