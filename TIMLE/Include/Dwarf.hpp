#ifndef DWARF_HPP
#define DWARF_HPP

#include "Enemy.hpp"


/**
 * \brief Class for three types of dwarfes with different abilities.
 */
class Dwarf final : public Enemy 
{
	private:
		/**
		 * \brief Keep current type of dwarf warrior.
		 */
		int		_dwarfType;


		/**
		 * \brief Special vector for keeping all teleport points of this object.
		 */
		float	_inaccuracy;
		

	public:
		/**
		 * \brief			A constructor with basic parameters.
		 * \param id		The identifier of the current entity.
		 * \param textures	Textures holder for extracting necessary textures.
		 * \param fonts		Fonts holder for extracting necessary textures.
		 * \param lvl		The reference to the level-class object.
		 * \param X			The initial x-coordinate of this object.
		 * \param Y			The initial y-coordinate of this object.
		 * \param width		Width of this object.
		 * \param height	Height if this object.
		 * \param type		An additional parameter for interaction with map.
		 * \param dwarfType	Special parameter for identifier type of dwarfes.
		 */
				Dwarf(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
					  const Level& lvl, const float X, const float Y, const int width, 
					  const int height, const std::string& type = "0", const int dwarfType = 0);


		/**
		 * \brief		Check collision this object with other objects and special areas in the map.
		 * \param Dx	The current delta x-coordinate of this object for checking.
	 	 * \param Dy	The current delta y-coordinate of this object for checking.
		 */
		void	checkCollisionWithMap(const float Dx, const float Dy) override;

		/**
		 * \brief		Overrided funtion for updating this object and its states.
		 * \param dt	Delta time parameter.
		 */
		void	update(const float dt) override;
};

#endif // DWARF_HPP
