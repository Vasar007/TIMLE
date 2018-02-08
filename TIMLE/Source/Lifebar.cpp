#include "../Include/Lifebar.hpp"


LifeBar::LifeBar(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
				 const int hitpoints)
: _max(hitpoints)
, _type(id)
, _bossName("", fonts.get(Fonts::ID::Main))
{
	if (id == Type::ID::ShadowBossBar)
	{
		_bossName.setString(L"Страж крепости");
		_texture = textures.get(Textures::ID::BossBar);
		_sprite.setTexture(_texture);
		_spriteEmpty.setTexture(_texture);
		_sprite.setTextureRect(sf::IntRect(0, 0, 334, 24));
		_spriteEmpty.setTextureRect(sf::IntRect(0, 24, 334, 24));
		_bossName.setScale(0.5f, 0.5f);
		_spriteEmpty.setScale(0.5f, 0.5f);
		_sprite.setScale(0.5f, 0.5f);
	}
	else if (id == Type::ID::GolemDarkBossBar)
	{
		_bossName.setString(L"Хранитель руны");
		_texture = textures.get(Textures::ID::BossBar);
		_sprite.setTexture(_texture);
		_spriteEmpty.setTexture(_texture);
		_sprite.setTextureRect(sf::IntRect(0, 0, 334, 24));
		_spriteEmpty.setTextureRect(sf::IntRect(0, 24, 334, 24));
		_bossName.setScale(0.5f, 0.5f);
		_spriteEmpty.setScale(0.5f, 0.5f);
		_sprite.setScale(0.5f, 0.5f);
	}
	else
	{
		_texture = textures.get(Textures::ID::HealthBar);
		_textureEmpty = textures.get(Textures::ID::EmptyBar);
		_sprite.setTexture(_texture);
		_spriteEmpty.setTexture(_textureEmpty);
		_spriteEmpty.setScale(0.5f, 0.5f);
		_sprite.setScale(0.5f, 0.5f);
	}
}

void LifeBar::update(int points)
{
	if (points < 0)
	{
		points = 0;
	}
	if (points <= _max)
	{
		if ((_type == Type::ID::ShadowBossBar) || (_type == Type::ID::GolemDarkBossBar))
		{
			_sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(334 * 
								   (static_cast<float>(points) / static_cast<float>(_max))), 24));
		}
		else
		{
			_sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(206 * 
								   (static_cast<float>(points) / static_cast<float>(_max))), 28));
		}
	}
}

void LifeBar::draw(sf::RenderWindow& window)
{
	const sf::Vector2f center = window.getView().getCenter();
	const sf::Vector2f size = window.getView().getSize();

	if ((_type == Type::ID::ShadowBossBar) || (_type == Type::ID::GolemDarkBossBar))
	{
		_bossName.setPosition(center.x - 50.f, center.y + size.y / 2.f - 60.f);
		_sprite.setPosition(center.x - 70.f, center.y + size.y / 2.f - 40.f);
		_spriteEmpty.setPosition(center.x - 70.f, center.y + size.y / 2.f - 40.f);
	}
	else
	{
		_sprite.setPosition(center.x - size.x / 2.f + 10.f, center.y - size.y / 2.f + 20.f);
		_spriteEmpty.setPosition(center.x - size.x / 2.f + 10.f, center.y - size.y / 2.f + 20.f);
	}

	if ((_type == Type::ID::ShadowBossBar) || (_type == Type::ID::GolemDarkBossBar))
	{
		window.draw(_bossName);
	}

	window.draw(_spriteEmpty);
	window.draw(_sprite);
}
