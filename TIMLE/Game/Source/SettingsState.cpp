#include "../Include/SettingsState.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


SettingsState::SettingsState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.mTextures->get(Textures::TitleScreen));
	
	// Build key binding buttons and labels
	addButtonLabel(PlayerInfo::MoveLeft,		300.f, L"Двигаться влево", context);
	addButtonLabel(PlayerInfo::MoveRight,		350.f, L"Двигаться вправо", context);
	addButtonLabel(PlayerInfo::MoveUp,			400.f, L"Прыжок", context);
	addButtonLabel(PlayerInfo::MoveDown,		450.f, L"Спуститься", context);
	addButtonLabel(PlayerInfo::Fire,			500.f, L"Огонь", context);
	addButtonLabel(PlayerInfo::LaunchMissile,	550.f, L"Особая атака", context);

	updateLabels();

	auto backButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures);
	backButton->setPosition(80.f, 620.f);
	backButton->setText(L"Вернуться в меню");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

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
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;
	
	// Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key
	for (std::size_t action = 0; action < PlayerInfo::ActionCount; ++action)
	{
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().mPlayerInfo->assignKey(static_cast<PlayerInfo::Action>(action), event.key.code);
				mBindingButtons[action]->deactivate();
			}
			break;
		}
	}

	// If pressed button changed key bindings, update labels; otherwise consider other buttons in container
	if (isKeyBinding)
		updateLabels();
	else
		mGUIContainer.handleEvent(event);

	return false;
}

void SettingsState::updateLabels()
{
	PlayerInfo& player = *getContext().mPlayerInfo;

	for (std::size_t i = 0; i < PlayerInfo::ActionCount; ++i)
	{
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<PlayerInfo::Action>(i));
		mBindingLabels[i]->setText(toString(key));
	}
}

void SettingsState::addButtonLabel(PlayerInfo::Action action, float y, const sf::String& text, Context context)
{
	mBindingButtons[action] = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures);
	mBindingButtons[action]->setPosition(80.f, y);
	mBindingButtons[action]->setText(text);
	mBindingButtons[action]->setToggle(true);

	mBindingLabels[action] = std::make_shared<GUI::Label>("", *context.mFonts);
	mBindingLabels[action]->setPosition(300.f, y + 15.f);

	mGUIContainer.pack(mBindingButtons[action]);
	mGUIContainer.pack(mBindingLabels[action]);
}
