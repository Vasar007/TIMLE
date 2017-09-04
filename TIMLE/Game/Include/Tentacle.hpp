#ifndef TENTACLE_HPP
#define TENTACLE_HPP

#include "Enemy.hpp"


class Tentacle : public Enemy 
{
	public:
		float			mAppearing;
		float			mDisappearing;


	public:
						Tentacle(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");
	
		void			appear(float time);
		void			disappear(float time);
		virtual void	checkCollisionWithMap(float Dx, float Dy);
		virtual void	update(float time);
};

#endif // TENTACLE_HPP
