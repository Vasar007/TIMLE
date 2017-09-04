#ifndef GOLEMDARK_HPP
#define GOLEMDARK_HPP

#include "Enemy.hpp"


class GolemDark : public Entity
{
	public:
		int				mCounter;

		bool			mIsTurned;

	public:
						GolemDark(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");
	
		virtual void	checkCollisionWithMap(float Dx, float Dy);
		virtual void	update(float time);
};

#endif // GOLEMDARK_HPP
