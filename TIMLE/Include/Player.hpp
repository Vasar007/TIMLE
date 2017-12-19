#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "DataTables.hpp"
#include "PlayerInfo.hpp"


class Player final : public Entity 
{
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


	private:
		float			_beforeJump;
		float			_afterJump;
		float			_fallingHeight;

		bool			_isLeft;
		bool			_isRight;
		bool			_needFirstMainDelay;
		bool			_hadFirstMainDelay;
		bool			_needFirstMiniDelay;
		bool			_hadFirstMiniDelay;
	
	
	public:
		float			mStayTimer;
		float			mOnPlatform;
		float			mShootTimer;
		float			mJumpTimer;
		float			mDelayTimer;
		
		std::size_t		mCounter;
		std::size_t		mDialogNumber;
		std::size_t		mMaxHitpoints;
					
		bool			mIsShoot; 
		bool			mCanShoted; 
		bool			mIsJumped;

		bool			mShooted;
		bool			mIsShooting;
		bool			mDoubleJump; 
		bool			mCanDoubleJump; 
		bool			mPressJump;
		bool			mIsRichedEnd;
		bool			mHasStartedFirstMainBoss;
		bool			mHasStartedFirstMiniBoss;
		bool			mGotKey;
		bool			mActivatedGate;
		bool			mHasTeleported;
		bool			mIsStartedDelay;
					
		State			mState;
	
		PlayerInfo*		mPlayerInfo;
	

	private:
		void			control(const float dt);

		void			doDelay(const float dt, bool& flag, bool& checker);
	

	public:
						Player(const Type::ID id, const TextureHolder& textures, 
							   const FontHolder& fonts, const Level& lvl, const float X, 
							   const float Y, const int width, const int height, 
							   PlayerInfo* playerInfo);
	
		void			checkCollisionWithMap(const float Dx, const float Dy);	// Функция взаимодейтсвия с картой
		
		void			update(const float dt) override;	// Функция "оживления" класса
};

#endif // PLAYER_HPP
