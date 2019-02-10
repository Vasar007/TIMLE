#pragma once

#include "Objects/entity.hpp"


class MovingPlatform final : public Entity 
{
    public:
             MovingPlatform(const Type::ID id, const TextureHolder& textures, 
                            const FontHolder& fonts, const level& lvl, const float X, const float Y,
                            const int width, const int height, const std::string& type = "0");

        void checkCollisionWithMap();

        void update(const float dt) override;
};
