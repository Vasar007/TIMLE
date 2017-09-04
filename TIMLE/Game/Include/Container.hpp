#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "Component.hpp"

#include <vector>
#include <memory>


namespace GUI
{

class Container : public Component
{
	public:
		typedef std::shared_ptr<Container> Ptr;
			

	private:
		std::vector<Component::Ptr>	mChildren;
		int							mSelectedChild;
	
	
	private:
		virtual void				draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
		bool						hasSelection() const;
		void						select(std::size_t index);
		void						selectNext();
		void						selectPrevious();


	public:
									Container();

		void						pack(Component::Ptr component);

		virtual bool				isSelectable() const;
		virtual void				handleEvent(const sf::Event& event);
};

}

#endif // CONTAINER_HPP
