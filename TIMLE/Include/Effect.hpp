#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <SFML/Graphics.hpp>

#include "data_tables.hpp"
#include "resource_holder.hpp"


/**
 * \brief Abstract base class for all effects in the game.
 */
class Effect
{
    public:
        /**
         * \brief Contain x coordinate of the object.
         */
        float       x;

        /**
         * \brief Contain y coordinate of the object.
         */
        float       y;

        /**
         * \brief Keep object width.
         */
        int         width;

        /**
         * \brief Keep object height.
         */
        int         height;

        /**
         * \brief Timer for usual period.
         */
        float       current_frame;

        /**
         * \brief Timer for attack period.
         */
        float       current_attack_frame;

        /**
         * \brief Timer for death period.
         */
        float       current_death_frame;

        /**
         * \brief Timer for moving action.
         */
        float       move_timer;

        /**
         * \brief Define that object is alive.
         */
        bool        alive;

        /**
         * \brief Define that object started some action.
         */
        bool        started;

        /**
         * \brief Define that object finished some action.
         */
        bool        finished;

        /**
         * \brief Define that object attacked someone.
         */
        bool        attacked;

        /**
         * \brief Define that object hit someone.
         */
        bool        hit;

        /**
         * \brief Define that object hit only once someone.
         */
        bool        hit_once;

        /**
         * \brief Type of the object. Takes from meta-data in the map.
         */
        std::string type;

        /**
         * \brief Sprite for drawing.
         */
        sf::Sprite  sprite;

        /**
         * \brief ID of the object. Usually, defined by type variable.
         */
        Type::ID    type_ID;


    public:
        /**
         * \brief            Default constructor.
         * \param[in] id     The identifier of the current effect.
         * \param[in] X      The initial x-coordinate of this object.
         * \param[in] Y      The initial y-coordinate of this object.
         * \param[in] width  Width of this object.
         * \param[in] height Height if this object.
         * \param[in] type   An additional parameter for interaction with map.
         */
                      Effect(const Type::ID id, const float X, const float Y, const int width,
                             const int height, std::string type = "0");

        /**
         * \brief Default virtual destructor.
         */
        virtual       ~Effect() = default;

        /**
         * \brief Default copy constructor.
         */
                      Effect(const Effect& other) = default;

        /**
         * \brief Default move constructor.
         */
                      Effect(Effect&& other) = default;

        /**
         * \brief Default copy operator.
         */
        Effect&       operator=(const Effect& other) = default;

        /**
         * \brief Default move operator.
         */
        Effect&       operator=(Effect&& other) = default;

        /**
         * \brief   Calculate rectangle for object relatively world position.
         * \retrurn Created rectangle from object data.
         */
        sf::FloatRect get_rect() const;

        /**
         * \brief   Calculate object center relatively world position.
         * \retrurn Created vector from object data.
         */
        sf::Vector2f  get_center() const;

        /**
         * \brief   Get world position of the object.
         * \detail  Call get_world_position method.
         * \retrurn Calculated world position from world transform data.
         */
        sf::Vector2f  get_world_position() const;

        /**
         * \brief   Get world transform of the object.
         * \retrurn Calculated transform from sprite transform data.
         */
        sf::Transform get_world_transform() const;

        /**
         * \brief      Draw current object.
         * \param[out] Target to draw.
         */
        virtual void  draw(sf::RenderTarget& target) const;

        /**
         * \brief        Virtual funtion for updating this object and its states.
         * \param[in] dt Delta time parameter.
         */
        virtual void  update(const float dt) = 0;
};

#endif // EFFECT_HPP
