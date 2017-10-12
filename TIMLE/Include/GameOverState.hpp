#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameOverState final : public State
{
	private:
		int				mOpacity;

		sf::Text		mGameOverText;
		sf::Time		mElapsedTime;

		PlayerInfo*		mPlayerInfo;


	public:
						GameOverState(StateStack& stack, Context context);

		void			draw() override;
		bool			update(sf::Time dt) override;
		bool			handleEvent(const sf::Event& event) override;
};

#endif // GAMEOVERSTATE_HPP
