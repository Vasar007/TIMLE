#pragma once

#include "enemy.hpp"


/**
 * \brief Pretty casual class for shadow's tentacles.
 */
class Tentacle final : public Enemy 
{
    private:
        /**
         * \brief Counter for helping with setting tentacles' sprites while they are appearing.
         */
        float _appearing;

        /**
         * \brief Counter for helping with setting tentacles' sprites while they are disappearing.
         */
        float _disappearing;


    private:
        /**
         * \brief        Helper function for appearing this object.
         * \param[in] dt Delta time parameter.
         */
        void appear(const float dt);

        /**
         * \brief        Helper function for disappearing this object.
         * \param[in] dt Delta time parameter.
         */
        void disappear(const float dt);


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
             Tentacle(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
                      const level& lvl, const float X, const float Y, const int width,
                      const int height, const std::string& type);


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
