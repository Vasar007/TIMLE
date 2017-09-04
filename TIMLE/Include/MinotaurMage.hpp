#ifndef MINOTAUR_MAGE_HPP
#define MINOTAUR_MAGE_HPP

#include "Enemy.hpp"


/**
 * \brief Classs for the strongest horned mage, who destroys the enemies deadly magic.
 */
class MinotaurMage final : public Enemy 
{
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
		 */
				MinotaurMage(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");


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

#endif // MINOTAUR_MAGE_HPP
