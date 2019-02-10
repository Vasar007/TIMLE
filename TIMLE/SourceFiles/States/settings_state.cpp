#include <SFML/Graphics/RenderWindow.hpp>

#include "Mechanics/resource_holder.hpp"
#include "Utils/utility.hpp"

#include "States/settings_state.hpp"


SettingsState::SettingsState(StateStack& stack, Context context)
: State(stack, context)
, _window(*context.mWindow)
, _videoModes(sf::VideoMode::getFullscreenModes())
, _labelsNames(SettingsState::Setting::SettingCount)
, _currentSettings(context.mCurrentSettings)
{
    // Define some variables for convenience.
    const auto& windowSize(context.mWindow->getView().getSize());
    const auto& windowCenter(context.mWindow->getView().getCenter());

    // Reset array of default strings.
    std::fill(_labelsNames.begin(), _labelsNames.end(), "");

    // Set default label names.
    _labelsNames.at(SettingsState::Setting::WindowSize) = utils::toString(_window.getSize().x) + " x " +
                                                          utils::toString(_window.getSize().y);
    _labelsNames.at(SettingsState::Setting::WindowStyle) = "Window Mode";
    _labelsNames.at(SettingsState::Setting::MusicVolume) = utils::toString(_currentSettings->mMusicVolume);
    _labelsNames.at(SettingsState::Setting::FontType) = "Noto Serif";
    _labelsNames.at(SettingsState::Setting::Language) = "Russian";
    _labelsNames.at(SettingsState::Setting::DebugMode) = L"Выключен";

    _backgroundSprite.setTexture(context.mTextures->get(Textures::ID::TitleScreen));
    _sound.setBuffer(context.mSounds->get(Sounds::ID::ButtonCLick));

    const float border = windowCenter.y - (windowSize.y / 2.f);
    
    // Build key binding buttons and labels.
    addButtonLabel(PlayerInfo::Action::MoveLeft,      border + 100.f, L"Двигаться влево", context);
    addButtonLabel(PlayerInfo::Action::MoveRight,     border + 150.f, L"Двигаться вправо", context);
    addButtonLabel(PlayerInfo::Action::MoveUp,        border + 200.f, L"Прыжок", context);
    addButtonLabel(PlayerInfo::Action::MoveDown,      border + 250.f, L"Спуститься", context);
    addButtonLabel(PlayerInfo::Action::Fire,          border + 300.f, L"Атака", context);
    addButtonLabel(PlayerInfo::Action::LaunchMissile, border + 350.f, L"Особая атака", context);
    addButtonLabel(PlayerInfo::Action::UseDoor,       border + 400.f, L"Взаимодействовать", context);

    addButtonLabel(SettingsState::Setting::WindowSize,  border + 100.f, L"Разрешение экрана", context);
    addButtonLabel(SettingsState::Setting::WindowStyle, border + 150.f, L"Режим окна", context);
    addButtonLabel(SettingsState::Setting::MusicVolume, border + 200.f, L"Громкость музыки", context);
    addButtonLabel(SettingsState::Setting::FontType,    border + 250.f, L"Шрифт", context);
    addButtonLabel(SettingsState::Setting::Language,    border + 300.f, L"Язык", context);
    addButtonLabel(SettingsState::Setting::DebugMode,   border + 350.f, L"Отладочный режим", context);

    updateLabels();

    // Button for changing settings.
    auto changeWindowSize = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
                                                          *context.mSounds);
    changeWindowSize->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, 
                                  windowCenter.y + (windowSize.y / 2.f) - 150.f);
    changeWindowSize->setText(L"Применить");
    changeWindowSize->setCallback([this]()
    {
        if (_currentSettings->mHasAnyChanges)
        {
            // Recreate a window and apply all changes.
            _window.create(sf::VideoMode(_currentSettings->mWindowSize.x, 
                                         _currentSettings->mWindowSize.y),
                                        "TIMLE", _currentSettings->mWindowStyle);
            _window.setVerticalSyncEnabled(true);

            mAudioManager.setMusicVolume(_currentSettings->mMusicVolume);
            
            // Update all labels names.
            _labelsNames.at(SettingsState::Setting::WindowSize) = utils::toString(_window.getSize().x)
                                                            + " x " + utils::toString(_window.getSize().y);

            switch (_currentSettings->mWindowStyle)
            {
                case State::WindowStyle::Fullscreen:
                    _labelsNames.at(SettingsState::Setting::WindowStyle) = "Fullscreen";
                    break;
                case State::WindowStyle::Close:
                    _labelsNames.at(SettingsState::Setting::WindowStyle) = "Window Mode";
                    break;
                default:
                    _labelsNames.at(SettingsState::Setting::WindowStyle) = "Unknown parameter";
                    break;
            }
            
            _labelsNames[SettingsState::Setting::MusicVolume] = 
                utils::toString(_currentSettings->mMusicVolume);

            switch (_currentSettings->mFontType)
            {
                case Fonts::ID::Main:
                    _labelsNames.at(SettingsState::Setting::FontType) = "Noto Serif";
                    break;
                default:
                    _labelsNames.at(SettingsState::Setting::FontType) = "Unknown parameter";
                    break;
            }
            
            switch (_currentSettings->mLanguage)
            {
                case State::ActualLanguage::Russian:
                    _labelsNames.at(SettingsState::Setting::Language) = "Russian";
                    break;
                case State::ActualLanguage::English:
                    _labelsNames.at(SettingsState::Setting::Language) = "English";
                    break;
                default:
                    _labelsNames.at(SettingsState::Setting::Language) = "Unknown parameter";
                    break;
            }

            switch (_currentSettings->mDebugMode)
            {
                case State::DebugMode::DebugOn:
                    _labelsNames.at(SettingsState::Setting::DebugMode) = L"Включён";
                    break;
                case State::DebugMode::DebugOff:
                    _labelsNames.at(SettingsState::Setting::DebugMode) = L"Выключен";
                    break;
                default:
                    _labelsNames.at(SettingsState::Setting::DebugMode) = "Unknown parameter";
                    break;
            }

            updateLabels();
        }

        std::cout << "Settings were changed!\n";
    });

    // Create return button.
    auto backButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
                                                    *context.mSounds);
    backButton->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, 
                            windowCenter.y + (windowSize.y / 2.f) - 100.f);
    backButton->setText(L"Вернуться в меню");
    backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

    _guiContainer.pack(changeWindowSize);
    _guiContainer.pack(backButton);
}

void SettingsState::draw()
{
    auto& window = *getContext().mWindow;

    window.draw(_backgroundSprite);
    window.draw(_guiContainer);
}

bool SettingsState::update(sf::Time)
{
    if (_currentSettings->mHasAnyChanges)
    {
        // Update all labels names.
        _labelsNames.at(SettingsState::Setting::WindowSize) = 
            utils::toString(_currentSettings->mWindowSize.x) + " x " +
            utils::toString(_currentSettings->mWindowSize.y);

        switch (_currentSettings->mWindowStyle)
        {
            case State::WindowStyle::Fullscreen:
                _labelsNames.at(SettingsState::Setting::WindowStyle) = "Fullscreen";
                break;
            case State::WindowStyle::Close:
                _labelsNames.at(SettingsState::Setting::WindowStyle) = "Window Mode";
                break;
            default:
                _labelsNames.at(SettingsState::Setting::WindowStyle) = "Unknown parameter";
                break;
        }

        _labelsNames.at(SettingsState::Setting::MusicVolume) = utils::toString(_currentSettings->mMusicVolume);

        switch (_currentSettings->mFontType)
        {
            case Fonts::ID::Main:
                _labelsNames.at(SettingsState::Setting::FontType) = "Noto Serif";
                break;
            default:
                _labelsNames.at(SettingsState::Setting::FontType) = "Unknown parameter";
                break;
        }

        switch (_currentSettings->mLanguage)
        {
            case State::ActualLanguage::Russian:
                _labelsNames.at(SettingsState::Setting::Language) = "Russian";
                break;
            case State::ActualLanguage::English:
                _labelsNames.at(SettingsState::Setting::Language) = "English";
                break;
            default:
                _labelsNames.at(SettingsState::Setting::Language) = "Unknown parameter";
                break;
        }

        switch (_currentSettings->mDebugMode)
        {
            case State::DebugMode::DebugOn:
                _labelsNames.at(SettingsState::Setting::DebugMode) = L"Включён";
                break;
            case State::DebugMode::DebugOff:
                _labelsNames.at(SettingsState::Setting::DebugMode) = L"Выключен";
                break;
            default:
                _labelsNames.at(SettingsState::Setting::DebugMode) = "Unknown parameter";
                break;
        }

        updateLabels();
    }

    return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Return)
    {
        _sound.play();
    }

    bool isKeyBinding = false;
    
    // Iterate through all key binding buttons to see if they are being pressed,
    // waiting for the user to enter a key.
    const auto actionCount = static_cast<std::size_t>(PlayerInfo::Action::ActionCount);
    for (std::size_t action = 0; action < actionCount; ++action)
    {
        if (_bindingButtons.at(action)->isActive())
        {
            isKeyBinding = true;
            if (event.type == sf::Event::KeyReleased)
            {
                getContext().mPlayerInfo->assignKey(static_cast<PlayerInfo::Action>(action), 
                                                    event.key.code);
                _bindingButtons.at(action)->deactivate();
            }
            break;
        }
    }

    // If pressed button changed key bindings, update labels;
    // otherwise consider other buttons in container.
    if (isKeyBinding)
    {
        updateLabels();
    }
    else
    {
        _guiContainer.handleEvent(event);
    }

    return false;
}

void SettingsState::updateLabels()
{
    auto& player = *getContext().mPlayerInfo;

    const auto actionCount = static_cast<std::size_t>(PlayerInfo::Action::ActionCount);
    for (std::size_t i = 0; i < actionCount; ++i)
    {
        const sf::Keyboard::Key key = player.getAssignedKey(static_cast<PlayerInfo::Action>(i));
        _bindingLabels.at(i)->setText(utils::toString(key));
    }

    for (std::size_t i = 0; i < SettingsState::Setting::SettingCount; ++i)
    {
        _settingLabels.at(i)->setText(_labelsNames[i]);
    }
}

void SettingsState::addButtonLabel(const PlayerInfo::Action action, const float y,
                                   const sf::String& text, const Context context)
{
    // Defines some variables for convenience.
    const auto& windowSize(context.mWindow->getView().getSize());
    const auto& windowCenter(context.mWindow->getView().getCenter());

    const auto index = static_cast<std::size_t>(action);
    _bindingButtons.at(index) = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
                                                              *context.mSounds);
    _bindingButtons.at(index)->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, y);
    _bindingButtons.at(index)->setText(text);
    _bindingButtons.at(index)->setToggle(true);

    _bindingLabels.at(index) = std::make_shared<GUI::Label>("", *context.mFonts);
    _bindingLabels.at(index)->setPosition(windowCenter.x - (windowSize.x / 2.f) + 320.f, y + 15.f);

    _guiContainer.pack(_bindingButtons.at(index));
    _guiContainer.pack(_bindingLabels.at(index));
}

void SettingsState::addButtonLabel(const Setting setting, const float y,
                                   const sf::String& text, const Context context)
{
    // Defines some variables for convenience.
    const auto& windowSize(context.mWindow->getView().getSize());
    const auto& windowCenter(context.mWindow->getView().getCenter());

    _settingButtons.at(setting) = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
                                                                *context.mSounds);
    _settingButtons.at(setting)->setPosition(windowCenter.x - (windowSize.x / 2.f) + 500.f, y);
    _settingButtons.at(setting)->setText(text);
    _settingButtons.at(setting)->setCallback([this, setting]()
    {
        _currentSettings->mPressedButton = setting;
        requestStackPush(States::ID::ChangingSettings);
    });

    _settingLabels.at(setting) = std::make_shared<GUI::Label>("", *context.mFonts);
    _settingLabels.at(setting)->setPosition(windowCenter.x - (windowSize.x / 2.f) + 720.f, y + 15.f);

    _guiContainer.pack(_settingButtons.at(setting));
    _guiContainer.pack(_settingLabels.at(setting));
}
