#include "../Include/GameState.hpp"


// Declaration static field in this file scope. Needs to be once.
AudioManager State::mAudioManager;

GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, mWorld(*context.mWindow, *context.mTextures, *context.mFonts, *context.mSounds, context.mPlayerInfo, mAudioManager)
, mPlayerInfo(*context.mPlayerInfo)
{
	mPlayerInfo.setGameStatus(PlayerInfo::GameRunning);
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);

	if(!mWorld.hasAlivePlayer())
	{
		if (mAudioManager.isPlaying())
			mAudioManager.stopAllMusics();
		mPlayerInfo.setGameStatus(PlayerInfo::GameOver);
		mPlayerInfo.resetData();
		requestStackPush(States::GameOver);
	}
	else if(mWorld.hasPlayerReachedEnd())
	{
		if (mAudioManager.isPlaying())
			mAudioManager.stopAllMusics();
		mPlayerInfo.setGameStatus(PlayerInfo::LevelComplete);
		mPlayerInfo.resetData();	// —делать перенос прогресса на последующие уровни.
		requestStackPush(States::GameOver);
	}

	mWorld.handleEvent();

	if (mPlayerInfo.mDialogNumber != 0)
	{
		requestStackPush(States::Dialog);
	}

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling.
	mWorld.handleEvent();

	// Escape pressed, trigger the pause screen.
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
}