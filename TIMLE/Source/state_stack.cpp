#include <cassert>

#include "state_stack.hpp"


StateStack::StateStack(const State::Context context)
: _context(context)
{
}

void StateStack::update(const sf::Time dt)
{
    // Iterate from top to bottom, stop as soon as update() returns false.
    for (auto itr = _stack.rbegin(); itr != _stack.rend(); ++itr)
    {
        if (!(*itr)->update(dt))
        {
            break;
        }
    }

    applyPendingChanges();
}

void StateStack::draw()
{
    // Draw all active states from bottom to top.
    for (auto& state : _stack)
    {
        state->draw();
    }
}

void StateStack::handleEvent(const sf::Event& event)
{
    // Iterate from top to bottom, stop as soon as handleEvent() returns false.
    for (auto itr = _stack.rbegin(); itr != _stack.rend(); ++itr)
    {
        if (!(*itr)->handleEvent(event))
        {
            break;
        }
    }

    applyPendingChanges();
}

void StateStack::pushState(const States::ID stateID)
{
    _pendingList.emplace_back(Action::Push, stateID);
}

void StateStack::popState()
{
    _pendingList.emplace_back(Action::Pop);
}

void StateStack::clearStates()
{
    _pendingList.emplace_back(Action::Clear);
}

bool StateStack::isEmpty() const
{
    return _stack.empty();
}

State::unPtr StateStack::createState(const States::ID stateID)
{
    auto found = _factories.find(stateID);
    assert(found != _factories.end());

    return found->second();
}

void StateStack::applyPendingChanges()
{
    for (const auto& change : _pendingList)
    {
        switch (change.mAction)
        {
            case Action::Push:
                _stack.emplace_back(createState(change.mStateID));
                break;

            case Action::Pop:
                _stack.pop_back();
                break;

            case Action::Clear:
                _stack.clear();
                break;
        }
    }

    _pendingList.clear();
}

StateStack::PendingChange::PendingChange(const Action action, const States::ID stateID)
: mAction(action)
, mStateID(stateID)
{
}
