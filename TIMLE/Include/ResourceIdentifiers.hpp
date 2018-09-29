#ifndef RESOURCEIDENTIFIERS_HPP
#define RESOURCEIDENTIFIERS_HPP


// Forward declaration of SFML classes.

namespace sf
{
    class Texture;
    class Font;
    class SoundBuffer;
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


// Forward declaration.

template <typename Resource, typename Identifier>
class ResourceHolder;

// Syntactic sugar.
using TextureHolder     = ResourceHolder<sf::Texture, Textures::ID>;

// Syntactic sugar.
using FontHolder        = ResourceHolder<sf::Font, Fonts::ID>;

// Syntactic sugar.
using SoundBufferHolder = ResourceHolder<sf::SoundBuffer, Sounds::ID>;

#endif // RESOURCEIDENTIFIERS_HPP
