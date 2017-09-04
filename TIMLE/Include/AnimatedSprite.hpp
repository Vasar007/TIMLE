#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include "Animation.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>


/**
 * \brief Not used class, work in progress.
 */
class AnimatedSprite : public sf::Drawable, public sf::Transformable
{
	private:
		const Animation*	mAnimation;
		sf::Time			mFrameTime;
		sf::Time			mCurrentTime;
		std::size_t			mCurrentFrame;
		bool				mIsPaused;
		bool				mIsLooped;
		const sf::Texture*	mTexture;
		sf::Vertex			mVertices[4];

		void		draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	
	public:
	    explicit			AnimatedSprite(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true);
	
	    void				update(sf::Time deltaTime);
	    void				setAnimation(const Animation& animation);
	    void				setFrameTime(sf::Time time);
	    void				play();
	    void				play(const Animation& animation);
	    void				pause();
	    void				stop();
	    void				setLooped(bool looped);
	    void				setColor(const sf::Color& color);
	    const Animation*	getAnimation() const;
	    sf::FloatRect		getLocalBounds() const;
		sf::FloatRect		getGlobalBounds() const;
	    bool				isLooped() const;
	    bool				isPlaying() const;
	    sf::Time			getFrameTime() const;
	    void				setFrame(std::size_t newFrame, bool resetTime = true);
};

#endif // ANIMATEDSPRITE_HPP
