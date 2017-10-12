#ifndef LIFEBAR_HPP
#define LIFEBAR_HPP

#include "ResourceHolder.hpp"
#include "DataTables.hpp"

#include <SFML/Graphics.hpp>


/**
 * \brief Class of the lifebars for all entities.
 */
class LifeBar
{
	private:
		/**
		 * \brief Maximum value, which equals entity's hitpoints.
		 */
		const int			mMax;
	
		/**
		 * \brief Texture of the lifebar.
		 */
		sf::Texture			mTexture;

		/**
		 * \brief Empty texture of the lifebar, need for correct drawing.
		 */
		sf::Texture			mTextureEmpty;

		/**
		 * \brief Sprite of the lifebar.
		 */
		sf::Sprite			mSprite;

		/**
		 * \brief Empty sprite of the lifebar, need for correct drawing.
		 */
		sf::Sprite			mSpriteEmpty;

		
		/**
		 * \brief Identifier of entity, which helps to create right lifebar.
		 */
		Type::ID			mType;

		/**
		 * \brief Text that is above lifebar.
		 */
		sf::Text			mBossName;
	
	
	public:
	/**
	 * \brief			A constructor with basic parameters.
	 * \param Id		The identifier of the entity.
	 * \param textures	The reference to textures holder for extracting necessary textures.
	 * \param fonts		The reference to fonts holder for extracting necessary textures.
	 * \param hitpoints	Number of the HP entity.
	 */
							LifeBar(Type::ID Id, const TextureHolder& textures, 
									const FontHolder& fonts, int hitpoints);
	
		/**
		 * \brief			Updating this object and its states.
		 * \param points	Current number of the HP entity.
		 */
		void				update(int points);

		/**
		 * \brief			Draws this object on the window.
		 * \param window	Active window to drawing.
		 */
		void				draw(sf::RenderWindow& window);
};

#endif // LIFEBAR_HPP
