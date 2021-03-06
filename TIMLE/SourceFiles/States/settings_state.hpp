﻿#pragma once

#include <array>
#include <vector>

#include <SFML/Graphics/Sprite.hpp>

#include "Interface/button.hpp"
#include "Interface/container.hpp"
#include "Interface/label.hpp"
#include "Player/player.hpp"
#include "States/state.hpp"


class SettingsState final : public State
{
    public:
        enum Setting : std::size_t
        {
            WindowSize,
            WindowStyle,
            MusicVolume,
            FontType,
            Language,
            DebugMode,
            SettingCount
        };


    private:
        sf::Sprite                                                      _backgroundSprite;
        GUI::Container                                                  _guiContainer;
        std::array<GUI::Button::shPtr,
            static_cast<std::size_t>(PlayerInfo::Action::ActionCount)>  _bindingButtons;

        std::array<GUI::Label::shPtr,
            static_cast<std::size_t>(PlayerInfo::Action::ActionCount)>  _bindingLabels;

        std::array<GUI::Button::shPtr, Setting::SettingCount>           _settingButtons;
        std::array<GUI::Label::shPtr, Setting::SettingCount>            _settingLabels;
        sf::Sound                                                       _sound;
        sf::RenderWindow&                                               _window;
        std::vector<sf::VideoMode>                                      _videoModes;
        std::vector<sf::String>                                         _labelsNames;
        CurrentSettings*                                                _currentSettings;
    
    
    private:
        void updateLabels();
        void addButtonLabel(const PlayerInfo::Action action, const float y, const sf::String& text,
                            const Context context);
        void addButtonLabel(const Setting setting, const float y, const sf::String& text,
                            const Context context);


    public:
             SettingsState(StateStack& stack, Context context);

        void draw() override;
        bool update(const sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;
};
