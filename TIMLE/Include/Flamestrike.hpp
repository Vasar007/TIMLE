#ifndef FIREBALL_HPP
#define FIREBALL_HPP

#include "Projectile.hpp"


class Flamestrike : public Projectile
{
	public:
				Flamestrike(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts,
							   Level &lvl, float X, float Y, int width, int height);
	
		void	update(float dt) override;
};

#endif // FIREBALL_HPP
