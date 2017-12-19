#include "../Include/AudioManager.hpp"


AudioManager::AudioManager()
: _currentMusic(MusicType::None)
, _currentState(CurrentState::Stop)
, _musics()
{
	buildMusic();
}

void AudioManager::buildMusic()
{
	auto music = new sf::Music;

	music->openFromFile("Media/Sounds/MainMenuTheme.ogg");
	music->setLoop(true);
	_musics.push_back(std::move(music));

	music = new sf::Music;
	music->openFromFile("Media/Sounds/FirstMainMusic.ogg");
	music->setLoop(true);
	_musics.push_back(std::move(music));

	music = new sf::Music;
	music->openFromFile("Media/Sounds/FirstBossMusic.ogg");
	music->setLoop(true);
	_musics.push_back(std::move(music));
}

void AudioManager::stopAllMusics()
{
	for (const auto it : _musics)
	{
		it->stop();
	}
}

void AudioManager::setMusic(const MusicType musicType)
{
	if (_currentMusic == musicType)
	{
		return;
	}

	const auto numMusic = static_cast<std::size_t>(musicType);

	switch (musicType)
	{
		case MusicType::MainMenuTheme:
			_currentMusic = musicType;
			stopAllMusics();
			_musics[numMusic]->play();
			break;
		case MusicType::FirstMainMusic:
			_currentMusic = musicType;
			stopAllMusics();
			_musics[numMusic]->play();
			break;
		case MusicType::FirstBossMusic:
			_currentMusic = musicType;
			stopAllMusics();
			_musics[numMusic]->play();
			break;
		default:
			std::cout << "Error! Invalid type of music.\n";
			break;
	}
}

AudioManager::MusicType AudioManager::getMusicType() const
{
	return _currentMusic;
}

bool AudioManager::isPlaying() const
{
	return _currentState == CurrentState::Play;
}

void AudioManager::setMusicVolume(const float volume)
{
	for (const auto it : _musics)
	{
		it->setVolume(volume);
	}
}

