#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "resource_holder.hpp"
#include "utility.hpp"

#include "loading_state.hpp"


LoadingState::LoadingState(StateStack& stack, Context context)
: State(stack, context)
, _loadingTask(*context.mTextures, *context.mSounds, *context.mPlayerInfo)
{
    auto& window = *getContext().mWindow;
    const auto& font = context.mFonts->get(Fonts::ID::Main);
    const auto viewSize = window.getView().getSize();

    _loadingText.setFont(font);
    _loadingText.setString(L"Загрузка...");
    centerOrigin(_loadingText);
    _loadingText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f + 50.f);

    _progressBarBackground.setFillColor(sf::Color::White);
    _progressBarBackground.setSize(sf::Vector2f(viewSize.x - 20.f, 10.f));
    _progressBarBackground.setPosition(10.f, _loadingText.getPosition().y + 40.f);

    _progressBar.setFillColor(sf::Color(100,100,100));
    _progressBar.setSize(sf::Vector2f(200.f, 10.f));
    _progressBar.setPosition(10.f, _loadingText.getPosition().y + 40.f);

    setCompletion(0.f);

    _loadingTask.execute();
}

void LoadingState::draw()
{
    auto& window = *getContext().mWindow;

    window.setView(window.getDefaultView());

    window.draw(_loadingText);
    window.draw(_progressBarBackground);
    window.draw(_progressBar);
}

bool LoadingState::update(sf::Time)
{
    // Update the progress bar from the remote task or finish it.
    if (_loadingTask.isFinished())
    {
        requestStackPop();
        requestStackPush(States::ID::Game);
    }
    else
    {
        setCompletion(_loadingTask.getCompletion());
    }
    return true;
}

bool LoadingState::handleEvent(const sf::Event&)
{
    return true;
}

void LoadingState::setCompletion(float percent)
{
    // Clamp.
    if (percent > 1.f)
    {
        percent = 1.f;
    }

    _progressBar.setSize(sf::Vector2f(_progressBarBackground.getSize().x * percent,
                                      _progressBar.getSize().y));
}
