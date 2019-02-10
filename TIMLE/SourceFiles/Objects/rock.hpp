#pragma once

#include "Objects/entity.hpp"


class Rock : public Entity
{
    public:
        int mSize;

    
    public:
             Rock(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts, 
                  const level& lvl, const float X, const float Y, const int height, 
                  const std::string& type);
    
        void fall();
        void checkCollisionWithMap(const float Dx, const float Dy);
        void update(const float dt) override;
};
