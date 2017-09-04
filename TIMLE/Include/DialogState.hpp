#ifndef DIALOGSTATE_HPP
#define DIALOGSTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class DialogState : public State
{
	private:
		int						mTextNumber;

		bool					mChoosing;

		sf::Texture				mTexture;
		sf::Sprite				mSprite;
		sf::Text				mText;
		sf::Text				mTalking;
		GUI::Container 			mGUIContainer;

		std::vector<sf::String>	mDialogText;
		std::vector<sf::String>	mDialogTalking;

		PlayerInfo*				mPlayerInfo;


	public:
								DialogState(StateStack& stack, Context context);

		void					addText(sf::String text, sf::String talking);
		void					setText(size_t number);
		virtual void			draw();
		virtual bool			update(sf::Time dt);
		virtual bool			handleEvent(const sf::Event& event);
};

#endif // DIALOGSTATE_HPP
