#include "../Include/DeadMan.hpp"


DeadMan::DeadMan(const Type::ID id, const TextureHolder& textures, const FontHolder&, const Level&,
				 const float X, const float Y, const int width, const int height, 
				 const std::string& type)
: Entity(id, X, Y, width, height, 50.f, 100, 0, type)
, mDialogNumber(std::stoi(type))
{
	if (id == Type::DeadJuggernaut)
	{
		mTexture = textures.get(Textures::ID::Swordsman);
	}
	else if (id == Type::DeadDwarf)
	{
		mTexture = textures.get(Textures::ID::Dwarf);
	}
	mSprite.setTexture(mTexture);

	switch(id)
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
			std::cout << "Invalid dead person type\n";
			break;
	}
}

void DeadMan::update(const float)
{
}
