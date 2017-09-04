#ifndef SHADOW_HPP
#define SHADOW_HPP

#include "Entity.hpp"


/**
 * \brief Class for main boss in the first level.
 */
class Shadow final : public Entity
{
	private:
		/**
		 * \brief The variable for the counting before we can destroy dead body.
		 */
		int							mCounter;

		/**
		 * \brief Keeping current teleportation point.
		 */
		int							mNumTelPoint;


		/**
		 * \brief An additional counter for helping with teleporting this object.
		 */
		float						mAppearing;

		/**
		 * \brief An additional counter for helping with teleporting this object.
		 */
		float						mDisappearing;

		/**
		 * \brief The timer for downtime this object.
		 */
		float						mStayTimer;

		/**
		 * \brief The timer for delay in place before the next teleport.
		 */
		float						mDelayTimer;

		/**
		 * \brief Fixing some graphics issues when object dying.
		 */
		float						mInaccuracy;


		/**
		 * \brief Flag for the state "After teleporting".
		 */
		bool						mIsTeleported;

		/**
		 * \brief Flag for the state "While teleporting".
		 */
		bool						mIsTeleporting;

		/**
		 * \brief Flag for the state "After dissapering".
		 */
		bool						mIsDisappeared;

		/**
		 * \brief Flag for the state "While dissapering".
		 */
		bool						mIsDisappearing;

		/**
		 * \brief Flag, means that this object must change current state on "Staying".
		 */
		bool						mIsNeedStay;

		/**
		 * \brief Additional flag for the intermediate state before teleporting.
		 */
		bool						mIsNeedDelay;


		/**
		 * \brief Special vector for keeping all teleport points of this object.
		 */
		std::vector<TeleportPoint>	mTeleporPoints;


	public:
		/**
		 * \brief Flag for the state "Staying".
		 */
		bool						mIsStay;

		/**
		 * \brief Flag for the activate delaying timer between teleportations.
		 */
		bool						mIsDelay;

		/**
		 * \brief Flag for the activate all tentacles.
		 */
		bool						mIsCalling;

		/**
		 * \brief Flag for the deactivate all tentacles.
		 */
		bool						mIsWithdrawing;


	private:
		/**
		 * \brief		Helper function for appearing this object, that's means the end of teleporting.
		 * \param dt	Delta time parameter.
		 */
		void						appear(float dt);

		/**
		 * \brief		Helper function for disappearing this object, that's means the start of teleporting.
		 * \param dt	Delta time parameter.
		 */
		void						disappear(float dt);


	public:
		/**
		 * \brief			A constructor with basic parameters.
		 * \param Id		The identifier of the current entity.
		 * \param textures	The reference to an associative array with textures.
		 * \param fonts		The reference to an associative array with fonts.
		 * \param lvl		The reference to the level-class object.
		 * \param X			The initial x-coordinate of this object.
		 * \param Y			The initial y-coordinate of this object.
		 * \param width		Width of this object.
		 * \param height	Height if this object.
		 * \param Type		An additional parameter for interaction with map.
		 */
									Shadow(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");


		/**
		 * \brief		Overrided funtion for updating this object and its states.
		 * \param dt	Delta time parameter.
		 */
		void						update(float dt) override;
};

#endif // SHADOW_HPP
