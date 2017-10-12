#ifndef BULLET_HPP
#define BULLET_HPP

#include "Projectile.hpp"


class Bullet : public Projectile
{
	public:
		float	mTargetX;
		float	mTargetY;	// Отвечают за направление движения пули к цели
		float	mVx;
		float	mVy;
		float	mDistance;

		bool	mIsMouse;
	
	
	public:
				Bullet(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts,
					   Level &lvl, float X, float Y, int width, int height, float tX, float tY);
								    
				Bullet(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, 
					   Level &lvl, float X, float Y, int width, int height, int dir);

		void	update(float time) override;
};

#endif // BULLET_HPP
