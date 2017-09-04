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
		int		mDwarfType;


		/**
		 * \brief Special vector for keeping all teleport points of this object.
		 */
		float	mInaccuracy;
		

	public:
		/**
		 * \brief			A constructor with basic parameters.
		 * \param Id		The identifier of the current entity.
		 * \param textures	The reference to an associative array with textures.
		 * \param fonts		The reference to an associative array with fonts.
		 * \param lvl		The reference to the level-class object.
		 * \param X			The initial x-coordinate of this object.
		 * \param Y			The initial y-coordinate of this object.
		 * \param width		Width of this object.
		 * \param height	Height if this object.
		 * \param Type		An additional parameter for interaction with map.
		 * \param dwarfType	Special parameter for identifier type of dwarfes.
		 */
				Dwarf(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0", int dwarfType = 0);


		/**
		 * \brief		Check collision this object with other objects and special areas in the map.
		 * \param Dx	The current delta x-coordinate of this object for checking.
	 	 * \param Dy	The current delta y-coordinate of this object for checking.
		 */
		void	checkCollisionWithMap(float Dx, float Dy) override;

		/**
		 * \brief		Overrided funtion for updating this object and its states.
		 * \param dt	Delta time parameter.
		 */
		void	update(float dt) override;
};

#endif // DWARF_HPP
