#include "../Include/AudioManager.hpp"


AudioManager::AudioManager()
: _currentState(CurrentState::Stop)
{
    buildMusic();
}

void AudioManager::loadMusic(const Music::ID musicType, const std::string& path)
{
    auto music = std::make_unique<sf::Music>();

    music->openFromFile(path);
    music->setLoop(true);
    _musics.insertResource(musicType, std::move(music));
    _availableMusicID.push_back(musicType);
}

void AudioManager::buildMusic()
{
    loadMusic(Music::ID::MainMenuTheme,      "Media/Sounds/MainMenuTheme.ogg");
    loadMusic(Music::ID::FirstMainMusic,     "Media/Sounds/FirstMainMusic.ogg");
    loadMusic(Music::ID::FirstBossMusic,     "Media/Sounds/FirstBossMusic.ogg");
    loadMusic(Music::ID::FirstMiniBossMusic, "Media/Sounds/FirstMiniBossMusic.ogg");
}

void AudioManager::stopAllMusics()
{
    for (const auto& music_id : _currentMusicID)
    {
        _musics.get(music_id).stop();
    }
    _currentMusicID.clear();
    _currentState = CurrentState::Stop;
}

void AudioManager::setMusic(const Music::ID musicType)
{
    if (std::find(_currentMusicID.begin(), _currentMusicID.end(), musicType) !=
        _currentMusicID.end())
    {
        return;
    }

    stopAllMusics();
    _currentMusicID.push_back(musicType);
    _musics.get(musicType).play();
    _currentState = CurrentState::Play;
}

std::vector<Music::ID> AudioManager::getPlayingMusicType() const
{
    return _currentMusicID;
}

bool AudioManager::isPlaying() const
{
    return _currentState == CurrentState::Play;
}

void AudioManager::setMusicVolume(const float volume)
{
    for (const auto& music_id : _availableMusicID)
    {
        _musics.get(music_id).setVolume(volume);
    }
}

