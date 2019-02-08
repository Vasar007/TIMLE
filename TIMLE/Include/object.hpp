#pragma once

#include <map>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>


/**
 * \brief Special data structure for reading information from the map and keeping it together.
 */
struct object
{
    /**
     * \brief Object id.
     */
    int                                id;

    /**
     * \brief Name of the object.
     */
    std::string                        name;

    /**
     * \brief Type of the object.
     */
    std::string                        type;

    /**
     * \brief Rectangle, which contains coordinates of the object, its width and height.
     */
    sf::FloatRect                      rect;

    /**
     * \brief An associative array, which contains all properties of the object.
     */
    std::map<std::string, std::string> properties;

    /**
     * \brief Sprite of the object.
     */
    sf::Sprite                         sprite;


    /**
     * \brief          Gets the needed value of the property.
     * \param[in] name Name of the property.
     * \return         Integer value of the property in the array.
     */
    int                                get_property_int(const std::string& name);

    /**
     * \brief          Gets the needed value of the property.
     * \param[in] name Name of the property.
     * \return         Floating point value of the property in the array.
     */
    float                              get_property_float(const std::string& name);

    /**
     * \brief          Gets the needed value of the property.
     * \param[in] name Name of the property.
     * \return         String value of the property in the array.
     */
    std::string                        get_property_string(const std::string& name);
};
