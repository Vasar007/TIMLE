#include "Effects/bloodsplat.hpp"


Bloodsplat::Bloodsplat(const Type::ID id, const sf::Texture& texture, const float X,
                       const float Y, const int width, const int height, const std::string& type)
: Effect(id, X, Y, width, height, type)
{
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 48, 24));
    const auto color = sf::Color(sprite.getColor().r, sprite.getColor().g,
                                 sprite.getColor().b, 0);
    sprite.setColor(color);

    started = true;
    finished = false;
}

void Bloodsplat::update(const float dt)
{
    if (started)
    {
        move_timer += 0.0085f * dt;

        const auto color = sprite.getColor();

        if (move_timer > 5.f)
        {
            started = false;
            move_timer = -5.f;
        }
        else
        {
            if (color.a + static_cast<sf::Uint8>(move_timer) * 5 > 255)
            {
                sprite.setColor(sf::Color(color.r, color.g, color.b, 255));
            }
            else
            {
                sprite.setColor(sf::Color(color.r, color.g, color.b,
                                 color.a + static_cast<sf::Uint8>(move_timer) * 5));
            }
        }
    }

    current_frame += 0.0075f * dt;
    if (current_frame > 13.f)
    {
        current_frame -= 13.f;
        finished = true;
    }

    sprite.setTextureRect(sf::IntRect(48 * static_cast<int>(current_frame), 0, 48, 24));


    sprite.setPosition(x + (width / 2.f) - 10.f, y + (height / 2.f) - 5.f);

    if (finished)
    {
        current_death_frame += 0.0075f * dt;

        const auto color = sprite.getColor();

        if (current_death_frame > 5.f)
        {
            alive = false;
            current_death_frame = -5.f;
        }
        else
        {
            if (color.a - static_cast<sf::Uint8>(current_death_frame) * 5 < 0)
            {
                sprite.setColor(sf::Color(color.r, color.g, color.b, 0));
            }
            else
            {
                sprite.setColor(sf::Color(color.r, color.g, color.b,
                                           color.a - static_cast<sf::Uint8>(current_death_frame) * 5));
            }
        }
    }
}
