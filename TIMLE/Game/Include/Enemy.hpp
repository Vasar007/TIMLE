#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"


class Enemy : public Entity 
{
	public:
		int				mCounter;

		bool			mIsTurned;


	public:
						Enemy(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");
		virtual			~Enemy();

		virtual void	checkCollisionWithMap(float Dx, float Dy) = 0;
		virtual void	update(float time) = 0;
};

#endif // ENEMY_HPP
