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
#include <SFML/Audio/Sound.hpp>


/**
 * \brief Additional namespace for convenience.
 */
namespace GUI
{


/**
 * \brief Standart button class.
 */
class Button : public Component
{
	public:
		typedef std::shared_ptr<Button>	Ptr;
		typedef std::function<void()>	Callback;


	private:
		Callback				mCallback;
		const sf::Texture&		mNormalTexture;
		const sf::Texture&		mSelectedTexture;
		const sf::Texture&		mPressedTexture;
		const sf::SoundBuffer&	mSoundBuffer;

		sf::Sound				mSound;
		sf::Sprite				mSprite;
		sf::Text				mText;
		bool					mIsToggle;
	
	
	private:
		void				draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	public:
							Button(const FontHolder& fonts, const TextureHolder& textures, const SoundBufferHolder& soundBuffer);

		void				setCallback(Callback callback);
		void				setText(const sf::String& text);
		void				setToggle(bool flag);

		bool				isSelectable() const override;
		void				select() override;
		void				deselect() override;

		void				activate() override;
		void				deactivate() override;

		void				handleEvent(const sf::Event& event) override;
};

}

#endif // BUTTON_HPP
