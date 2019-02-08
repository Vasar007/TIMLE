#pragma once

#include <SFML/Graphics.hpp>


/**
 * \brief Data structure which contains meta information about tile layer of the map.
 */
class tile_layer : public sf::Drawable, public sf::Transformable
{
private:
    /**
     * \brief Points number per one sprite.
     */
    static constexpr int k_points_number = 4;

    /**
     * \brief Size of the tile in layer.
     */
    sf::Vector2u _tile_size;

    /**
     * \brief Width of the whole level.
     */
    unsigned int _level_width;

    /**
     * \brief Height of the whole level.
     */
    unsigned int _level_height;

    /**
     * \brief Vertives storage for layer.
     */
    sf::VertexArray _vertices;

    /**
     * \brief Subset of the vertices which would be drawn.
     */
    mutable std::vector<sf::Vertex> _drawing_vertices;
    
    /**
     * \brief Tileset for layer.
     */
    const sf::Texture& _tileset;


    /**
     * \brief             Draw only player's area of this map.
     * \param[out] target Active window to drawing.
     * \param[in] states  Render states to use for drawing.
     */
    void draw(sf::RenderTarget& target,
              sf::RenderStates states = sf::RenderStates::Default) const override;


    /**
     * \brief            Get index of point with specified coordinates.
     * \param[in] x      Left x-coordinate of the point.
     * \param[in] y      Top y-coordinate of the point.
     * \param[in] offset Offset for shifting to row forward.
     * \return           Index of point in vertices array.
     */
    constexpr std::size_t get_index_in_array(const unsigned int x, const unsigned int y,
                                             const unsigned int offset = 0) const noexcept;

public:
    /**
     * \brief                  Constructor.
     * \param[in] tileset      Tileset texture of the layer.
     * \param[in] tile_size    Tile size of the layer.
     * \param[in] level_width  Width of the level map.
     * \param[in] level_height Height of the level map.
     */
    tile_layer(const sf::Texture& tileset, const sf::Vector2u tile_size,
               const unsigned int level_width, const unsigned int level_height);

    /**
     * \brief              Append tile (add 4 points to the storage) for current layer.
     * \param[in] textRect Texture rectangle of the tile.
     * \param[in] x        Left x-coordinate of tile.
     * \param[in] y        Top y-coordinate of tile.
     * \param[in] color    Tile color with opacity.
     */
    void add_tile(const sf::IntRect& textRect, const int x, const int y, const sf::Color color);

    /**
     * \brief                    Draw only player's area of this map.
     * \param[out] target        Active window to drawing.
     * \param[in] drawing_bounds Drawing bounds which is the player's scope.
     * \param[in] states         Render states to use for drawing.
     */
    void tile_layer::draw_rect(sf::RenderTarget& target, const sf::IntRect drawing_bounds,
                               sf::RenderStates states = sf::RenderStates::Default) const;
};
