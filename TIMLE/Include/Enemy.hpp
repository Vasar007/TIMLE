#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"


/**
 * \brief Abstract interim class for enemies entities.
 */
class Enemy : public Entity 
{
	protected:
		/**
		 * \brief The variable for the counting before we can destroy dead body.
		 */
		int				mCounter;

		/**
		 * \brief The variable for turning the object.
		 */
		bool			mIsTurned;


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
						Enemy(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");

		/**
		 * \brief Default destructor for correct deleting derived objects.
		 */
		virtual			~Enemy() = default;

		/**
		 * \brief		Pure virtual function of checking collision this object with other objects and special areas in the map.
		 * \param Dx	The current delta x-coordinate of this object for checking.
		 * \param Dy	The current delta y-coordinate of this object for checking.
		 */
		virtual void	checkCollisionWithMap(float Dx, float Dy) = 0;

		/**
		 * \brief		Pure virtual and overrided funtion for updating this object and its states.
		 * \param dt	Delta time parameter.
		 */
		void			update(float dt) override = 0;
};

#endif // ENEMY_HPP
