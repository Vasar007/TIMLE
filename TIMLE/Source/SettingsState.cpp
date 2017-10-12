#include "../Include/SettingsState.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


SettingsState::SettingsState(StateStack& stack, Context context)
: State(stack, context)
, mBackgroundSprite()
, mGUIContainer()
, mSound()
, mWindow(*context.mWindow)
, mVideoModes(sf::VideoMode::getFullscreenModes())
, mLabelsNames(SettingCount)
, mCurrentSettings(context.mCurrentSettings)
{
	// Define some variables for convenience.
	sf::Vector2f windowSize(context.mWindow->getView().getSize());
	sf::Vector2f windowCenter(context.mWindow->getView().getCenter());

	// Reset array of default strings.
	for (size_t i = 0; i < SettingCount; i++)
		mLabelsNames[i] = "";

	// Set default label names.
	mLabelsNames[WindowSize]	= toString(mWindow.getSize().x) + " x " +
								  toString(mWindow.getSize().y);
	mLabelsNames[WindowStyle]	= "Window Mode";
	mLabelsNames[MusicVolume]	= toString(mCurrentSettings->mMusicVolume);
	mLabelsNames[FontType]		= "Noto Serif";
	mLabelsNames[Language]		= "Russian";

	mBackgroundSprite.setTexture(context.mTextures->get(Textures::TitleScreen));
	mSound.setBuffer(context.mSounds->get(Sounds::ButtonCLick));

	float border = windowCenter.y - (windowSize.y / 2.f);
	
	// Build key binding buttons and labels.
	addButtonLabel(PlayerInfo::MoveLeft,		border + 100.f, L"Двигаться влево", context);
	addButtonLabel(PlayerInfo::MoveRight,		border + 150.f, L"Двигаться вправо", context);
	addButtonLabel(PlayerInfo::MoveUp,			border + 200.f, L"Прыжок", context);
	addButtonLabel(PlayerInfo::MoveDown,		border + 250.f, L"Спуститься", context);
	addButtonLabel(PlayerInfo::Fire,			border + 300.f, L"Атака", context);
	addButtonLabel(PlayerInfo::LaunchMissile,	border + 350.f, L"Особая атака", context);

	addButtonLabel(WindowSize,					border + 100.f, L"Разрешение экрана", context);
	addButtonLabel(WindowStyle,					border + 150.f, L"Режим окна", context);
	addButtonLabel(MusicVolume,					border + 200.f, L"Громкость музыки", context);
	addButtonLabel(FontType,					border + 250.f, L"Шрифт", context);
	addButtonLabel(Language,					border + 300.f, L"Язык", context);

	updateLabels();

	// Button for changing settings.
	auto changeWindowSize = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
														  *context.mSounds);
	changeWindowSize->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, 
								  windowCenter.y + (windowSize.y / 2.f) - 150.f);
	changeWindowSize->setText(L"Применить (test)");
	changeWindowSize->setCallback([this]()
	{
		if (mCurrentSettings->mHasAnyChanges)
		{
			// Recreate a window and apply all changes.
			mWindow.create(sf::VideoMode(mCurrentSettings->mWindowSize.x, 
										 mCurrentSettings->mWindowSize.y),
						   "TIMLE", mCurrentSettings->mWindowStyle);
			mWindow.setVerticalSyncEnabled(true);

			mAudioManager.setMusicVolume(mCurrentSettings->mMusicVolume);
			
			// Update all labels names.
			mLabelsNames[WindowSize] = toString(mWindow.getSize().x) + " x " +
									   toString(mWindow.getSize().y);

			switch (mCurrentSettings->mWindowStyle)
			{
				case Fullscreen:
					mLabelsNames[WindowStyle] = "Fullscreen";
					break;
				case Close:
					mLabelsNames[WindowStyle] = "Window Mode";
					break;
				default:
					mLabelsNames[WindowStyle] = "Unknown parameter";
					break;
			}
			
			mLabelsNames[MusicVolume] = toString(mCurrentSettings->mMusicVolume);

			switch (mCurrentSettings->mFontType)
			{
				case Fonts::Main:
					mLabelsNames[FontType] = "Noto Serif";
					break;
				default:
					mLabelsNames[FontType] = "Unknown parameter";
					break;
			}
			
			switch (mCurrentSettings->mLanguage)
			{
				case Russian:
					mLabelsNames[Language] = "Russian";
					break;
				case English:
					mLabelsNames[Language] = "English";
					break;
				default:
					mLabelsNames[Language] = "Unknown parameter";
					break;
			}

			updateLabels();
		}

		std::cout << "Settings were changed!" << std::endl;
	});

	// Create return button.
	auto backButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
													*context.mSounds);
	backButton->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, 
							windowCenter.y + (windowSize.y / 2.f) - 100.f);
	backButton->setText(L"Вернуться в меню");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

	mGUIContainer.pack(changeWindowSize);
	mGUIContainer.pack(backButton);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().mWindow;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time)
{
	if (mCurrentSettings->mHasAnyChanges)
	{
		// Update all labels names.
		mLabelsNames[WindowSize] = toString(mCurrentSettings->mWindowSize.x) + " x " +
								   toString(mCurrentSettings->mWindowSize.y);

		switch (mCurrentSettings->mWindowStyle)
		{
			case Fullscreen:
				mLabelsNames[WindowStyle] = "Fullscreen";
				break;
			case Close:
				mLabelsNames[WindowStyle] = "Window Mode";
				break;
			default:
				mLabelsNames[WindowStyle] = "Unknown parameter";
				break;
		}

		mLabelsNames[MusicVolume] = toString(mCurrentSettings->mMusicVolume);

		switch (mCurrentSettings->mFontType)
		{
			case Fonts::Main:
				mLabelsNames[FontType] = "Noto Serif";
				break;
			default:
				mLabelsNames[FontType] = "Unknown parameter";
				break;
		}

		switch (mCurrentSettings->mLanguage)
		{
			case Russian:
				mLabelsNames[Language] = "Russian";
				break;
			case English:
				mLabelsNames[Language] = "English";
				break;
			default:
				mLabelsNames[Language] = "Unknown parameter";
				break;
		}

		updateLabels();
	}

	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
	{
		mSound.play();
	}

	bool isKeyBinding = false;
	
	// Iterate through all key binding buttons to see if they are being pressed,
	// waiting for the user to enter a key.
	for (size_t action = 0; action < PlayerInfo::ActionCount; ++action)
	{
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().mPlayerInfo->assignKey(static_cast<PlayerInfo::Action>(action), 
													event.key.code);
				mBindingButtons[action]->deactivate();
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
		mGUIContainer.handleEvent(event);
	}

	return false;
}

void SettingsState::updateLabels()
{
	PlayerInfo& player = *getContext().mPlayerInfo;

	for (size_t i = 0; i < PlayerInfo::ActionCount; ++i)
	{
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<PlayerInfo::Action>(i));
		mBindingLabels[i]->setText(toString(key));
	}

	for (size_t i = 0; i < SettingCount; ++i)
	{
		mSettingLabels[i]->setText(mLabelsNames[i]);
	}
}

void SettingsState::addButtonLabel(PlayerInfo::Action action, float y, 
								   const sf::String& text, Context context)
{
	// Defines some variables for convenience.
	sf::Vector2f windowSize(context.mWindow->getView().getSize());
	sf::Vector2f windowCenter(context.mWindow->getView().getCenter());

	mBindingButtons[action] = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
															*context.mSounds);
	mBindingButtons[action]->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, y);
	mBindingButtons[action]->setText(text);
	mBindingButtons[action]->setToggle(true);

	mBindingLabels[action] = std::make_shared<GUI::Label>("", *context.mFonts);
	mBindingLabels[action]->setPosition(windowCenter.x - (windowSize.x / 2.f) + 320.f, y + 15.f);

	mGUIContainer.pack(mBindingButtons[action]);
	mGUIContainer.pack(mBindingLabels[action]);
}

void SettingsState::addButtonLabel(Setting setting, float y, 
								   const sf::String& text, Context context)
{
	// Defines some variables for convenience.
	sf::Vector2f windowSize(context.mWindow->getView().getSize());
	sf::Vector2f windowCenter(context.mWindow->getView().getCenter());

	mSettingButtons[setting] = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
															 *context.mSounds);
	mSettingButtons[setting]->setPosition(windowCenter.x - (windowSize.x / 2.f) + 500.f, y);
	mSettingButtons[setting]->setText(text);
	mSettingButtons[setting]->setCallback([this, setting]()
	{
		mCurrentSettings->mPressedButton = setting;
		requestStackPush(States::Changing);
	});

	mSettingLabels[setting] = std::make_shared<GUI::Label>("", *context.mFonts);
	mSettingLabels[setting]->setPosition(windowCenter.x - (windowSize.x / 2.f) + 720.f, y + 15.f);

	mGUIContainer.pack(mSettingButtons[setting]);
	mGUIContainer.pack(mSettingLabels[setting]);
}
