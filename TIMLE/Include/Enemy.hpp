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
		 * \param id		The identifier of the current entity.
		 * \param textures	Textures holder for extracting necessary textures.
		 * \param fonts		Fonts holder for extracting necessary textures.
		 * \param lvl		The reference to the level-class object.
		 * \param X			The initial x-coordinate of this object.
		 * \param Y			The initial y-coordinate of this object.
		 * \param width		Width of this object.
		 * \param height	Height if this object.
		 * \param type		An additional parameter for interaction with map.
		 */
						Enemy(Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
							  const Level& lvl, const float X, const float Y, const int width, 
							  const int height, const std::string& type = "0");

		/**
		 * \brief Default destructor for correct deleting derived objects.
		 */
		virtual			~Enemy() = default;

		Enemy(const Enemy& other) = default;

		Enemy(Enemy&& other) = default;

		Enemy& operator=(const Enemy& other) = default;

		Enemy& operator=(Enemy&& other) = default;

		/**
		 * \brief		Check collision this object with other objects and special areas in the map.
		 * \param Dx	The current delta x-coordinate of this object for checking.
		 * \param Dy	The current delta y-coordinate of this object for checking.
		 */
		virtual void	checkCollisionWithMap(const float Dx, const float Dy) = 0;

		/**
		 * \brief		Pure virtual and overrided funtion for updating this object and its states.
		 * \param dt	Delta time parameter.
		 */
		void			update(const float dt) override = 0;
};

#endif // ENEMY_HPP
