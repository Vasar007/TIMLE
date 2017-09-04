#ifndef DIALOGPERSON_HPP
#define DIALOGPERSON_HPP

#include "Entity.hpp"


class DialogPerson : public Entity
{
	public:
		int				mDialogNumber;


	public:
						DialogPerson(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string type);
		virtual void	update(float time);
};

#endif // DIALOGPERSON_HPP
