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
		float				y;	// ����������
		float				dx;	// ��������� �� x
		float				dy;	// ��������� �� y
		float				mSpeed;	// ��������
		float				mMoveTimer;	// ������
		float				mCurrentFrame;	// ������ ��������
		float				mCurrentAttack;
		float				mCurrentDeath;

		int					mWidth;	// ������
		int					mHeight;	// ������
		int					mHitpoints;	// ���������� ��������
		int					mDamage;

		bool				mLife; // ����� ��������
		bool				mIsMove;	// ���������� ��������
		bool				mOnGround;	// ���������� �� �����
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

		std::string			mType;	// ��� ��������, �������� �� TMX-�����

		sf::Sprite			mSprite;
		sf::Texture			mTexture;
		sf::Texture			mTextureAttack;
		sf::Texture			mTextureDeath;
		Type::ID			mTypeID;
	
		std::vector<Object>	mLevelObjects;	// ������ �������� �����
	
	
	public:
							Entity(Type::ID Id, float X, float Y, int width, int height, float speed, int hitpoints, int damage, std::string Type = "0");
		virtual				~Entity();
	
		sf::FloatRect		getRect() const;	// ������� ��������� ��������������, ��� ��������� � ��������(������, ������)
	
		virtual void		update(float dt) = 0;
};

#endif // ENTITY_HPP
