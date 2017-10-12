#include "../Include/Enemy.hpp"


namespace
{
	const std::vector<EntityData> ETable = initializeEnemyData();
}

Enemy::Enemy(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl,
			 float X, float Y, int width, int height, std::string Type)
: Entity(Id, X, Y, width, height, ETable[Id - Type::HeroCount].speed, 
		 ETable[Id - Type::HeroCount].hitpoints, ETable[Id - Type::HeroCount].damage, Type)
, mCounter(0)
, mIsTurned(false)
{
	// Инициализируем.получаем нужные объекты для взаимодействия врага с картой
	std::vector<Object> levelObjects = lvl.getObjects("enemyBorder");
	for (size_t i = 0; i < levelObjects.size(); i++)
	{
		if (levelObjects[i].mType == Type)
		{
			mLevelObjects.push_back(levelObjects[i]);
		}
	}

	levelObjects = lvl.getObjects("death");
	for (size_t i = 0; i < levelObjects.size(); i++)
	{
		mLevelObjects.push_back(levelObjects[i]);
	}

	/*
	for (size_t i = 0; i < mLevelObjects.size(); i++)
		// Проверяем пересечени с объектом
		if (getRect().intersects(mLevelObjects[i].rect))
		{
			// Если встретили препятствие
			if (mLevelObjects[i].name == "solid")
			{
				if (Dy > 0)
				{
					y = mLevelObjects[i].rect.top - mHeight;
					dy = 0;
					onGround = true;
				}
				if (Dy < 0)
				{
					y = mLevelObjects[i].rect.top + mLevelObjects[i].rect.height;
					dy = 0;
				}
				if (Dx > 0)
				{
					x = mLevelObjects[i].rect.left - mWidth;
					isTurned = true;
					dx = -0.075f;
					//sprite.scale(-1, 1);
				}
				if (Dx < 0)
				{
					x = mLevelObjects[i].rect.left + mLevelObjects[i].rect.width;
					isTurned = true;
					dx = 0.075f;
					//sprite.scale(-1, 1);
				}
			}
		}
	*/
}
