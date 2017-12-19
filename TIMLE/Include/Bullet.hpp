#ifndef BULLET_HPP
#define BULLET_HPP

#include "Projectile.hpp"


class Bullet final : public Projectile
{
	public:
		float	mTargetX;
		float	mTargetY;	// Отвечают за направление движения пули к цели
		float	mVx;
		float	mVy;
		float	mDistance;

		bool	mIsMouse;
	
	
	public:
				Bullet(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
					   const Level& lvl, const float X, const float Y, const int width, 
					   const int height, const float tX, const float tY);
								    
				Bullet(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
					   const Level& lvl, const float X, const float Y, const int width,
					   const int height, const int dir);

		void	update(const float dt) override;
};

#endif // BULLET_HPP
