#include <utility>

#include "Effects/effect.hpp"


Effect::Effect(const Type::ID id, const float X, const float Y, const int width, const int height,
               std::string type)
: x(X)
, y(Y)
, width(width)
, height(height)
, current_frame(0.f)
, current_attack_frame(0.f)
, current_death_frame(0.f)
, move_timer(0.f)
, alive(true)
, started(false)
, finished(false)
, attacked(false)
, hit(false)
, hit_once(false)
, type(std::move(type))
, type_ID(id)
{
    sprite.setOrigin(static_cast<float>(width) / 2.f, static_cast<float>(height) / 2.f);
}

sf::FloatRect Effect::get_rect() const
{
    return sf::FloatRect(x, y, static_cast<float>(width), static_cast<float>(height));
}

sf::Vector2f Effect::get_center() const
{
    return sf::Vector2f(x + static_cast<float>(width) / 2.f,
                        y + static_cast<float>(height) / 2.f);
}

sf::Vector2f Effect::get_world_position() const
{
    return get_world_transform() * sf::Vector2f();
}

sf::Transform Effect::get_world_transform() const
{
    sf::Transform transform = sf::Transform::Identity;

    transform *= sprite.getTransform();

    return transform;
}

void Effect::draw(sf::RenderTarget& target) const
{
    target.draw(sprite);
}
