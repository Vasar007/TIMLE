#ifndef GATE_HPP
#define GATE_HPP

#include "Entity.hpp"


class Gate : public Entity
{
	public:
		int		mSize;
		
		float	mWaySizeDown;
		float	mWaySizeUp;

	
	public:
				Gate(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, std::string type);
	
		void	close();
		void	checkCollisionWithMap(float Dx, float Dy);
		void	update(float time);
};

#endif // GATE_HPP
