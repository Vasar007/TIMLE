#pragma once

#include "entity.hpp"


/**
 * \brief Abstract interim class for enemies entities.
 */
class Enemy : public Entity 
{
    protected:
        /**
         * \brief The variable for the counting before we can destroy dead body.
         */
        int  mCounter;

        /**
         * \brief The variable for turning the object.
         */
        bool mIsTurned;


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
                     Enemy(Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
                           const level& lvl, const float X, const float Y, const int width, 
                           const int height, const std::string& type = "0");

        /**
         * \brief Default destructor for correct deleting derived objects.
         */
        virtual      ~Enemy() = default;

        /**
         * \brief Deleted copy constructor.
         */
                     Enemy(const Enemy& other) = default;

        /**
         * \brief Deleted move constructor.
         */
                     Enemy(Enemy&& other) = default;

        /**
         * \brief Deleted copy operator=.
         */
        Enemy&       operator=(const Enemy& other) = default;

        /**
         * \brief Deleted move operator=.
         */
        Enemy&       operator=(Enemy&& other) = default;

        /**
         * \brief        Check collision this object with other objects and special areas in the
         *               map.
         * \param[in] Dx The current delta x-coordinate of this object for checking.
         * \param[in] Dy The current delta y-coordinate of this object for checking.
         */
        virtual void checkCollisionWithMap(const float Dx, const float Dy) = 0;

        /**
         * \brief        Pure virtual and overrided funtion for updating this object and its states.
         * \param[in] dt Delta time parameter.
         */
        void         update(const float dt) override = 0;
};
