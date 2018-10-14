#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iterator>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>


namespace sf
{
    class Sprite;
    class Text;
}


namespace utils
{

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.

template <typename T>
std::string        toString(const T& value);


// Convert enumerators to strings.

std::string        toString(sf::Keyboard::Key key);


// Call setOrigin() with the center of the object.

void               centerOrigin(sf::Sprite& sprite);
void               centerOrigin(sf::Text& text);


// Degree/radian conversion.

float              toDegree(const float radian);
float              toRadian(const float degree);


// Random number generation.

int                randomInt(const int exclusiveMax);


// Mathematic cector operations.

float              length(const sf::Vector2f vector);
sf::Vector2f       unitVector(const sf::Vector2f vector);

// Debug operations.

sf::RectangleShape buildBorderLines(const sf::Vector2f position, const sf::Vector2f size, 
                                    const sf::Color fillColor, const sf::Color outlineColor, 
                                    const float outlineThickness);

sf::RectangleShape buildBorderLines(const sf::FloatRect rect, const sf::Color fillColor,
                                    const sf::Color outlineColor, const float outlineThickness);

// Copy for smart pointers.

template <class T>
std::unique_ptr<T> copyUnique(const std::unique_ptr<T>& source);

// Copy and move operation with std::vector's.

template <typename T>
typename std::vector<T>::iterator append(std::vector<T>& dest, const std::vector<T>& src);

template <typename T>
typename std::vector<T>::iterator append(std::vector<T>& dest, std::vector<T>&& src);

} // namespace utils

#include "utility.inl"

#endif // UTILITY_HPP
