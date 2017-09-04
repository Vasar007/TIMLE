#ifndef LABEL_HPP
#define LABEL_HPP

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/Text.hpp>


/**
 * \brief Additional namespace for convenience.
 */
namespace GUI
{

/**
 * \brief Standart label class.
 */
class Label : public Component
{
	public:
		typedef std::shared_ptr<Label> Ptr;
			

	private:
		sf::Text	mText;
	
	
	private:
		void		draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	public:
					Label(const std::string& text, const FontHolder& fonts);

		bool		isSelectable() const override;
		void		setText(const sf::String& text);

		void		handleEvent(const sf::Event& event) override;
};

}

#endif // LABEL_HPP
