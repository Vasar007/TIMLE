#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

#include "Level.hpp"
#include "ResourceHolder.hpp"
#include "DataTables.hpp"


class Entity 
{
	public:
		typedef std::function<sf::FloatRect(float left, float top,
											float width, float height, 
											float direction)> FuncCalcBodyRect;


	private:
		FuncCalcBodyRect	_calcBodyRect;
		sf::Vector2f		_velocity;


	public:
		float				x;
		float				y;
		float				dx;	// Ускорение по x
		float				dy;	// Ускорение по y
		float				mSpeed;	// Скорость
		float				mMoveTimer;
		float				mDeathTimer;
		float				mCurrentFrame;	// Таймер анимации
		float				mCurrentAttack;
		float				mCurrentDeath;

		int					mWidth;	// Ширина
		int					mHeight;	// Высота

		float				mBodyX;
		float				mBodyY;
		float				mBodyWidth;
		float				mBodyHeight;

		int					mHitpoints;	// Количество здоровья
		std::size_t			mDamage;

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
		Entity(const Type::ID id, const float X, const float Y, const int width, const int height,
			   const float speed, const int hitpoints, const  std::size_t damage,
		       std::string type = "0", const FuncCalcBodyRect& calcBodyRect = nullptr);

		virtual				~Entity() = default;

		Entity(const Entity& other) = default;

		Entity(Entity&& other) = default;

		Entity& operator=(const Entity& other) = default;

		Entity& operator=(Entity&& other) = default;
	
		void				setVelocity(const sf::Vector2f velocity);
		void				setVelocity(const float vx, const float vy);
		sf::Vector2f		getVelocity() const;

		// Функция получения прямоугольника, его координат и размеров(ширина, высота).
		sf::FloatRect		getRect() const;

		sf::FloatRect		getBodyRect() const;

		sf::Vector2f		getCenter() const;
		sf::Vector2f		getWorldPosition() const;
		sf::Transform		getWorldTransform() const;
		virtual void		draw(sf::RenderTarget& target) const;
	
		virtual void		update(const float dt) = 0;
};

#endif // ENTITY_HPP
