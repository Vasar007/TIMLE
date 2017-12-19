#include "../Include/Bloodsplat.hpp"


Bloodsplat::Bloodsplat(const Type::ID id, const TextureHolder& textures, const float X,
					   const float Y, const int width, const int height, const std::string& type)
: Effect(id, X, Y, width, height, type)
{
	mTexture = textures.get(Textures::ID::Bloodsplat);

	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, 48, 24));
	//mSprite.setScale(0.5f, 0.5f);
	const auto color = sf::Color(mSprite.getColor().r, mSprite.getColor().g, mSprite.getColor().b, 0);
	mSprite.setColor(color);

	mIsStarted	= true;
	mIsEnd		= false;
}

void Bloodsplat::update(const float dt)
{
	if (mIsStarted)
	{
		mMoveTimer += 0.0085f * dt;

		const auto color = mSprite.getColor();

		if (mMoveTimer > 5.f)
		{
			mIsStarted = false;
			mMoveTimer = -5.f;
		}
		else
		{
			if (color.a + static_cast<sf::Uint8>(mMoveTimer) * 5 > 255)
			{
				mSprite.setColor(sf::Color(color.r, color.g, color.b, 255));
			}
			else
			{
				mSprite.setColor(sf::Color(color.r, color.g, color.b,
								 color.a + static_cast<sf::Uint8>(mMoveTimer) * 5));
			}
		}
	}

	mCurrentFrame += 0.0075f * dt;
	if (mCurrentFrame > 13.f)
	{
		mCurrentFrame -= 13.f;
		mIsEnd = true;
	}

	mSprite.setTextureRect(sf::IntRect(48 * static_cast<int>(mCurrentFrame), 0, 48, 24));


	mSprite.setPosition(x + (mWidth / 2.f) - 10.f, y + (mHeight / 2.f) - 5.f);

	if (mIsEnd)
	{
		mCurrentDeath += 0.0075f * dt;

		const auto color = mSprite.getColor();

		if (mCurrentDeath > 5.f)
		{
			mLife = false;
			mCurrentDeath = -5.f;
		}
		else
		{
			if (color.a - static_cast<sf::Uint8>(mCurrentDeath) * 5 < 0)
			{
				mSprite.setColor(sf::Color(color.r, color.g, color.b, 0));
			}
			else
			{
				mSprite.setColor(sf::Color(color.r, color.g, color.b,
					color.a - static_cast<sf::Uint8>(mCurrentDeath) * 5));
			}
		}
	}
}
