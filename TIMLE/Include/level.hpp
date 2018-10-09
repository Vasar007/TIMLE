#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include <SFML/Graphics.hpp>
#include <tinyxml.h>


/**
 * \brief Special data structure for reading information from the map and keeping it together.
 */
struct Object 
{
    /**
     * \brief Object id.
     */
    int                                mId;

    /**
     * \brief Name of the object.
     */
    std::string                        mName;

    /**
     * \brief Type of the object.
     */
    std::string                        mType;

    /**
     * \brief Rectangle, which contains coordinates of the object, its width and height.
     */
    sf::Rect<float>                    mRect;

    /**
     * \brief An associative array, which contains all properties of the object.
     */
    std::map<std::string, std::string> mProperties;

    /**
     * \brief Sprite of the object.
     */
    sf::Sprite                         mSprite;


    /**
     * \brief          Gets the needed value of the property.
     * \param[in] name Name of the property.
     * \return         Integer value of the property in the array. 
     */
    int                                getPropertyInt(const std::string& name);

    /**
     * \brief          Gets the needed value of the property.
     * \param[in] name Name of the property.
     * \return         Floating point value of the property in the array. 
     */
    float                              getPropertyFloat(const std::string& name);

    /**
     * \brief          Gets the needed value of the property.
     * \param[in] name Name of the property.
     * \return         String value of the property in the array. 
     */
    std::string                        getPropertyString(const std::string& name);
};


/**
 * \brief Additional data structure for keeping information about layers to the map.
 */
struct Layer 
{
    /**
     * \brief The opacity of the layer.
     */
    int                     mOpacity{};

    /**
     * \brief Vector tiles layer.
     */
    std::vector<sf::Sprite> mTiles;
};


/**
 * \brief Main data structure, which contains all information about current map.
 */
class Level 
{
    private:
        /**
         * \brief Width of the map.
         */
        int                _width;

        /**
         * \brief Height of the map.
         */
        int                _height;

        /**
         * \brief Current width of the tile in the map.
         */
        int                _tileWidth;

        /**
         * \brief Current height of the tile in the map.
         */
        int                _tileHeight;

        /**
         * \brief Identifier of the first tile in the map.
         */
        int                _firstTileID;


        /**
         * \brief The size of the bounds, which we draw.
         */
        sf::FloatRect      _drawingBounds;

        /**
         * \brief Current tileset of this map.
         */
        sf::Texture        _tilesetImage;

        /**
         * \brief Vector with all current map layers.
         */
        std::vector<Layer> _layers;

    
    public:
        /**
         * \brief Number of the loaded level.
         */
        std::size_t         mLevelNumber;

        /**
         * \brief Vector with all objects, which exist on the map.
         */
        std::vector<Object> mObjects;
    
    
    public:
        /**
         * \brief Default constructor.
         */
                            Level();

        /**
         * \brief              Loading data from map.
         * \param[in] filename Name of the map.
         * \return             Boolean type: if loading was successful – true, if wasn't – false.
         */
        bool                loadFromFile(const std::string& filename);

        /**
         * \brief          Gets first object with this name from map.
         * \param[in] name Name of the object.
         * \return         Found object or nullptr if object wasn't found.
         */
        Object                getObject(const std::string_view name) const;

        /**
         * \brief          Gets all object with the same name from map.
         * \param[in] name Name of the object.
         * \return         All found object or nullptr if objects weren't found.
         */
        std::vector<Object> getObjects(const std::string_view name) const;

        /**
         * \brief  Gets absolutely all objects on the map.
         * \return Vector with all objects on the map.
         */
        std::vector<Object> getAllObjects() const;

        /**
         * \brief             Draws all map.
         * \param[out] window Active window to drawing.
         */
        void                drawAll(sf::RenderWindow& window) const;

        /**
         * \brief             Draws only player's area of this map.
         * \param[out] window Active window to drawing.
         */
        void                draw(sf::RenderWindow& window);

        /**
         * \brief  Gets tile size of the current map.
         * \return Vector2i, which contains tile size.
         */
        sf::Vector2i        getTileSize() const;
};

#endif // LEVEL_HPP
