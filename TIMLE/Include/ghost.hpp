#ifndef GHOST_HPP
#define GHOST_HPP

#include "enemy.hpp"


/**
 * \brief Class for the entity, which can passing through walls.
 */
class Ghost final : public Enemy 
{
    public:
        /**
         * \brief              A constructor with basic parameters.
         * \param[in] id       The identifier of the current entity.
         * \param[in] textures Textures holder for extracting necessary textures.
         * \param[in] fonts    Fonts holder for extracting necessary textures.
         * \param[in] lvl      The reference to the level-class object.
         * \param[in] X        The initial x-coordinate of this object.
         * \param[in] Y        The initial y-coordinate of this object.
         * \param[in] width    Width of this object.
         * \param[in] height   Height if this object.
         * \param[in] type     An additional parameter for interaction with map.
         */
             Ghost(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
                   const Level& lvl, const float X, const float Y, const int width,
                   const int height, const std::string& type = "0");


        /**
         * \brief        Check collision this object with other objects and special areas in the
         *               map.
         * \param[in] Dx The current delta x-coordinate of this object for checking.
         * \param[in] Dy The current delta y-coordinate of this object for checking.
         */
        void checkCollisionWithMap(const float Dx, const float Dy) override;

        /**
         * \brief        Overrided funtion for updating this object and its states.
         * \param[in] dt Delta time parameter.
         */
        void update(const float dt) override;
};

#endif // GHOST_HPP
