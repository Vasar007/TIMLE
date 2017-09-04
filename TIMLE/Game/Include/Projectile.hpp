#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include "DataTables.hpp"


class Projectile : public Entity
{
	public:
								Projectile(Type::ID type, const TextureHolder& textures);

		void					guideTowards(sf::Vector2f position);
		bool					isGuided() const;

		virtual unsigned int	getCategory() const;
		virtual sf::FloatRect	getBoundingRect() const;
		float					getMaxSpeed() const;
		int						getDamage() const;

	
	private:
		virtual void			update(sf::Time);
		virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		Type::ID				mType;
		sf::Sprite				mSprite;
		sf::Vector2f			mTargetDirection;
};

#endif // PROJECTILE_HPP
