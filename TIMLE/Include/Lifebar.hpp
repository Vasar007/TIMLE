#ifndef LIFEBAR_HPP
#define LIFEBAR_HPP

#include <SFML/Graphics.hpp>

#include "ResourceHolder.hpp"
#include "DataTables.hpp"


/**
 * \brief Class of the lifebars for all entities.
 */
class LifeBar
{
    private:
        /**
         * \brief Maximum value, which equals entity's hitpoints.
         */
        const int            _max;
    
        /**
         * \brief Texture of the lifebar.
         */
        sf::Texture            _texture;

        /**
         * \brief Empty texture of the lifebar, need for correct drawing.
         */
        sf::Texture            _textureEmpty;

        /**
         * \brief Sprite of the lifebar.
         */
        sf::Sprite            _sprite;

        /**
         * \brief Empty sprite of the lifebar, need for correct drawing.
         */
        sf::Sprite            _spriteEmpty;

        
        /**
         * \brief Identifier of entity, which helps to create right lifebar.
         */
        Type::ID            _type;

        /**
         * \brief Text that is above lifebar.
         */
        sf::Text            _bossName;
    
    
    public:
    /**
     * \brief            A constructor with basic parameters.
     * \param id        The identifier of the entity.
     * \param textures    The reference to textures holder for extracting necessary textures.
     * \param fonts        The reference to fonts holder for extracting necessary textures.
     * \param hitpoints    Number of the HP entity.
     */
                            LifeBar(Type::ID id, const TextureHolder& textures, 
                                    const FontHolder& fonts, const int hitpoints);
    
        /**
         * \brief            Updating this object and its states.
         * \param points    Current number of the HP entity.
         */
        void                update(int points);

        /**
         * \brief            Draws this object on the window.
         * \param window    Active window to drawing.
         */
        void                draw(sf::RenderWindow& window);
};

#endif // LIFEBAR_HPP
