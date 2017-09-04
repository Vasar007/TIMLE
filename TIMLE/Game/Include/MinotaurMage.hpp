#ifndef MINOTAUR_MAGE_HPP
#define MINOTAUR_MAGE_HPP

#include "Enemy.hpp"


class MinotaurMage : public Enemy 
{
	public:
						MinotaurMage(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");
	
		virtual void	checkCollisionWithMap(float Dx, float Dy);
		virtual void	update(float time);
};

#endif // MINOTAUR_MAGE_HPP
