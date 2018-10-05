#include "../Include/Enemy.hpp"


namespace
{
    const std::map<Type::ID, EntityData> ENEMY_TABLE = initializeEnemyData();
}

Enemy::Enemy(const Type::ID id, const TextureHolder&, const FontHolder&, const Level& lvl,
             const float X, const float Y, const int width, const int height, 
             const std::string& type)
: Entity(id, X, Y, width, height, ENEMY_TABLE.at(id).mSpeed, ENEMY_TABLE.at(id).mHitpoints,
         ENEMY_TABLE.at(id).mDamage, type, ENEMY_TABLE.at(id).mCalcBodyRect)
, mCounter(0)
, mIsTurned(false)
{
    // ��������������.�������� ������ ������� ��� �������������� ����� � ������
    for (const auto& object : lvl.getObjects("enemyBorder"))
    {
        if (object.mType == type)
        {
            mLevelObjects.push_back(object);
        }
    }

    for (const auto& object : lvl.getObjects("death"))
    {
        mLevelObjects.push_back(object);
    }
}
