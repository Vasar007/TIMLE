#ifndef CHOOSINGSTATE_HPP
#define CHOOSINGSTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>


class ChoosingState : public State
{
	private:
		int						mTextNumber;

		sf::Text				mText;
		GUI::Container 			mGUIContainer;
		sf::Sound				mSound;

		std::vector<sf::String>	mDialogText;
		std::vector<sf::String>	mDialogTalking;

		PlayerInfo*				mPlayerInfo;


	public:
								ChoosingState(StateStack& stack, Context context);

		void					addText(sf::String text);
		void					setText(size_t number);
		virtual void			draw();
		virtual bool			update(sf::Time dt);
		virtual bool			handleEvent(const sf::Event& event);
};

#endif // CHOOSINGSTATE_HPP
