#ifndef DARKSOLDIER_HPP
#define DARKSOLDIER_HPP

#include "Enemy.hpp"


class DarkSoldier : public Enemy 
{
	public:
						DarkSoldier(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");
	
		virtual void	checkCollisionWithMap(float Dx, float Dy);
		virtual void	update(float time);
};

#endif // DARKSOLDIER_HPP
