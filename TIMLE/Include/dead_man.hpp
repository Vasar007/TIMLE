#ifndef DEAD_MAN_HPP
#define DEAD_MAN_HPP

#include "entity.hpp"


class DeadMan final : public Entity
{
    public:
             DeadMan(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts, 
                     const Level& lvl, const float X, const float Y, const int width, 
                     const int height, const std::string& type);

        void update(const float dt) override;
};

#endif // DEAD_MAN_HPP
