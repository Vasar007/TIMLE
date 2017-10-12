#ifndef TITLESTATE_HPP
#define TITLESTATE_HPP

#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class TitleState final : public State
{
	private:
		sf::Sprite	mBackgroundSprite;
		sf::Text	mText;
	
		bool		mShowText;
		sf::Time	mTextEffectTime;


	public:
					TitleState(StateStack& stack, Context context);

		void		draw() override;
		bool		update(sf::Time dt) override;
		bool		handleEvent(const sf::Event& event) override;
};

#endif // TITLESTATE_HPP
