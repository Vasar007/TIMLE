#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>


class PauseState final : public State
{
	private:
		sf::Sprite		mBackgroundSprite;
		sf::Text		mPausedText;
		GUI::Container 	mGUIContainer;
		sf::Sound		mSound;

		PlayerInfo*		mPlayerInfo;


	public:
						PauseState(StateStack& stack, Context context);

		void			draw() override;
		bool			update(sf::Time dt) override;
		bool			handleEvent(const sf::Event& event) override;
};

#endif // PAUSESTATE_HPP
