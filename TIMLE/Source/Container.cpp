#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include "container.hpp"


namespace GUI
{

Container::Container()
: _selectedChild(-1)
{
}

void Container::pack(const Component::ShPtr& component)
{
    _children.push_back(component);

    if (!hasSelection() && component->isSelectable())
    {
        select(_children.size() - 1);
    }
}

bool Container::isSelectable() const
{
    return false;
}

void Container::handleEvent(const sf::Event& event)
{
    // If we have selected a child then give it events.
    if (hasSelection() && _children[_selectedChild]->isActive())
    {
        _children[_selectedChild]->handleEvent(event);
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
        {
            selectPrevious();
        }
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
        {
            selectNext();
        }
        else if (event.key.code == sf::Keyboard::Return)
        {
            if (hasSelection())
            {
                _children[_selectedChild]->activate();
            }
        }
    }
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (const auto& child : _children)
    {
        target.draw(*child, states);
    }
}

bool Container::hasSelection() const
{
    return _selectedChild >= 0;
}

void Container::select(const std::size_t index)
{
    if (_children[index]->isSelectable())
    {
        if (hasSelection())
        {
            _children[_selectedChild]->deselect();
        }

        _children[index]->select();
        _selectedChild = index;
    }
}

void Container::selectNext()
{
    if (!hasSelection())
    {
        return;
    }

    // Search next component that is selectable, wrap around if necessary.
    int next = _selectedChild;
    do
    {
        next = (next + 1) % _children.size();
    }
    while (!_children[next]->isSelectable());

    // Select that component.
    select(next);
}

void Container::selectPrevious()
{
    if (!hasSelection())
    {
        return;
    }

    // Search previous component that is selectable, wrap around if necessary.
    int prev = _selectedChild;
    do
    {
        prev = (prev + _children.size() - 1) % _children.size();
    }
    while (!_children[prev]->isSelectable());

    // Select that component.
    select(prev);
}

}
