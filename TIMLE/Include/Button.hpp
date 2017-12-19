#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <memory>
#include <functional>
#include <SFML/Audio/Sound.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"


/**
 * \brief Additional namespace for working with GUI elements.
 */
namespace GUI
{

/**
 * \brief Standart button class.
 */
class Button final : public Component
{
	public:
		/**
		 * \brief Syntactic sugar. ( std::shared_ptr<Button> ).
		 */
		typedef std::shared_ptr<Button>	shPtr;

		/**
		 * \brief Syntactic sugar. ( std::function<void()> ).
		 */
		typedef std::function<void()>	Callback;


	private:
		/**
		 * \brief Action that will be executed when button is pressed.
		 */
		Callback				_callback;

		/**
		 * \brief Textrure for button when it is not pressed or selected.
		 */
		const sf::Texture&		_normalTexture;

		/**
		 * \brief Textrure for button when it is selected but not pressed.
		 */
		const sf::Texture&		_selectedTexture;

		/**
		 * \brief Textrure for button when it is pressed.
		 */
		const sf::Texture&		_pressedTexture;

		/**
		 * \brief Sound buffer for keeping button sounds.
		 */
		const sf::SoundBuffer&	_soundBuffer;


		/**
		 * \brief Sound of the button (playing when button is pressed or selected).
		 */
		sf::Sound				_sound;

		/**
		 * \brief Sprite of the button for drawing.
		 */
		sf::Sprite				_sprite;

		/**
		 * \brief Text of the button.
		 */
		sf::Text				_text;

		/**
		 * \brief Boolean flag of toggling.
		 */
		bool					_isToggle;
	
	
	private:
		/**
		 * \brief 			Draw the object to a render target.
		 * \param target	Render target to draw to.
		 * \param states	Current render states.
		 */
		void				draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	public:
		/**
		 * \brief				Default constructor.
		 * \param fonts			Fonts holder for extracting necessary font.
		 * \param textures		Textures holder for extracting necessary textures.
		 * \param soundBuffer	Sounds holder for extracting necessary sounds.
		 */
							Button(const FontHolder& fonts, const TextureHolder& textures,
								   const SoundBufferHolder& soundBuffer);

		/**
		 * \brief			Set the callback (action that will be executed when button is pressed).
		 * \param callback	Action for button.
		 */
		void				setCallback(Callback callback);

		/**
		 * \brief 		Set text to this button.
		 * \param text	Text of the button.
		 */
		void				setText(const sf::String& text);

		/**
		 * \brief		Change toggle flag of this button.
		 * \param flag	New value for toggle flag.
		 */
		void				setToggle(const bool flag);

		/**
		 * \brief 	Check if button can be selectable.
		 * \return	True if button is selectable or false otherwise.
		 */
		bool				isSelectable() const override;

		/**
		 * \brief Call parent method, play sound and update button sprite.
		 */
		void				select() override;

		/**
		 * \brief Call parent method and update button sprite.
		 */
		void				deselect() override;

		/**
		 * \brief Call parent method, play sound, do callback and update button sprite.
		 */
		void				activate() override;

		/**
		 * \brief Call parent method and update button sprite.
		 */
		void				deactivate() override;

		/**
		 * \brief 		Handle input events.
		 * \param event The event that was sent.
		 */
		void				handleEvent(const sf::Event& event) override;
};

}

#endif // BUTTON_HPP
