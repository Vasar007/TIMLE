#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "Component.hpp"

#include <vector>
#include <memory>


/**
 * \brief Additional namespace for convenience.
 */
namespace GUI
{

/**
 * \brief Element, which can contain other GUI elements.
 */
class Container : public Component
{
	public:
		typedef std::shared_ptr<Container> Ptr;
			

	private:
		std::vector<Component::Ptr>	mChildren;
		int							mSelectedChild;
	
	
	private:
		void						draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
		bool						hasSelection() const;
		void						select(std::size_t index);
		void						selectNext();
		void						selectPrevious();


	public:
									Container();

		void						pack(Component::Ptr component);

		bool						isSelectable() const override;
		void						handleEvent(const sf::Event& event) override;
};

}

#endif // CONTAINER_HPP
