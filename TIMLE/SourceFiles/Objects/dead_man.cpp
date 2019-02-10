#include <iostream>

#include "Objects/dead_man.hpp"


DeadMan::DeadMan(const Type::ID id, const TextureHolder& textures, const FontHolder&, const level&,
                 const float X, const float Y, const int width, const int height, 
                 const std::string& type)
: Entity(id, X, Y, width, height, 50.f, 100, 0, type)
{
    switch(id)
    {
        case Type::ID::DeadJuggernaut:
            mTexture = textures.get(Textures::ID::Swordsman);
            mSprite.setTexture(mTexture);
            mSprite.setTextureRect(sf::IntRect(1865, 915, 115, 45));
            mSprite.setPosition(x + (mWidth / 2.f) - 13.f, y + (mHeight / 2.f) + 2.f);
            mSprite.setScale(0.33f, 0.33f);
            break;
        case Type::ID::DeadDwarf:
            mTexture = textures.get(Textures::ID::Dwarf);
            mSprite.setTexture(mTexture);
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
