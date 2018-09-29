#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <SFML/Graphics.hpp>

#include "DataTables.hpp"
#include "ResourceHolder.hpp"


class Effect
{
    public:
        float       x;
        float       y;
        int         mWidth;
        int         mHeight;

        float       mCurrentFrame;
        float       mCurrentAttack{};
        float       mCurrentDeath{};
        float       mMoveTimer{};

        bool        mLife{};
        bool        mIsStarted;
        bool        mIsEnd;
        bool        mIsAttacked{};
        bool        mIsHittedOnce{};
        bool        mIsHitted{};

        std::string mType;

        sf::Sprite  mSprite;
        sf::Texture mTexture;
        Type::ID    mTypeID;


    public:
                      Effect(const Type::ID id, const float X, const float Y, const int width,
                             const int height, std::string type = "0");

        virtual       ~Effect() = default;

                      Effect(const Effect& other) = default;

                      Effect(Effect&& other) = default;

        Effect&       operator=(const Effect& other) = default;

        Effect&       operator=(Effect&& other) = default;

        sf::FloatRect getRect() const;

        sf::Vector2f  getCenter() const;
        sf::Vector2f  getWorldPosition() const;
        sf::Transform getWorldTransform() const;
        virtual void  draw(sf::RenderTarget& target) const;

        virtual void  update(const float dt) = 0;
};

#endif // EFFECT_HPP
