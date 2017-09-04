#ifndef LABEL_HPP
#define LABEL_HPP

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/Text.hpp>


namespace GUI
{

class Label : public Component
{
	public:
		typedef std::shared_ptr<Label> Ptr;
			

	private:
		sf::Text		mText;
	
	
	private:
		void			draw(sf::RenderTarget& target, sf::RenderStates states) const;


	public:
						Label(const std::string& text, const FontHolder& fonts);

		virtual bool	isSelectable() const;
		void			setText(const sf::String& text);

		virtual void	handleEvent(const sf::Event& event);
};

}

#endif // LABEL_HPP
