#ifndef TITRESTATE_HPP
#define TITRESTATE_HPP

#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class TitreState : public State
{
	private:
		sf::Sprite		mBackgroundSprite;
		sf::Text		mText;
	
		sf::Time		mTextEffectTime;
		sf::Time		mTextAppearenceTime;

	private:
		void			setText();

	public:
						TitreState(StateStack& stack, Context context);

		virtual void	draw();
		virtual bool	update(sf::Time dt);
		virtual bool	handleEvent(const sf::Event& event);
};

#endif // TITRESTATE_HPP
