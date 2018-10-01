#ifndef BLOODSPLAT_HPP
#define BLOODSPLAT_HPP

#include "Effect.hpp"


class Bloodsplat final : public Effect
{
    public:
             Bloodsplat(const Type::ID id, const TextureHolder& textures, const float X,
                        const float Y, const int width, const int height,
                        const std::string& type = "0");

        void update(const float dt) override;
};

#endif // BLOODSPLAT_HPP
