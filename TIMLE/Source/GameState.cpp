#include "../Include/GameState.hpp"


// Declaration static field in this file scope. Needs to be once.

AudioManager State::mAudioManager;

GameState::GameState(StateStack& stack, const Context context)
: State(stack, context)
, _world(*context.mWindow, *context.mTextures, *context.mFonts, *context.mSounds, 
         context.mPlayerInfo, mAudioManager, context.mCurrentSettings->mDebugMode,
         context.mPlayerInfo->getLevelNumber())
, _playerInfo(*context.mPlayerInfo)
{
    _playerInfo.setGameStatus(PlayerInfo::GameStatus::GameRunning);
}

void GameState::draw()
{
    _world.draw();
}

bool GameState::update(const sf::Time dt)
{
    _world.update(dt);

    if(!_world.hasAlivePlayer())
    {
        if (mAudioManager.isPlaying())
        {
            mAudioManager.stopAllMusics();
        }

        if (_playerInfo.mLivesCount == 0)
        {
            _playerInfo.setGameStatus(PlayerInfo::GameOver);
            _playerInfo.resetData();
            requestStackPush(States::ID::GameOver);
        }
        else
        {
            _playerInfo.ressurectPlayer();
        }
        
    }
    else if(_world.hasPlayerReachedEnd())
    {
        if (mAudioManager.isPlaying())
        {
            mAudioManager.stopAllMusics();
        }

        _playerInfo.setGameStatus(PlayerInfo::LevelComplete);
        // TODO: make the transfer progress to the next levels.
        _playerInfo.resetData();
        requestStackPush(States::ID::GameOver);
    }

    _world.handleEvent();

    if (_playerInfo.mDialogNumber != 0)
    {
        requestStackPush(States::ID::Dialog);
    }

    if (_playerInfo.getLevelNumber() != _world.getLevelNumber())
    {
        _playerInfo.setLevelNumber(_world.getLevelNumber());
    }

    return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
    // Game input handling.
    _world.handleEvent();

    // Escape pressed, trigger the pause screen.
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestStackPush(States::ID::Pause);
    }

    return true;
}
