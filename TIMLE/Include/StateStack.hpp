#ifndef STATESTACK_HPP
#define STATESTACK_HPP

#include <vector>
#include <functional>
#include <map>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include "State.hpp"
#include "StateIdentifiers.hpp"


namespace sf
{
    class Event;
    class RenderWindow;
}

class StateStack : private sf::NonCopyable
{
    private:
        enum class Action
        {
            Push,
            Pop,
            Clear,
        };

        struct PendingChange
        {
            explicit        PendingChange(Action action, States::ID stateID = States::ID::None);
    
            Action            mAction;
            States::ID        mStateID;
        };
    
    
    private:
        std::vector<State::unPtr>                            _stack;
        std::vector<PendingChange>                            _pendingList;
    
        State::Context                                        _context;
        std::map<States::ID, std::function<State::unPtr()>>    _factories;


    private:
        State::unPtr        createState(const States::ID stateID);
        void                applyPendingChanges();


    public:        
        explicit            StateStack(const State::Context context);
        virtual                ~StateStack() = default;

        StateStack(const StateStack& other) = delete;

        StateStack(StateStack&& other) = delete;

        StateStack& operator=(const StateStack& other) = delete;

        StateStack& operator=(StateStack&& other) = delete;

        template <typename T>
        void                registerState(const States::ID stateID);

        virtual void        update(const sf::Time dt);
        void                draw();
        void                handleEvent(const sf::Event& event);

        void                pushState(const States::ID stateID);
        void                popState();
        void                clearStates();

        bool                isEmpty() const;
};


template <typename T>
void StateStack::registerState(const States::ID stateID)
{
    _factories[stateID] = [this] ()
    {
        return State::unPtr(new T(*this, _context));
    };
}

#endif // STATESTACK_HPP
