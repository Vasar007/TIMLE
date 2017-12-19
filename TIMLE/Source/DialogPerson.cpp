#include "../Include/DialogPerson.hpp"



DialogPerson::DialogPerson(const Type::ID id, const TextureHolder& textures, const FontHolder&,
						   const Level&, const float X, const float Y, const int width, 
						   const int height, const std::string& type)
: Entity(id, X, Y, width, height, 50.f, 100, 0, type)
, mDialogNumber(std::stoi(type))
{
	if (id == Type::Oswald || id == Type::Heinrich)
	{
		mTexture = textures.get(Textures::ID::Knight);
	}
	mSprite.setTexture(mTexture);

	switch(id)
	{
		case Type::Oswald:
			mSprite.setTextureRect(sf::IntRect(68, 78, 26, 32));
			mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f) - 5.f);
			mSprite.setScale(0.75f, 0.75f);
			break;
		case Type::Heinrich:
			mSprite.setTextureRect(sf::IntRect(0, 120, 57, 63));
			mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f) - 20.f);
			mSprite.setScale(0.75f, 0.75f);
			break;
		default:
			std::cout << "Invalid dialog person type\n";
			break;
	}
}

void DialogPerson::update(const float)
{
}
