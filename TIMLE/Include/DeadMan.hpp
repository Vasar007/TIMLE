#ifndef DEADMAN_HPP
#define DEADMAN_HPP

#include "Entity.hpp"


class DeadMan : public Entity
{
	public:
		int		mDialogNumber;


	public:
				DeadMan(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string type);
		void	update(float time) override;
};

#endif // DEADMAN_HPP
