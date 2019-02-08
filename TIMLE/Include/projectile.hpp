#pragma once

#include "entity.hpp"


class Projectile : public Entity, sf::Transformable
{
    private:
        enum Category : unsigned int
        {
            EnemyProjectile,
            AlliedProjectile
        };


    private:
        sf::Vector2f          _targetDirection;
        bool                  _guided;


    public:
                              Projectile(const Type::ID id, const TextureHolder& textures,
                                         const FontHolder& fonts, const level& lvl, const float X,
                                         const float Y, const int width, const int height);

        void                  guideTowards(const sf::Vector2f position);
        bool                  isGuided() const;

        void                  updateDirection(float dt);

        virtual unsigned int  getCategory() const;
        virtual sf::FloatRect getBoundingRect() const;
        float                 getMaxSpeed() const;
        int                   getDamage() const;

        /**
         * \brief        Pure virtual and overrided funtion for updating this object and its states.
         * \param[in] dt Delta time parameter.
         */
        void                  update(float dt) override = 0;
};
