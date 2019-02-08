#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <SFML/Audio/Music.hpp>

#include "resource_holder.hpp"
#include "resource_identifiers.hpp"


/**
 * \brief Class for manipulation with musics in this app.
 */
class AudioManager
{
    private:
        /**
         * \brief Array of music's states.
         */
        enum class CurrentState
        {
            Play,
            Pause,
            Stop
        };      


    private:
        /**
         * \brief Current playing music types array.
         */
        std::vector<Music::ID> _currentMusicID;

        /**
         * \brief Current loaded music types array.
         */
        std::vector<Music::ID> _availableMusicID;

        /**
         * \brief Current music's state.
         */
        CurrentState           _currentState;

        /**
        * \brief Main array with all musics.
        */
        MusicHolder            _musics;


    private:
        /**
         * \brief Initialization available musics.
         */
        void buildMusic();

        /**
         * \brief               Load music file from disk.
         * \param[in] musicType New music type to load.
         * \param[in] path      File name to load.
         */
        void loadMusic(const Music::ID musicType, const std::string& path);


    public:
        /**
         * \brief Default constructor.
         */
                  AudioManager();

        /**
         * \brief               Changes playing music for another.
         * \param[in] musicType New music type to play.
         */
        void      setMusic(const Music::ID musicType);

        /**
         * \brief Stops all tracks even they aren't playing now.
         */
        void      stopAllMusics();

        /**
         * \brief  Gets current music type.
         * \return Current playing music type.
         */
        std::vector<Music::ID> getPlayingMusicType() const;

        /**
         * \brief  Gets information about current state.
         * \return True, if music is playing, else – false.
         */
        bool      isPlaying() const;

        /**
         * \brief            Set music volume.
         * \param[in] volume New value to set.
         */
        void      setMusicVolume(const float volume);


};
