#include "Interface/component.hpp"


namespace GUI
{

Component::Component() noexcept
: mIsSelected(false)
, mIsActive(false)
{
}

bool Component::isSelected() const noexcept
{
    return mIsSelected;
}

void Component::select()
{
    mIsSelected = true;
}

void Component::deselect()
{
    mIsSelected = false;
}

bool Component::isActive() const noexcept
{
    return mIsActive;
}

void Component::activate()
{
    mIsActive = true;
}

void Component::deactivate()
{
    mIsActive = false;
}

}
