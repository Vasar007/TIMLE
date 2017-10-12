#include "../Include/Projectile.hpp"
#include "../Include/DataTables.hpp"
#include "../Include/Utility.hpp"


namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts,
					   Level &lvl, float X, float Y, int width, int height)
: Entity(Id, X, Y, width, height, Table[Id - Type::HeroCount - Type::EnemyCount].speed, 100,
		 Table[Id - Type::HeroCount - Type::EnemyCount].damage)
, mTargetDirection()
, mGuided(false)
{
}

void Projectile::guideTowards(sf::Vector2f position)
{
	mTargetDirection = unitVector(position - getWorldPosition());
}

bool Projectile::isGuided() const
{
	return mGuided;
}

//void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
//{
//	if (isGuided())
//	{
//		const float approachRate = 200.f;
//
//		sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection +
//								   getVelocity());
//		newVelocity *= getMaxSpeed();
//		float angle = std::atan2(newVelocity.y, newVelocity.x);
//
//		setRotation(toDegree(angle) + 90.f);
//		setVelocity(newVelocity);
//	}
//
//	Entity::updateCurrent(dt, commands);
//}

unsigned int Projectile::getCategory() const
{
	if (mTypeID == (Type::EnemyBullet | Type::Fireball | Type::Flamestrike))
	{
		return EnemyProjectile;
	}
	else
	{
		return AlliedProjectile;
	}
}

sf::FloatRect Projectile::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
	return Table[mTypeID - Type::HeroCount - Type::EnemyCount].speed;
}

int Projectile::getDamage() const
{
	return Table[mTypeID - Type::HeroCount - Type::EnemyCount].damage;
}
