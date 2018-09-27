#ifndef MAGIC_ARROW_HPP
#define MAGIC_ARROW_HPP

#include "Projectile.hpp"


class MagicArrow final : public Projectile
{
    public:
        float    mTargetX;
        float    mTargetY;    // Отвечают за направление движения пули к цели
        float    mVx;
        float    mVy;
        float    mDistance;
    
    
    public:
                MagicArrow(const Type::ID id, const TextureHolder& textures, 
                           const FontHolder& fonts, const Level& lvl, const float X, const float Y, 
                           const int width, const int height, const float tX, const float tY);

        void    update(const float dt) override;
};

#endif // MAGIC_ARROW_HPP
