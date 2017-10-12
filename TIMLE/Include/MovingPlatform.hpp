#ifndef MOVING_PLATFORM_HPP
#define MOVING_PLATFORM_HPP

#include "Entity.hpp"


class MovingPlatform : public Entity 
{
	public:
				MovingPlatform(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts,
							   Level &lvl, float X, float Y, int width, int height);
	
		void	update(float dt) override;
};

#endif // MOVING_PLATFORM_HPP
