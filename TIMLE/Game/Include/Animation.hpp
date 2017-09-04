#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

class Animation
{
	private:
		std::vector<sf::IntRect>	mFrames;
		const sf::Texture*			mTexture;
	
	
	public:
		Animation();
	
		void						addFrame(sf::IntRect rect);
		void						setSpriteSheet(const sf::Texture& texture);
		const sf::Texture*			getSpriteSheet() const;
		std::size_t					getSize() const;
		const sf::IntRect&			getFrame(std::size_t n) const;
};

#endif // ANIMATION_HPP
