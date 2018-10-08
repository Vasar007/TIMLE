#ifndef SHADOW_HPP
#define SHADOW_HPP

#include "entity.hpp"


/**
 * \brief Class for main boss in the first level.
 */
class Shadow final : public Entity
{
    private:
        /**
         * \brief The variable for the counting before we can destroy dead body.
         */
        int                        counter_;

        /**
         * \brief Keeping current teleportation point.
         */
        int                        teleportation_point_number_;

        /**
         * \brief An additional counter for helping with teleporting this object.
         */
        float                      appearence_timer_;

        /**
         * \brief An additional counter for helping with teleporting this object.
         */
        float                      disappearance_timer_;

        /**
         * \brief The timer for downtime this object.
         */
        float                      stay_timer_;

        /**
         * \brief The timer for delay in place before the next teleport.
         */
        float                      delay_timer_;

        /**
         * \brief Fixing some graphics issues when object dying.
         */
        float                      inaccuracy_;

        /**
         * \brief Flag for the state "After teleporting".
         */
        bool                       teleported_;

        /**
         * \brief Flag for the state "While teleporting".
         */
        bool                       teleporting_;

        /**
         * \brief Flag for the state "After dissapering".
         */
        bool                       isDisappeared_;

        /**
         * \brief Flag for the state "While dissapering".
         */
        bool                       disappearing_;

        /**
         * \brief Flag, means that this object must change current state on "Idle".
         */
        bool                       needs_idle_;

        /**
         * \brief Additional flag for the intermediate state before teleporting.
         */
        bool                       needs_delay_;

        /**
         * \brief Special vector for keeping all teleport points of this object.
         */
        std::vector<TeleportPoint> telepor_points_;


    public:
        /**
         * \brief Flag for the state "Idle".
         */
        bool idling;

        /**
         * \brief Flag for the activate delaying timer between teleportations.
         */
        bool is_delay;

        /**
         * \brief Flag for the activate all tentacles.
         */
        bool summoning;

        /**
         * \brief Flag for the deactivate all tentacles.
         */
        bool calling_of;


    private:
        /**
         * \brief        Appearing this object, that's means the end of teleporting.
         * \param[in] dt Delta time parameter.
         */
        void appear(const float dt);

        /**
         * \brief        Disappearing this object, that's means the start of teleporting.
         * \param[in] dt Delta time parameter.
         */
        void disappear(const float dt);


    public:
        /**
         * \brief              A constructor with basic parameters.
         * \param[in] id       The identifier of the current entity.
         * \param[in] textures The reference to an associative array with textures.
         * \param[in] fonts    The reference to an associative array with fonts.
         * \param[in] level    The reference to the level-class object.
         * \param[in] x_coord  The initial x-coordinate of this object.
         * \param[in] y_coord  The initial y-coordinate of this object.
         * \param[in] width    Width of this object.
         * \param[in] height   Height if this object.
         * \param[in] type     An additional parameter for interaction with map.
         */
             Shadow(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
                    const Level& level, const float x_coord, const float y_coord, const int width,
                    const int height, const std::string& type = "0");


        /**
         * \brief        Overrided funtion for updating this object and its states.
         * \param[in] dt Delta time parameter.
         */
        void update(const float dt) override;
};

#endif // SHADOW_HPP
