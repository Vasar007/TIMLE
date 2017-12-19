#ifndef DIALOGPERSON_HPP
#define DIALOGPERSON_HPP

#include "Entity.hpp"


class DialogPerson final : public Entity
{
	public:
		int		mDialogNumber;


	public:
				DialogPerson(const Type::ID id, const TextureHolder& textures, 
							 const FontHolder& fonts, const Level& lvl, const float X, 
							 const float Y, const int width, const int height, 
							 const std::string& type);

		void	update(const float dt) override;
};

#endif // DIALOGPERSON_HPP
