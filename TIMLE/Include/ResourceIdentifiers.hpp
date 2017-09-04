#ifndef RESOURCEIDENTIFIERS_HPP
#define RESOURCEIDENTIFIERS_HPP


// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
}

namespace Textures
{
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

namespace Fonts
{
	enum ID
	{
		Main,
	};
}


// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>	TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>			FontHolder;

#endif // RESOURCEIDENTIFIERS_HPP
