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
    auto levelObjects = lvl.getObjects("enemyBorder");
    for (const auto& object : levelObjects)
    {
        if (object.mType == type)
        {
            mLevelObjects.push_back(object);
        }
    }

    levelObjects = lvl.getObjects("death");
    for (const auto& object : levelObjects)
    {
        mLevelObjects.push_back(object);
    }
}
