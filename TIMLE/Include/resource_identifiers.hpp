#pragma once


// Forward declaration of SFML classes.

namespace sf
{
    class Texture;
    class Font;
    class SoundBuffer;
    class Music;
}

/**
 * \brief Additional namespace for keeping id of the textures.
 */
namespace Textures
{
    /**
     * \brief Enumeration of identificators of all app textures.
     */
    enum class ID
    {
        Archer,
        Swordsman,
        Mage,

        Ghost,
        GhostDeath,
        Golem,
        GolemAttack,
        GolemDeath,
        GolemDark,
        GolemDarkAttack,
        GolemDarkDeath,
        DarkSoldier,
        Goblin,
        MinotaurMage,
        Dwarf,
        Shadow,
        Tentacle,
        DarkArcher,

        Bullet,
        Fire,
        MagicArrow,
        MovingPlatform,
        Quest,

        Bloodsplat,

        RockDirt,
        RockCristal,
        RockDarkStone,
        RockHellStone,
        RockCastle,

        GateDirt,
        GateCristal,
        GateDarkRock,
        GateHellRock,
        GateWood,

        DialogBox,
        HealthBar,
        ManaBar,
        EmptyBar,
        BossBar,

        Knight,

        TitleScreen,
        ButtonNormal,
        ButtonSelected,
        ButtonPressed
    };
}

/**
 * \brief Additional namespace for keeping id of the fonts.
 */
namespace Fonts
{
    /**
     * \brief Enumeration of identificators of all app fonts.
     */
    enum class ID
    {
        Main,
    };
}

/**
* \brief Additional namespace for keeping id of the sound buffers.
*/
namespace Sounds
{
    /**
    * \brief Enumeration of identificators of all app sounds.
    */
    enum class ID
    {
        ButtonCLick,

        StartingDialog,
        Death,

        Bullet
    };
}

/**
* \brief Additional namespace for keeping id of the musics.
*/
namespace Music
{
    /**
     * \brief Contains all music types for playing.
     */
    enum class ID
    {
        MainMenuTheme,

        FirstMainMusic,
        FirstBossMusic,
        FirstMiniBossMusic,

        SecondMusic,
        SecondBossMusic,
        SecondMiniBossMusic,

        ThirdMusic,
        ThirdBossMusic,
        ThirdMiniBossMusic
    };
}


// Forward declaration.

template <typename Resource, typename Identifier>
class ResourceHolder;

using TextureHolder     = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder        = ResourceHolder<sf::Font, Fonts::ID>;
using SoundBufferHolder = ResourceHolder<sf::SoundBuffer, Sounds::ID>;
using MusicHolder       = ResourceHolder<sf::Music, Music::ID>;
