#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Level.hpp"
#include "ResourceHolder.hpp"
#include "DataTables.hpp"

#include <SFML/Graphics.hpp>


class Entity 
{
	public:
		float				x;
		float				y;	// Координаты
		float				dx;	// Ускорение по x
		float				dy;	// Ускорение по y
		float				mSpeed;	// Скорость
		float				mMoveTimer;	// Таймер
		float				mCurrentFrame;	// Таймер анимации
		float				mCurrentAttack;
		float				mCurrentDeath;

		int					mWidth;	// Ширина
		int					mHeight;	// Высота
		int					mHitpoints;	// Количество здоровья
		int					mDamage;

		bool				mLife; // Жизнь сущности
		bool				mIsMove;	// Разрешение движения
		bool				mOnGround;	// Нахождение на земле
		bool				mIsAttacked;
		bool				mIsHitted;
		bool				mIsHittedOnce;
		bool				mIsStarted;
		bool				mIsBack;
		bool				mIsSpawn;
		bool				mIsEnd;
		bool				mIsEnabled;
		bool				mIsDisabled;
		bool				mIsEnabling;

		std::string			mType;	// Тип сущности, получаем из TMX-карты

		sf::Sprite			mSprite;
		sf::Texture			mTexture;
		sf::Texture			mTextureAttack;
		sf::Texture			mTextureDeath;
		Type::ID			mTypeID;
	
		std::vector<Object>	mLevelObjects;	// Вектор объектов карты
	
	
	public:
							Entity(Type::ID Id, float X, float Y, int width, int height, float speed, int hitpoints, int damage, std::string Type = "0");
		virtual				~Entity();
	
		sf::FloatRect		getRect() const;	// Функция получения прямоугольника, его координат и размеров(ширина, высота)
	
		virtual void		update(float dt) = 0;
};

#endif // ENTITY_HPP
