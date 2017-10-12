#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Entity.hpp"


class Projectile : public Entity
{
	private:
		enum Category
		{
			EnemyProjectile,
			AlliedProjectile
		};

		sf::Vector2f			mTargetDirection;
		bool					mGuided;


	public:
								Projectile(Type::ID Id, const TextureHolder& textures, 
										   const FontHolder& fonts, Level &lvl, float X, float Y, 
										   int width, int height);

		void					guideTowards(sf::Vector2f position);
		bool					isGuided() const;

		virtual unsigned int	getCategory() const;
		virtual sf::FloatRect	getBoundingRect() const;
		float					getMaxSpeed() const;
		int						getDamage() const;

		/**
		 * \brief		Pure virtual and overrided funtion for updating this object and its states.
		 * \param dt	Delta time parameter.
		 */
		void					update(float dt) override = 0;
};

#endif // PROJECTILE_HPP
