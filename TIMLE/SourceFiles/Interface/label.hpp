#pragma once

#include <SFML/Graphics/Text.hpp>

#include "Interface/component.hpp"
#include "Mechanics/resource_identifiers.hpp"
#include "Mechanics/resource_holder.hpp"


/**
 * \brief Additional namespace for working with GUI elements.
 */
namespace GUI
{

/**
 * \brief Standart label class.
 */
class Label final : public Component
{
    public:
        /**
         * \brief Type alias.
         */
        using shPtr = std::shared_ptr<Label>;
            

    private:
        /**
         * \brief Text of the label.
         */
        sf::Text _text;
    
    
    private:
        /**
         * \brief             Draw the object to a render target.
         * \param[out] target Render target to draw to.
         * \param[in] states  Current render states.
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


    public:
        /**
         * \brief           Constuctor.
         * \param[in] text  Text of the label.
         * \param[in] fonts Font holder for extracting necessary font.
         */
             Label(const std::string& text, const FontHolder& fonts);

        /**
         * \brief  Check if label can be selectable.
         * \return True if label is selectable or false otherwise.
         */
        bool isSelectable() const noexcept override;

        /**
         * \brief          Set text to this label.
         * \param[in] text Text of the label.
         */
        void setText(const sf::String& text);

        /**
         * \brief           Handle input events.
         * \param[in] event The event that was sent.
         */
        void handleEvent(const sf::Event& event) noexcept override;
};

}
