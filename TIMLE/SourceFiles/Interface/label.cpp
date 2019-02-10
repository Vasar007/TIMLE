#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Interface/label.hpp"


namespace GUI
{

Label::Label(const std::string& text, const FontHolder& fonts)
: _text(text, fonts.get(Fonts::ID::Main), 16)
{
}

bool Label::isSelectable() const noexcept
{
    return false;
}

void Label::handleEvent(const sf::Event&) noexcept
{
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(_text, states);
}

void Label::setText(const sf::String& text)
{
    _text.setString(text);
}

}
