#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "DataTables.hpp"
#include "PlayerInfo.hpp"


class Player : public Entity {
public:
	enum State
	{
		Left,
		Right,
		Up,
		Down,
		Jump,
		Stay
	};


public:
	float			mStayTimer; // Таймер анимации
	float			mOnPlatform;
	float			mShootTimer;
	float			mJumpTimer;
	float			mBeforeJump;
	float			mAfterJump;
	float			mFallingHeight;
				
	size_t			mCounter;
	size_t			mDialogNumber;
	size_t			mMaxHitpoints;
				
	bool			mIsShoot; 
	bool			mCanShoted; 
	bool			mIsJumped;
	bool			mIsLeft; 
	bool			mIsRight;
	bool			mShooted;
	bool			mIsShooting;
	bool			mDoubleJump; 
	bool			mCanDoubleJump; 
	bool			mPressJump;
	bool			mIsRichedEnd;
	bool			mHasStartedFirstBoss;
	bool			mHasStartedSecondBoss;
	bool			mGotKey;
	bool			mActivatedGate;
	bool			mHasTeleported;
				
	State			mState;

	PlayerInfo*		mPlayerInfo;


public:
					Player(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, PlayerInfo* playerInfo);

	void			control(float dt);
	void			checkCollisionWithMap(float Dx, float Dy);	// Функция взаимодейтсвия с картой
	void			update(float dt) override;	// Функция "оживления" класса

};

#endif // PLAYER_HPP
