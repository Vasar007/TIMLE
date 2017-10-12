#include "../Include/Flamestrike.hpp"


Flamestrike::Flamestrike(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, 
						 Level &lvl, float X, float Y, int width, int height)
: Projectile(Id, textures, fonts, lvl, X, Y, width, height)
{
	mTexture = textures.get(Textures::Fire);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, width, height));
	mSprite.setScale(0.5f, 0.5f);
	mIsEnd = false;
}

void Flamestrike::update(float dt)
{
	mCurrentFrame += 0.0075f * dt;
	if (mCurrentFrame > 24.f)
	{
		mCurrentFrame -= 24.f;
		mIsEnd = true;
	}

	if (mIsAttacked)
	{
		mCurrentAttack += 0.0075f * dt;
		if (mCurrentAttack > 2.f)
		{
			mCurrentAttack -= 2.f;
			mIsAttacked = false;
			mIsHittedOnce = false;
		}
		else if (static_cast<int>(mCurrentAttack) == 1 && !mIsHittedOnce)
		{
			mIsHitted = true;
			mIsHittedOnce = true;
		}
		else
		{
			mIsHitted = false;
		}
	}
	else
	{
		mCurrentAttack = 0.f;
	}

	mSprite.setTextureRect(sf::IntRect(128 * (static_cast<int>(mCurrentFrame) % 8),
						   256 * (static_cast<int>(mCurrentFrame) / 8), 128, 256));


	mSprite.setPosition(x + (mWidth / 2.f) - 30.f, y + (mHeight / 2.f) - 91.f);

	if (mIsEnd)
	{
		mCurrentDeath += 0.0075f * dt;
		
		auto color = mSprite.getColor();

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
