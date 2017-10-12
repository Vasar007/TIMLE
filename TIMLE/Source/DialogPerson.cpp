#include "../Include/DialogPerson.hpp"



DialogPerson::DialogPerson(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, 
						   Level& lvl, float X, float Y, int width, int height, std::string type)
: Entity(Id, X, Y, width, height, 50.f, 100, 0, type)
, mDialogNumber(stoi(type))
{
	if (Id == Type::Oswald || Id == Type::Heinrich)
	{
		mTexture = textures.get(Textures::Knight);
	}
	mSprite.setTexture(mTexture);
	switch(Id)
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
			std::cout << "Invalid dialog person type" << std::endl;
			break;
	}
}

void DialogPerson::update(float dt)
{
}
