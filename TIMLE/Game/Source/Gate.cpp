#include "../Include/Gate.hpp"


Gate::Gate(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, std::string type)
: Entity(Id, X, Y, width, std::stoi(type) * 16, 50.f, 100, 0, type)
, mSize(std::stoi(type))
, mWaySizeDown(Y + mSize * 16.f)
, mWaySizeUp(Y)
{
	mLevelObjects = lvl.getObjects("solid");

	if (type == "3")
		mTexture = textures.get(Textures::GateDirt);
	else if (type == "4")
		mTexture = textures.get(Textures::GateDirt);
	else if (type == "5")
		mTexture = textures.get(Textures::GateWood);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, width, 16 * mSize));
	mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
	dy = 0.05f;
}

void Gate::close()
{
	mIsAttacked = true;
}

void Gate::checkCollisionWithMap(float Dx, float Dy)
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

void Gate::update(float time)
{
	switch (mTypeID)
	{
		case Type::OpeningGate:
			if (mIsStarted)
			{
				mCurrentFrame += 0.005f * time;

				y += dy * time;
				if (y >= mWaySizeDown)
				{
					dy = 0.f;
					mIsEnd = true;
				}

				mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
			}
			break;
		case Type::ClosingGate:
			if (mIsAttacked)
			{
				mCurrentFrame += 0.005f * time;

				y += dy * time;
				if (y >= mWaySizeDown)
				{
					dy = 0.f;
					mIsEnd = true;
				}

				mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
			}
			break;
		case Type::OpenClosingGate:
			mCurrentFrame += 0.005f * time;

			y += dy * time;
			if ((y >= mWaySizeDown && !mIsEnd) || (y <= mWaySizeUp && mIsEnd))
			{
				if (dy > 0.f)
					mIsEnd = true;
				else
					mIsEnd = false;

				dy = -dy;
			}

			mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
			break;
		default:
			std::cout << "Invalid object type!" << std::endl;
			break;
	}
}
