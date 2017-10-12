#ifndef LABEL_HPP
#define LABEL_HPP

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/Text.hpp>


/**
 * \brief Additional namespace for working with GUI elements.
 */
namespace GUI
{

/**
 * \brief Standart label class.
 */
class Label final : public Component
{
	public:
		/**
		 * \brief Syntactic sugar. ( std::shared_ptr<Label> ).
		 */
		typedef std::shared_ptr<Label> Ptr;
			

	private:
		/**
		 * \brief Text of the label.
		 */
		sf::Text	mText;
	
	
	private:
		/**
		 * \brief			Draw the object to a render target.
		 * \param target	Render target to draw to.
		 * \param states	Current render states.
		 */
		void		draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	public:
		/**
		 * \brief		Default constuctor.
		 * \param text	Text of the label.
		 * \param fonts	Font holder for extracting necessary font.
		 */
					Label(const std::string& text, const FontHolder& fonts);

		/**
		 * \brief	Check if label can be selectable.
		 * \return	True if label is selectable or false otherwise.
		 */
		bool		isSelectable() const override;

		/**
		 * \brief		Set text to this label.
		 * \param text	Text of the label.
		 */
		void		setText(const sf::String& text);

		/**
		 * \brief		Handle input events.
		 * \param event The event that was sent.
		 */
		void		handleEvent(const sf::Event& event) override;
};

}

#endif // LABEL_HPP
