#ifndef GHOST_HPP
#define GHOST_HPP

#include "Enemy.hpp"


class Ghost : public Enemy 
{
	public:
						Ghost(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");
	
		virtual void	checkCollisionWithMap(float Dx, float Dy);
		virtual void	update(float time);
};

#endif // GHOST_HPP
