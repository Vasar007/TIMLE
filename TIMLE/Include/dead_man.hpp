#pragma once

#include "entity.hpp"


class DeadMan final : public Entity
{
    public:
             DeadMan(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts, 
                     const level& lvl, const float X, const float Y, const int width, 
                     const int height, const std::string& type);

        void update(const float dt) override;
};
