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
				Gate(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
					const Level& lvl, const float X, const float Y, const int width,
					const std::string& type);
	
		void	close();
		void	checkCollisionWithMap(const float Dx, const float Dy);
		void	update(const float dt) override;
};

#endif // GATE_HPP
