#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "button.hpp"
#include "resource_holder.hpp"
#include "utility.hpp"

#include "pause_state.hpp"


PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context)
, _playerInfo(*context.mPlayerInfo)
{
    const auto& font = context.mFonts->get(Fonts::ID::Main);
    const sf::Vector2f windowSize(context.mWindow->getSize());

    _sound.setBuffer(context.mSounds->get(Sounds::ID::ButtonCLick));

    _pausedText.setFont(font);
    _pausedText.setString(L"Игра приостановлена");    
    _pausedText.setCharacterSize(70);
    centerOrigin(_pausedText);
    _pausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

    auto returnButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
                                                      *context.mSounds);
    returnButton->setPosition(0.5f * windowSize.x - 100.f, 0.4f * windowSize.y + 75.f);
    returnButton->setText(L"Вернуться");
    returnButton->setCallback([this] ()
    {
        requestStackPop();
    });

    auto backToMenuButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
                                                          *context.mSounds);
    backToMenuButton->setPosition(0.5f * windowSize.x - 100.f, 0.4f * windowSize.y + 125.f);
    backToMenuButton->setText(L"Выйти в меню");
    backToMenuButton->setCallback([this] ()
    {
        _playerInfo.mShowedDialogs.clear();
        _playerInfo.resetData();
        requestStateClear();
        requestStackPush(States::ID::Menu);
    });

    _guiContainer.pack(returnButton);
    _guiContainer.pack(backToMenuButton);
}

void PauseState::draw()
{
    auto& window = *getContext().mWindow;
    window.setView(window.getDefaultView());

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);
    window.draw(_pausedText);
    window.draw(_guiContainer);
}

bool PauseState::update(const sf::Time)
{
    return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Return)
    {
        _sound.play();
    }

    _guiContainer.handleEvent(event);
    return false;
}