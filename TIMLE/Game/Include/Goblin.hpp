#ifndef GOBLIN_HPP
#define GOBLIN_HPP

#include "Enemy.hpp"


class Goblin : public Enemy 
{
	public:
						Goblin(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");
	
		virtual void	checkCollisionWithMap(float Dx, float Dy);
		virtual void	update(float time);
};

#endif // GOBLIN_HPP
