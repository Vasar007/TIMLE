#include "../Include/State.hpp"
#include "../Include/StateStack.hpp"


State::CurrentSettings::CurrentSettings(sf::Vector2u windowSize, WindowStyle windowStyle, 
										float musicVolume, Fonts::ID fontType, 
										ActualLanguage language)
: mWindowSize(windowSize)
, mWindowStyle(windowStyle)
, mMusicVolume(musicVolume)
, mFontType(fontType)
, mLanguage(language)
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

State::State(StateStack& stack, Context context)
: mStack(&stack)
, mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}
