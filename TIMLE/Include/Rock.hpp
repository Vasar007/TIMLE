#ifndef ROCK_HPP
#define ROCK_HPP

#include "Entity.hpp"


class Rock : public Entity
{
    public:
        int mSize;

    
    public:
             Rock(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts, 
                  const Level& lvl, const float X, const float Y, const int height, 
                  const std::string& type);
    
        void fall();
        void checkCollisionWithMap(const float Dx, const float Dy);
        void update(const float dt) override;
};

#endif // ROCK_HPP
