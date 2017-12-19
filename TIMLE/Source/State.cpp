#include "../Include/State.hpp"
#include "../Include/StateStack.hpp"


State::CurrentSettings::CurrentSettings(const sf::Vector2u windowSize, 
										const WindowStyle windowStyle, const float musicVolume, 
										const Fonts::ID fontType, const ActualLanguage language,
										const DebugMode debugMode)
: mWindowSize(windowSize)
, mWindowStyle(windowStyle)
, mMusicVolume(musicVolume)
, mFontType(fontType)
, mLanguage(language)
, mDebugMode(debugMode)
, mPressedButton()
, mHasAnyChanges(false)
{
}


State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
						SoundBufferHolder& sounds, PlayerInfo& playerInfo, 
						CurrentSettings& currentSettings)
: mWindow(&window)
, mTextures(&textures)
, mFonts(&fonts)
, mSounds(&sounds)
, mPlayerInfo(&playerInfo)
, mCurrentSettings(&currentSettings)
{
}

State::State(StateStack& stack, const Context context)
: _stack(&stack)
, _context(context)
{
}

State::~State()
{
}

void State::requestStackPush(const States::ID stateID) const
{
	_stack->pushState(stateID);
}

void State::requestStackPop() const
{
	_stack->popState();
}

void State::requestStateClear() const
{
	_stack->clearStates();
}

State::Context State::getContext() const
{
	return _context;
}
