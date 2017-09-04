#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameOverState : public State
{
	private:
		int				mOpacity;

		sf::Text		mGameOverText;
		sf::Time		mElapsedTime;

		PlayerInfo*		mPlayerInfo;


	public:
						GameOverState(StateStack& stack, Context context);

		virtual void	draw();
		virtual bool	update(sf::Time dt);
		virtual bool	handleEvent(const sf::Event& event);
};

#endif // GAMEOVERSTATE_HPP
