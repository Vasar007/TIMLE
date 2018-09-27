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
        int                            _counter;

        /**
         * \brief Keeping current teleportation point.
         */
        int                            _numTelPoint;


        /**
         * \brief An additional counter for helping with teleporting this object.
         */
        float                        _appearing;

        /**
         * \brief An additional counter for helping with teleporting this object.
         */
        float                        _disappearing;

        /**
         * \brief The timer for downtime this object.
         */
        float                        _stayTimer;

        /**
         * \brief The timer for delay in place before the next teleport.
         */
        float                        mDelayTimer;

        /**
         * \brief Fixing some graphics issues when object dying.
         */
        float                        _inaccuracy;


        /**
         * \brief Flag for the state "After teleporting".
         */
        bool                        _isTeleported;

        /**
         * \brief Flag for the state "While teleporting".
         */
        bool                        _isTeleporting;

        /**
         * \brief Flag for the state "After dissapering".
         */
        bool                        _isDisappeared;

        /**
         * \brief Flag for the state "While dissapering".
         */
        bool                        _isDisappearing;

        /**
         * \brief Flag, means that this object must change current state on "Staying".
         */
        bool                        _isNeedStay;

        /**
         * \brief Additional flag for the intermediate state before teleporting.
         */
        bool                        _isNeedDelay;


        /**
         * \brief Special vector for keeping all teleport points of this object.
         */
        std::vector<TeleportPoint>    _teleporPoints;


    public:
        /**
         * \brief Flag for the state "Staying".
         */
        bool                        mIsStay;

        /**
         * \brief Flag for the activate delaying timer between teleportations.
         */
        bool                        mIsDelay;

        /**
         * \brief Flag for the activate all tentacles.
         */
        bool                        mIsCalling;

        /**
         * \brief Flag for the deactivate all tentacles.
         */
        bool                        mIsWithdrawing;


    private:
        /**
         * \brief        Appearing this object, that's means the end of teleporting.
         * \param dt    Delta time parameter.
         */
        void                        appear(const float dt);

        /**
         * \brief        Disappearing this object, that's means the start of teleporting.
         * \param dt    Delta time parameter.
         */
        void                        disappear(const float dt);


    public:
        /**
         * \brief            A constructor with basic parameters.
         * \param id        The identifier of the current entity.
         * \param textures    The reference to an associative array with textures.
         * \param fonts        The reference to an associative array with fonts.
         * \param lvl        The reference to the level-class object.
         * \param X            The initial x-coordinate of this object.
         * \param Y            The initial y-coordinate of this object.
         * \param width        Width of this object.
         * \param height    Height if this object.
         * \param type        An additional parameter for interaction with map.
         */
                                    Shadow(const Type::ID id, const TextureHolder& textures, 
                                           const FontHolder& fonts, const Level& lvl, const float X,
                                           const float Y, const int width, const int height,
                                           const std::string& type = "0");


        /**
         * \brief        Overrided funtion for updating this object and its states.
         * \param dt    Delta time parameter.
         */
        void                        update(const float dt) override;
};

#endif // SHADOW_HPP
