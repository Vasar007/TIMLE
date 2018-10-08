#ifndef FIREBALL_HPP
#define FIREBALL_HPP

#include "projectile.hpp"


class Flamestrike final : public Projectile
{
    public:
             Flamestrike(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
                         const Level& lvl, const float X, const float Y, const int width,
                         const int height);
    
        void update(const float dt) override;
};

#endif // FIREBALL_HPP
