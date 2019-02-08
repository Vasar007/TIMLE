#pragma once

#include "projectile.hpp"


class Flamestrike final : public Projectile
{
    public:
             Flamestrike(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
                         const level& lvl, const float X, const float Y, const int width,
                         const int height);
    
        void update(const float dt) override;
};
