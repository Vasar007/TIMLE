#ifndef DIALOGSTATE_HPP
#define DIALOGSTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>


class DialogState final : public State
{
	private:
		int						mTextNumber;

		bool					mChoosing;

		sf::Texture				mTexture;
		sf::Sprite				mSprite;
		sf::Text				mText;
		sf::Text				mTalking;
		GUI::Container 			mGUIContainer;
		sf::Sound				mSound;

		std::vector<sf::String>	mDialogText;
		std::vector<sf::String>	mDialogTalking;

		PlayerInfo*				mPlayerInfo;


	public:
								DialogState(StateStack& stack, Context context);

		void					addText(sf::String text, sf::String talking);
		void					setText(size_t number);
		void					draw() override;
		bool					update(sf::Time dt) override;
		bool					handleEvent(const sf::Event& event) override;
};

#endif // DIALOGSTATE_HPP
