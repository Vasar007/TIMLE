#ifndef BLOODSPLAT_HPP
#define BLOODSPLAT_HPP

#include <SFML/Graphics/Texture.hpp>

#include "effect.hpp"


/**
 * \brief Blodsplat effect which created when someone dyeing.
 */
class Bloodsplat final : public Effect
{
    public:
        /**
         * \brief             Default constructor.
         * \param[in] id      The identifier of the current effect.
         * \param[in] texture The reference to texture from TextureHolder.
         * \param[in] X       The initial x-coordinate of this object.
         * \param[in] Y       The initial y-coordinate of this object.
         * \param[in] width   Width of this object.
         * \param[in] height  Height if this object.
         * \param[in] type    An additional parameter for interaction with map.
         */
             Bloodsplat(const Type::ID id, const sf::Texture& texture, const float X,
                        const float Y, const int width, const int height,
                        const std::string& type = "0");

        /**
         * \brief        Overriden virtual funtion for updating this object and its states.
         * \param[in] dt Delta time parameter.
         */
        void update(const float dt) override;
};

#endif // BLOODSPLAT_HPP
