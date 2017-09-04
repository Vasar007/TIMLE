#ifndef BULLET_HPP
#define BULLET_HPP

#include "Entity.hpp"


class Bullet : public Entity
{
	public:
		float			mTargetX;
		float			mTargetY;	// �������� �� ����������� �������� ���� � ����
		float			mVx;
		float			mVy;
		float			mDistance;

		bool			mIsMouse;
	
	
	public:
	
						Bullet(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, float tX, float tY);	// ��� ����� ������
										    
						Bullet(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, int dir);	// ��� ������� space
	
		virtual void	update(float time);
};

#endif // BULLET_HPP
