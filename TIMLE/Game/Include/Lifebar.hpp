#ifndef LIFEBAR_HPP
#define LIFEBAR_HPP

#include "ResourceHolder.hpp"
#include "DataTables.hpp"

#include <SFML/Graphics.hpp>


class LifeBar
{
	public:
		sf::Texture			mTexture;
		sf::Texture			mTextureEmpty;
		sf::Sprite			mSprite;
		sf::Sprite			mSpriteEmpty;
		const int			mMax;
		Type::ID			mType;
		sf::Text			mBossName;
	
	
	public:
							LifeBar(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, int hitpoints);
	
		void				update(int points);
		void				draw(sf::RenderWindow& window);
};

#endif // LIFEBAR_HPP
