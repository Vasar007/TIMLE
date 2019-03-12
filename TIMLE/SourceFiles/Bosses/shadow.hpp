#pragma once

#include "Interface/level.hpp"
#include "Mechanics/data_tables.hpp"
#include "Mechanics/resource_holder.hpp"
#include "Objects/entity.hpp"


/**
 * \brief Class for main boss in the first level.
 */
class Shadow final : public Entity
{
    private:
        /**
         * \brief The variable for the counting before we can destroy dead body.
         */
        int                        _counter;

        /**
         * \brief Keeping current teleportation point.
         */
        int                        _teleportation_point_number;

        /**
         * \brief An additional counter for helping with teleporting this object.
         */
        float                      _appearence_timer;

        /**
         * \brief An additional counter for helping with teleporting this object.
         */
        float                      _disappearance_timer;

        /**
         * \brief The timer for downtime this object.
         */
        float                      _stay_timer;

        /**
         * \brief The timer for delay in place before the next teleport.
         */
        float                      _delay_timer;

        /**
         * \brief Fixing some graphics issues when object dying.
         */
        float                      _inaccuracy;

        /**
         * \brief Flag for the state "After teleporting".
         */
        bool                       _teleported;

        /**
         * \brief Flag for the state "While teleporting".
         */
        bool                       _teleporting;

        /**
         * \brief Flag for the state "After disappearing".
         */
        bool                       _is_isappeared;

        /**
         * \brief Flag for the state "While disappearing".
         */
        bool                       _disappearing;

        /**
         * \brief Flag, means that this object must change current state on "Idle".
         */
        bool                       _needs_idle;

        /**
         * \brief Additional flag for the intermediate state before teleporting.
         */
        bool                       _needs_delay;

        /**
         * \brief Special vector for keeping all teleport points of this object.
         */
        std::vector<TeleportPoint> _teleport_points;


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
                    const level& level, const float x_coord, const float y_coord, const int width,
                    const int height, const std::string& type = "0");


        /**
         * \brief        Overridden function for updating this object and its states.
         * \param[in] dt Delta time parameter.
         */
        void update(const float dt) override;
};
