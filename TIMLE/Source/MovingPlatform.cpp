#include "../Include/MovingPlatform.hpp"


MovingPlatform::MovingPlatform(Type::ID id, const TextureHolder& textures, const FontHolder&,
                               const Level& lvl, const float X, const float Y, const int width, 
                               const int height, const std::string& type)
: Entity(id, X, Y, width, height, 50.f, 100, 0, type)
{
    for (const auto& object : lvl.getObjects("platformBorder"))
    {
        if (object.mType == type)
        {
            mLevelObjects.push_back(object);
        }
    }

    mTexture = textures.get(Textures::ID::MovingPlatform);
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, width, height));
    mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
    // ����������� ���������
    dx = 0.08f;
}

void MovingPlatform::checkCollisionWithMap()
{
    for (auto& mLevelObject : mLevelObjects)
    {
        // ��������� ����������� � ��������
        if (getRect().intersects(mLevelObject.mRect))
        {
            dx = -dx;
            dy = -dy;
        }
    }
}

void MovingPlatform::update(const float dt)
{
    // �������� �� �����������
    x += dx * dt;
    checkCollisionWithMap();

    //mMoveTimer += dt;
    //if (mMoveTimer > 2000.f)
    //{
    //    dx *= -1.f;
    //    // ���� ������ 2 �������, �� ������ ����������� �������� ��������� �� ���������������
    //    mMoveTimer = 0.f;
    //}
    mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
}