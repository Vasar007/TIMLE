#ifndef CHANINGSTATE_HPP
#define CHANINGSTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>


class ChanginState final : public State
{
	private:
		int						mTextNumber;

		sf::Text				mText;
		GUI::Container 			mGUIContainer;
		sf::Sound				mSound;

		std::vector<sf::String>	mDialogText;

		PlayerInfo*				mPlayerInfo;
		CurrentSettings*		mCurrentSettings;


	public:
								ChanginState(StateStack& stack, Context context);

		void					addText(sf::String text);
		void					setText(size_t number);
		void					draw() override;
		bool					update(sf::Time dt) override;
		bool					handleEvent(const sf::Event& event) override;
};

#endif // CHANINGSTATE_HPP
