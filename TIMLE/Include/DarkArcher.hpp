#ifndef DARK_ARCHER_HPP
#define DARK_ARCHER_HPP

#include "Enemy.hpp"


/**
 * \brief An archer which shoots homing arrows.
 */
class DarkArcher final : public Enemy 
{
    public:
        /**
         * \brief            A constructor with basic parameters.
         * \param id        The identifier of the current entity.
         * \param textures    Textures holder for extracting necessary textures.
         * \param fonts        Fonts holder for extracting necessary textures.
         * \param lvl        The reference to the level-class object.
         * \param X            The initial x-coordinate of this object.
         * \param Y            The initial y-coordinate of this object.
         * \param width        Width of this object.
         * \param height    Height if this object.
         * \param type        An additional parameter for interaction with map.
         */
                DarkArcher(const Type::ID id, const TextureHolder& textures, 
                           const FontHolder& fonts, const Level& lvl, const float X, const float Y, 
                           const int width, const int height, const std::string& type = "0");


        /**
         * \brief        Check collision this object with other objects and special areas in the map.
         * \param Dx    The current delta x-coordinate of this object for checking.
         * \param Dy    The current delta y-coordinate of this object for checking.
         */
        void    checkCollisionWithMap(const float Dx, const float Dy) override;

        /**
         * \brief        Overrided funtion for updating this object and its states.
         * \param dt    Delta time parameter.
         */
        void    update(const float dt) override;
};

#endif // DARK_ARCHER_HPP
