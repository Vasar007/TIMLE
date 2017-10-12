#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "State.hpp"
#include "World.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameState final : public State
{
	private:
		World			mWorld;
		PlayerInfo&		mPlayerInfo;


	public:
						GameState(StateStack& stack, Context context);

		void			draw() override;
		bool			update(sf::Time dt) override;
		bool			handleEvent(const sf::Event& event) override;
};

#endif // GAMESTATE_HPP
