#ifndef TENTACLE_HPP
#define TENTACLE_HPP

#include "Enemy.hpp"


/**
 * \brief Pretty casual class for shadow's tentacles.
 */
class Tentacle final : public Enemy 
{
	private:
		/**
		 * \brief An additional counter for helping with setting tentacles' sprites while they are appearing.
		 */
		float	mAppearing;

		/**
		 * \brief An additional counter for helping with setting tentacles' sprites while they are disappearing.
		 */
		float	mDisappearing;


	private:
		/**
		 * \brief		Helper function for appearing this object.
		 * \param dt	Delta time parameter.
		 */
		void	appear(float dt);

		/**
		 * \brief		Helper function for disappearing this object.
		 * \param dt	Delta time parameter.
		 */
		void	disappear(float dt);


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
				Tentacle(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");


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

#endif // TENTACLE_HPP
