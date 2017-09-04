#ifndef GOLEM_HPP
#define GOLEM_HPP

#include "Enemy.hpp"


class Golem : public Enemy 
{
	public:
						Golem(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");
	
		virtual void	checkCollisionWithMap(float Dx, float Dy);
		virtual void	update(float time);
};

#endif // GOLEM_HPP
