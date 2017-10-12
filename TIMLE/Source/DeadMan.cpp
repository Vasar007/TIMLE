#include "../Include/DeadMan.hpp"


DeadMan::DeadMan(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, 
				 float X, float Y, int width, int height, std::string type)
: Entity(Id, X, Y, width, height, 50.f, 100, 0, type)
, mDialogNumber(stoi(type))
{
	if (Id == Type::DeadJuggernaut)
	{
		mTexture = textures.get(Textures::Swordsman);
	}
	else if (Id == Type::DeadDwarf)
	{
		mTexture = textures.get(Textures::Dwarf);
	}
	mSprite.setTexture(mTexture);
	switch(Id)
	{
		case Type::DeadJuggernaut:
			mSprite.setTextureRect(sf::IntRect(1865, 915, 115, 45));
			mSprite.setPosition(x + (mWidth / 2.f) - 13.f, y + (mHeight / 2.f) + 2.f);
			mSprite.setScale(0.33f, 0.33f);
			break;
		case Type::DeadDwarf:
			mSprite.setScale(0.5f, 0.5f);
			mSprite.setPosition(x + (mWidth / 2.f) - 20.f, y + (mHeight / 2.f) - 27.f);
			mSprite.setTextureRect(sf::IntRect(100 * 15, 0, 100, 80));
			break;
		default:
			std::cout << "Invalid dead person type" << std::endl;
			break;
	}
}

void DeadMan::update(float dt)
{
}
