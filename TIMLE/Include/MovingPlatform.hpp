#ifndef MOVING_PLATFORM_HPP
#define MOVING_PLATFORM_HPP

#include "Entity.hpp"


class MovingPlatform final : public Entity 
{
    public:
             MovingPlatform(const Type::ID id, const TextureHolder& textures, 
                            const FontHolder& fonts, const Level& lvl, const float X, const float Y,
                            const int width, const int height, const std::string& type = "0");

        void checkCollisionWithMap();

        void update(const float dt) override;
};

#endif // MOVING_PLATFORM_HPP
