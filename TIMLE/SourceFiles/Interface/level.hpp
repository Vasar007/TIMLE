#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <SFML/Graphics.hpp>
#include <TinyXML/tinyxml.h>

#include "Interface/object.hpp"
#include "Interface/tile_layer.hpp"


/**
 * \brief Main data structure, which contains all information about current map.
 */
class level 
{
private:
    /**
     * \brief Additional data structure for keeping information about layers to the map.
     */
    struct layer
    {
                   layer(const sf::Texture& tileset, const sf::Vector2u tile_size,
                         const unsigned int width, const unsigned int height)
                   : opacity(0)
                   , tiles(tileset, tile_size, width, height)
                   {
                   }

        /**
         * \brief The opacity of the layer.
         */
        int        opacity;

        /**
         * \brief Array which contains tiles from the same layer in tile map.
         */
        tile_layer tiles;
    };


    /**
     * \brief Width of the map.
     */
    unsigned int        _width;

    /**
     * \brief Height of the map.
     */
    unsigned int        _height;

    /**
     * \brief Current width of the tile in the map.
     */
    unsigned int        _tile_width;

    /**
     * \brief Current height of the tile in the map.
     */
    unsigned int        _tile_height;

    /**
     * \brief Identifier of the first tile in the map.
     */
    int                 _first_tile_id;

    /**
     * \brief The size of the bounds, which we draw.
     */
    sf::FloatRect       _drawing_bounds;

    /**
     * \brief Current tileset of this map.
     */
    sf::Texture         _tileset;

    /**
     * \brief Vector with all current map layers.
     */
    std::vector<layer>  _layers;


public:
    /**
     * \brief Number of the loaded level.
     */
    std::size_t         level_number;

    /**
     * \brief Vector with all objects, which exist on the map.
     */
    std::vector<object> objects;


    /**
     * \brief Default constructor.
     */
                        level();

    /**
     * \brief                     Loading data from map.
     * \param[in] filename        Name of the map.
     * \param[in] tile_sheet_path Additional path of the tile sheet.
     * \return                    If loading was successful returns true, false otherwise.
     */
    bool                load_from_file(const std::string& filename,
                                       const std::string& tile_sheet_path = "");

    /**
     * \brief          Get first object with this name from map.
     * \param[in] name Name of the object.
     * \return         Found object or nullptr if object wasn't found.
     */
    object              get_object(const std::string_view name) const;

    /**
     * \brief          Get all object with the same name from map.
     * \param[in] name Name of the object.
     * \return         All found object or nullptr if objects weren't found.
     */
    std::vector<object> get_objects(const std::string_view name) const;

    /**
     * \brief  Get absolutely all objects on the map.
     * \return Vector with all objects on the map.
     */
    std::vector<object> get_all_objects() const noexcept;

    /**
     * \brief             Draw all map.
     * \param[out] window Active window to drawing.
     */
    void                draw_all(sf::RenderWindow& window) const;

    /**
     * \brief             Draw only player's area of this map.
     * \param[out] window Active window to drawing.
     */
    void                draw(sf::RenderWindow& window);

    /**
     * \brief  Get tile size of the current map.
     * \return Vector2u, which contains tile size.
     */
    sf::Vector2u        get_tile_size() const noexcept;
};
