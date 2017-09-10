#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>


class PauseState : public State
{
	private:
		sf::Sprite		mBackgroundSprite;
		sf::Text		mPausedText;
		GUI::Container 	mGUIContainer;
		sf::Sound		mSound;

		PlayerInfo*		mPlayerInfo;


	public:
						PauseState(StateStack& stack, Context context);

		virtual void	draw();
		virtual bool	update(sf::Time dt);
		virtual bool	handleEvent(const sf::Event& event);
};

#endif // PAUSESTATE_HPP
