#pragma once

#include "Interface/level.hpp"
#include "Mechanics/data_tables.hpp"
#include "Mechanics/resource_holder.hpp"
#include "Objects/entity.hpp"


/**
 * \brief Class for mini-boss in the first level.
 */
class GolemDark final : public Entity
{
    private:
        /**
         * \brief The variable for the counting before we can destroy dead body.
         */
        int  _counter;

        /**
         * \brief The variable for turning the object.
         */
        bool _turned_around;
        

    public:
        /**
         * \brief              A constructor with basic parameters.
         * \param[in] id       The identifier of the current entity.
         * \param[in] textures The reference to an associative array with textures.
         * \param[in] fonts    The reference to an associative array with fonts.
         * \param[in] level    The reference to the level-class object.
         * \param[in] x_coord  The initial x-coordinate of this object.
         * \param[in] y_coord  The initial y-coordinate of this object.
         * \param[in] width    Width of this object.
         * \param[in] height   Height if this object.
         * \param[in] type     An additional parameter for interaction with map.
         */
             GolemDark(const Type::ID id, const TextureHolder& textures,  const FontHolder& fonts,
                       const level& level, const float x_coord, const float y_coord,
                       const int width, const int height, const std::string& type = "0");
    

        /**
         * \brief         Check collision this object with other objects and special areas in the
         *                map.
         * \param[in] dx_ The current delta x-coordinate of this object for checking.
         * \param[in] dy_ The current delta y-coordinate of this object for checking.
         */
        void check_collision_with_map(const float dx_, const float dy_);

        /**
         * \brief        Overridden function for updating this object and its states.
         * \param[in] dt Delta time parameter.
         */
        void update(const float dt) override;
};
