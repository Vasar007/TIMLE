#include "../Include/Rock.hpp"


Rock::Rock(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int height, std::string type)
: Entity(Id, X, Y, std::stoi(type) * 16, height, 50.f, 100, 0, type)
, mSize(std::stoi(type))
{
	mLevelObjects = lvl.getObjects("solid");

	mTexture = textures.get(Textures::RockDirt);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, 16 * mSize, height));
	mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
}

void Rock::fall()
{
	mIsAttacked = true;
}

void Rock::checkCollisionWithMap(float Dx, float Dy)
{
	for (size_t i = 0; i < mLevelObjects.size(); i++)
		// Проверяем пересечение с объектом
		if (getRect().intersects(mLevelObjects[i].mRect))
		{
			if (mLevelObjects[i].mName == "solid")
			{
				if (Dy > 0.f)
				{
					y = mLevelObjects[i].mRect.top - mHeight;
					dy = 0.f;
					mIsEnd = true;
				}
				if (Dy < 0.f)
				{
					y = mLevelObjects[i].mRect.top + mLevelObjects[i].mRect.height;
					dy = 0.f;
				}
			}
		}
}

void Rock::update(float time)
{
	if (mIsAttacked)
	{
		mCurrentFrame += 0.005f * time;

		// Притяжение к земле
		dy += 0.0005f * time;
		y += dy * time;
		checkCollisionWithMap(0.f, dy);

		mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
	}
}
