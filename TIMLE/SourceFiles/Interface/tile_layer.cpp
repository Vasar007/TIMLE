#include <iterator>

#include "Interface/tile_layer.hpp"


tile_layer::tile_layer(const sf::Texture& tileset, const sf::Vector2u tile_size,
                       const unsigned int level_width, const unsigned int level_height)
: _tile_size(tile_size)
, _level_width(level_width)
, _level_height(level_height)
, _vertices(sf::Quads, level_width * level_height * k_points_number) // Resize the vertex array to fit the level size.
, _drawing_vertices()
, _tileset(tileset)
{
}

void tile_layer::add_tile(const sf::IntRect& textRect, const int x, const int y,
                          const sf::Color color)
{
    // Populate the vertex array, with one quad per tile.

    // Check that left_index of point is not out of range.
    const auto index = get_index_in_array(x, y);
    if (index + 4 > _vertices.getVertexCount()) return;

    // Get a pointer to the current tile's quad.
    sf::Vertex* const quad = &_vertices[index];

    // Define its 4 corners.
    quad[0].position = sf::Vector2f(static_cast<float>(x * _tile_size.x),       static_cast<float>(y * _tile_size.y));
    quad[1].position = sf::Vector2f(static_cast<float>((x + 1) * _tile_size.x), static_cast<float>(y * _tile_size.y));
    quad[2].position = sf::Vector2f(static_cast<float>((x + 1) * _tile_size.x), static_cast<float>((y + 1) * _tile_size.y));
    quad[3].position = sf::Vector2f(static_cast<float>(x * _tile_size.x),       static_cast<float>((y + 1) * _tile_size.y));

    // Define its 4 texture coordinates.
    quad[0].texCoords = sf::Vector2f(static_cast<float>(textRect.left),                  static_cast<float>(textRect.top));
    quad[1].texCoords = sf::Vector2f(static_cast<float>(textRect.left + textRect.width), static_cast<float>(textRect.top));
    quad[2].texCoords = sf::Vector2f(static_cast<float>(textRect.left + textRect.width), static_cast<float>(textRect.top + textRect.height));
    quad[3].texCoords = sf::Vector2f(static_cast<float>(textRect.left),                  static_cast<float>(textRect.top + textRect.height));

    // Define its color.
    quad[0].color = color;
    quad[1].color = color;
    quad[2].color = color;
    quad[3].color = color;
}

void tile_layer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Apply the transform.
    states.transform *= getTransform();

    // Apply the tileset texture.
    states.texture = &_tileset;

    // Draw the vertex array.
    target.draw(_vertices, states);
}

constexpr std::size_t tile_layer::get_index_in_array(const int x, const int y,
                                                     const int offset) const noexcept
{
    return (x + (y + offset) * _level_width) * k_points_number;
}

void tile_layer::draw_rect(sf::RenderTarget& target, const sf::IntRect drawing_bounds,
                           sf::RenderStates states) const
{
    // Call destructors but not free memory.
    _drawing_vertices.clear();

    const auto count = (drawing_bounds.height / _tile_size.y + 1);
    if (const auto new_capacity = (drawing_bounds.width / _tile_size.x + 1) * count * k_points_number * k_points_number;
        _drawing_vertices.capacity() < new_capacity)
    {
        _drawing_vertices.reserve(new_capacity);
    }

    for (int i = 0; i < count; ++i)
    {
        const auto left_index = get_index_in_array(
            drawing_bounds.left / _tile_size.x,
            drawing_bounds.top / _tile_size.y,
            i
        );
        const auto right_index = get_index_in_array(
            (drawing_bounds.left + drawing_bounds.width) / _tile_size.x,
            drawing_bounds.top / _tile_size.y,
            i
        ) + k_points_number; // Shift position to the right because of the rounding error.

        if (left_index >= _vertices.getVertexCount() || right_index >= _vertices.getVertexCount()) break;
        _drawing_vertices.insert(std::end(_drawing_vertices), &_vertices[left_index], &_vertices[right_index]);
    }

    if (_drawing_vertices.empty())
        return;

    // Apply the transform.
    states.transform *= getTransform();

    // Apply the tileset texture.
    states.texture = &_tileset;

    // Draw the vertex array.
    target.draw(&_drawing_vertices.front(), _drawing_vertices.size(), sf::Quads, states);
}
