#ifndef TITRESTATE_HPP
#define TITRESTATE_HPP

#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class TitreState final : public State
{
	private:
		sf::Sprite	mBackgroundSprite;
		sf::Text	mText;
	
		sf::Time	mTextEffectTime;
		sf::Time	mTextAppearenceTime;

	private:
		void		setText();

	public:
					TitreState(StateStack& stack, Context context);

		void		draw() override;
		bool		update(sf::Time dt) override;
		bool		handleEvent(const sf::Event& event) override;
};

#endif // TITRESTATE_HPP
