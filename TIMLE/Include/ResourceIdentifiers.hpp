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
	enum ID
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

		Bullet,
		Fire,
		MovingPlatform,
		Quest,

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
	enum ID
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
	enum ID
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

// Syntactic sugar. ( ResourceHolder<sf::Texture, Textures::ID> ).
typedef ResourceHolder<sf::Texture, Textures::ID>	TextureHolder;

// Syntactic sugar. ( ResourceHolder<sf::Font, Fonts::ID>	 ).
typedef ResourceHolder<sf::Font, Fonts::ID>			FontHolder;

// Syntactic sugar. ( ResourceHolder<sf::SoundBuffer, Sounds::ID> ).
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID>	SoundBufferHolder;

#endif // RESOURCEIDENTIFIERS_HPP
