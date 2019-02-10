#pragma once

#include "Objects/entity.hpp"


class DialogPerson final : public Entity
{
    public:
        int mDialogNumber;


    public:
             DialogPerson(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
                          const level& lvl, const float X, const float Y, const int width,
                          const int height, const std::string& type);

        void update(const float dt) override;
};
