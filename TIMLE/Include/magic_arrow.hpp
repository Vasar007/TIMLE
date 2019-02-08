#pragma once

#include "projectile.hpp"


class MagicArrow final : public Projectile
{
    public:
        float mTargetX;
        float mTargetY;
        float mVx;
        float mVy;
        float mDistance;


    public:
             MagicArrow(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
                        const level& lvl, const float X, const float Y, const int width,
                        const int height, const float tX, const float tY);

        void update(const float dt) override;
};
