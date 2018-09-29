#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

#include "Level.hpp"
#include "ResourceHolder.hpp"
#include "DataTables.hpp"


class Entity 
{
    public:
        using FuncCalcBodyRect = std::function<sf::FloatRect(float left, float top,
                                               float width, float height, 
                                               float direction)>;


    private:
        FuncCalcBodyRect _calcBodyRect;
        sf::Vector2f     _velocity;


    public:
        float               x;
        float               y;
        float               dx;
        float               dy;
        float               mSpeed;
        float               mMoveTimer;
        float               mDeathTimer;
        float               mCurrentFrame;
        float               mCurrentAttack;
        float               mCurrentDeath;

        int                 mWidth;
        int                 mHeight;

        float               mBodyX;
        float               mBodyY;
        float               mBodyWidth;
        float                mBodyHeight;

        int                 mHitpoints;
        std::size_t         mDamage;

        bool                mLife;
        bool                mIsMove;
        bool                mOnGround;
        bool                mIsAttacked;
        bool                mIsHitted;
        bool                mIsHittedOnce;
        bool                mIsStarted;
        bool                mIsBack;
        bool                mIsSpawn;
        bool                mIsEnd;
        bool                mIsEnabled;
        bool                mIsDisabled;
        bool                mIsEnabling;

        std::string         mType;

        sf::Sprite          mSprite;
        sf::Texture         mTexture;
        sf::Texture         mTextureAttack;
        sf::Texture         mTextureDeath;
        Type::ID            mTypeID;
    
        std::vector<Object> mLevelObjects;

    
    public:
                     Entity(const Type::ID id, const float X, const float Y, const int width,
                            const int height, const float speed, const int hitpoints,
                            const  std::size_t damage, std::string type = "0",
                            const FuncCalcBodyRect& calcBodyRect = nullptr);

        virtual      ~Entity() = default;

                     Entity(const Entity& other) = default;

                     Entity(Entity&& other) = default;

        Entity&      operator=(const Entity& other) = default;

        Entity&      operator=(Entity&& other) = default;
    
        void         setVelocity(const sf::Vector2f velocity);
        void         setVelocity(const float vx, const float vy);
        sf::Vector2f getVelocity() const;

        sf::FloatRect getRect() const;

        sf::FloatRect getBodyRect() const;

        sf::Vector2f  getCenter() const;
        sf::Vector2f  getWorldPosition() const;
        sf::Transform getWorldTransform() const;
        virtual void  draw(sf::RenderTarget& target) const;
    
        virtual void  update(const float dt) = 0;
};

#endif // ENTITY_HPP
