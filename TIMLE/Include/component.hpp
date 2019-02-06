#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>


// Forward declaration.

namespace sf
{
    class Event;
}

/**
 * \brief Additional namespace for working with GUI elements.
 */
namespace GUI
{

/**
 * \brief Original and abstract class for all GUI elements.
 */
class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
    public:
        /**
         * \brief Syntactic sugar.
         */
        using ShPtr = std::shared_ptr<Component> ;


    private:
        /**
         * \brief Selection flag of the component.
         */
        bool mIsSelected;

        /**
         * \brief Active flag of the component.
         */
        bool mIsActive;


    public:
        /**
         * \brief Default constructor.
         */
                     Component();

        /**
         * \brief Default destructor.
         */
        virtual      ~Component() = default;

                     Component(const Component& other) = delete;

                     Component(Component&& other) = delete;

        Component&   operator=(const Component& other) = delete;

        Component&   operator=(Component&& other) = delete;


        /**
         * \brief  Check if component can be selectable.
         * \return True if component is selectable or false otherwise.
         */
        virtual bool isSelectable() const = 0;

        /**
         * \brief  Check if component is already selected.
         * \return True if this component is selected or not otherwise.
         */
        bool         isSelected() const;

        /**
         * \brief Change focus on the other component.
         */
        virtual void select();

        /**
         * \brief Remove select flag from this component.
         */
        virtual void deselect();

        /**
         * \brief  Check if this component is active.
         * \return True if this component is active or false otherwise.
         */
        virtual bool isActive() const;

        /**
         * \brief Toggle active flag on true.
         */
        virtual void activate();

        /**
         * \brief Toggle active flag on false.
         */
        virtual void deactivate();

        /**
         * \brief           Handle input events.
         * \param[in] event The event that was sent.
         */
        virtual void handleEvent(const sf::Event& event) = 0;
};

}

#endif // COMPONENT_HPP
