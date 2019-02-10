#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "States/parallel_task.hpp"
#include "States/state.hpp"


class LoadingState final : public State
{
    private:
        sf::Text           _loadingText;
        sf::RectangleShape _progressBarBackground;
        sf::RectangleShape _progressBar;
    
        ParallelTask       _loadingTask;


    public:
             LoadingState(StateStack& stack, Context context);
    
        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) noexcept override;
    
        void setCompletion(float percent);
};
