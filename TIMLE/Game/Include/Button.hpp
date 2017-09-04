#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>


namespace GUI
{

class Button : public Component
{
	public:
		typedef std::shared_ptr<Button>	Ptr;
		typedef std::function<void()>	Callback;


	private:
		Callback			mCallback;
		const sf::Texture&	mNormalTexture;
		const sf::Texture&	mSelectedTexture;
		const sf::Texture&	mPressedTexture;
		sf::Sprite			mSprite;
		sf::Text			mText;
		bool				mIsToggle;
	
	
	private:
		virtual void		draw(sf::RenderTarget& target, sf::RenderStates states) const;


	public:
							Button(const FontHolder& fonts, const TextureHolder& textures);

		void				setCallback(Callback callback);
		void				setText(const sf::String& text);
		void				setToggle(bool flag);

		virtual bool		isSelectable() const;
		virtual void		select();
		virtual void		deselect();

		virtual void		activate();
		virtual void		deactivate();

		virtual void		handleEvent(const sf::Event& event);
};

}

#endif // BUTTON_HPP
