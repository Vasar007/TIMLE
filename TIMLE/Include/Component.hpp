#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>


// Forward declaration.
namespace sf
{
	class Event;
}

/**
 * \brief Additional namespace for convenience.
 */
namespace GUI
{

/**
 * \brief Original and abstract class for all GUI elements.
 */
class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
	public:
		typedef std::shared_ptr<Component> Ptr;


	private:
		bool			mIsSelected;
		bool			mIsActive;


	public:
						Component();
		virtual			~Component();

		virtual bool	isSelectable() const = 0;
		bool			isSelected() const;
		virtual void	select();
		virtual void	deselect();

		virtual bool	isActive() const;
		virtual void	activate();
		virtual void	deactivate();

		virtual void	handleEvent(const sf::Event& event) = 0;
};

}

#endif // COMPONENT_HPP
