#include "../Include/Enemy.hpp"


namespace
{
    const std::vector<EntityData> ENEMY_TABLE = initializeEnemyData();
}

Enemy::Enemy(const Type::ID id, const TextureHolder&, const FontHolder&, const Level& lvl,
             const float X, const float Y, const int width, const int height, 
             const std::string& type)
: Entity(id, X, Y, width, height, ENEMY_TABLE[id - Type::HERO_COUNT].mSpeed, 
         ENEMY_TABLE[id - Type::HERO_COUNT].mHitpoints, ENEMY_TABLE[id - Type::HERO_COUNT].mDamage,
         type, ENEMY_TABLE[id - Type::HERO_COUNT].mCalcBodyRect)
, mCounter(0)
, mIsTurned(false)
{
    // Инициализируем.получаем нужные объекты для взаимодействия врага с картой
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
