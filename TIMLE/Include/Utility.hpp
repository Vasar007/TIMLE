#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <sstream>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


namespace sf
{
    class Sprite;
    class Text;
}


// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.

template <typename T>
std::string        toString(const T& value);


// Convert enumerators to strings.

std::string        toString(sf::Keyboard::Key key);


// Call setOrigin() with the center of the object.

void            centerOrigin(sf::Sprite& sprite);
void            centerOrigin(sf::Text& text);


// Degree/radian conversion.

float            toDegree(const float radian);
float            toRadian(const float degree);


// Random number generation.

int                randomInt(const int exclusiveMax);


// Vector operations.

float            length(const sf::Vector2f vector);
sf::Vector2f    unitVector(const sf::Vector2f vector);

// Debug operations.

sf::RectangleShape buildBorderLines(const sf::Vector2f position, const sf::Vector2f size, 
                                    const sf::Color fillColor, const sf::Color outlineColor, 
                                    const float outlineThickness);

sf::RectangleShape buildBorderLines(const sf::FloatRect rect, const sf::Color fillColor,
                                    const sf::Color outlineColor, const float outlineThickness);

#include "Utility.inl"

#endif // UTILITY_HPP
