#ifndef DEADMAN_HPP
#define DEADMAN_HPP

#include "Entity.hpp"


class DeadMan final : public Entity
{
    public:
        int mDialogNumber;


    public:
             DeadMan(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts, 
                     const Level& lvl, const float X, const float Y, const int width, 
                     const int height, const std::string& type);

        void update(const float dt) override;
};

#endif // DEADMAN_HPP
