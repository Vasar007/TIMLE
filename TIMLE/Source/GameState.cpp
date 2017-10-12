#include "../Include/GameState.hpp"


// Declaration static field in this file scope. Needs to be once.

AudioManager State::mAudioManager;

GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, mWorld(*context.mWindow, *context.mTextures, *context.mFonts, *context.mSounds, 
		 context.mPlayerInfo, mAudioManager)
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
		{
			mAudioManager.stopAllMusics();
		}

		if (mPlayerInfo.mLivesCount == 0)
		{
			mPlayerInfo.setGameStatus(PlayerInfo::GameOver);
			mPlayerInfo.resetData();
			requestStackPush(States::GameOver);
		}
		else
		{
			mPlayerInfo.ressurectPlayer();
		}
		
	}
	else if(mWorld.hasPlayerReachedEnd())
	{
		if (mAudioManager.isPlaying())
		{
			mAudioManager.stopAllMusics();
		}

		mPlayerInfo.setGameStatus(PlayerInfo::LevelComplete);
		// TODO: make the transfer progress to the next levels.
		mPlayerInfo.resetData();	
		requestStackPush(States::GameOver);
	}

	mWorld.handleEvent();

	if (mPlayerInfo.mDialogNumber != 0)
	{
		requestStackPush(States::Dialog);
	}

	if (mPlayerInfo.getLevelNumber() != mWorld.getLevelNumber())
	{
		mPlayerInfo.setLevelNumber(mWorld.getLevelNumber());
	}

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling.
	mWorld.handleEvent();

	// Escape pressed, trigger the pause screen.
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		requestStackPush(States::Pause);
	}

	return true;
}
