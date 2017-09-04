#ifndef DWARF_HPP
#define DWARF_HPP

#include "Enemy.hpp"


class Dwarf : public Enemy 
{
	public:
		int				mDwarfType;
		float			mInaccuracy;
		

	public:
						Dwarf(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0", int dwarfType = 0);
	
		virtual void	checkCollisionWithMap(float Dx, float Dy);
		virtual void	update(float time);
};

#endif // DWARF_HPP
