#ifndef SHADOW_HPP
#define SHADOW_HPP

#include "Entity.hpp"


class Shadow : public Entity 
{
	public:
		int							mCounter;
		int							numTelPoint;

		float						mAppearing;
		float						mDisappearing;
		float						mStayTimer;
		float						mDelayTimer;

		float						mInaccuracy;

		bool						mIsTeleported;
		bool						mIsTeleporting;
		bool						mIsDisappeared;
		bool						mIsDisappearing;
		bool						mIsStay;
		bool						mIsNeedStay;
		bool						mIsDelay;
		bool						mIsNeedDelay;
		bool						mIsCalling;
		bool						mIsWithdrawing;

		std::vector<TeleportPoint>	mTeleporPoints;


	public:
									Shadow(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");

		void						appear(float time);
		void						disappear(float time);
		virtual void				update(float time);
};

#endif // SHADOW_HPP
