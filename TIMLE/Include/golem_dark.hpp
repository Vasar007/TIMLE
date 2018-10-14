﻿#ifndef GOLEM_DARK_HPP
#define GOLEM_DARK_HPP

#include "enemy.hpp"


/**
 * \brief Class for mini-boss in the first level.
 */
class GolemDark final : public Entity
{
    private:
        /**
         * \brief The variable for the counting before we can destroy dead body.
         */
        int  counter_;

        /**
         * \brief The variable for turning the object.
         */
        bool turned_around_;
        

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
                       const Level& level, const float x_coord, const float y_coord,
                       const int width, const int height, const std::string& type = "0");
    

        /**
         * \brief        Check collision this object with other objects and special areas in the
         *               map.
         * \param[in] dX The current delta x-coordinate of this object for checking.
         * \param[in] dY The current delta y-coordinate of this object for checking.
         */
        void check_collision_with_map(const float dX, const float dY);

        /**
         * \brief        Overrided funtion for updating this object and its states.
         * \param[in] dt Delta time parameter.
         */
        void update(const float dt) override;
};

#endif // GOLEM_DARK_HPP