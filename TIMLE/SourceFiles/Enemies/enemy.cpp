#include "Utils/utility.hpp"

#include "Enemies/enemy.hpp"


namespace
{
    const std::map<Type::ID, EntityData> ENEMY_TABLE = initializeEnemyData();
}

Enemy::Enemy(const Type::ID id, const TextureHolder&, const FontHolder&, const level& lvl,
             const float X, const float Y, const int width, const int height, 
             const std::string& type)
: Entity(id, X, Y, width, height, ENEMY_TABLE.at(id).mSpeed, ENEMY_TABLE.at(id).mHitpoints,
         ENEMY_TABLE.at(id).mDamage, type, ENEMY_TABLE.at(id).mCalcBodyRect)
, mCounter(0)
, mIsTurned(false)
{
    // Инициализируем.получаем нужные объекты для взаимодействия врага с картой
    for (const auto& object : lvl.get_objects("enemyBorder"))
    {
        if (object.type == type)
        {
            mLevelObjects.push_back(object);
        }
    }
    utils::append(mLevelObjects, lvl.get_objects("death"));
}
