#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "../Include/ChoosingState.hpp"
#include "../Include/Button.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"


ChoosingState::ChoosingState(StateStack& stack, Context context)
: State(stack, context)
, _textNumber(0)
, _playerInfo(*context.mPlayerInfo)
{
    // Define some variables for convenience.
    const auto windowSize(context.mWindow->getView().getSize());
    const auto windowCenter(context.mWindow->getView().getCenter());

    _sound.setBuffer(context.mSounds->get(Sounds::ID::ButtonCLick));

    _text.setFont(context.mFonts->get(Fonts::ID::Main));
    _text.setCharacterSize(24);
    _text.setFillColor(sf::Color::White);

    switch(_playerInfo.mChoosingNumber)
    {
        case 1:
        {
            addText(L"Хотите ли вы обыскать тела?");
            auto firstButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
                                                             *context.mSounds);
            firstButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, 
                                     windowCenter.y + windowSize.y / 2.f - 257.f);
            firstButton->setText(L"Да");
            firstButton->setCallback([this]()
            {
                _playerInfo.mChosenSolution[0] = 1;
                requestStackPop();
            });
            _guiContainer.pack(firstButton);

            auto secondButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, 
                                                              *context.mSounds);
            secondButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, 
                                      windowCenter.y + windowSize.y / 2.f - 207.f);
            secondButton->setText(L"Нет");
            secondButton->setCallback([this]()
            {
                _playerInfo.mChosenSolution[0] = 2;
                requestStackPop();
            });
            _guiContainer.pack(secondButton);
            break;
        }
        case 2:
        {
            addText(L"Пока что голем не нападает. Что вы намерены сделать?");

            if (_playerInfo.mChosenSolution[0] == 1)
            {
                auto firstButton = std::make_shared<GUI::Button>(*context.mFonts, 
                                                                 *context.mTextures, 
                                                                 *context.mSounds);
                firstButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, 
                                         windowCenter.y + windowSize.y / 2.f - 307.f);
                firstButton->setText(L"Сказать фразу");
                firstButton->setCallback([this]()
                {
                    _playerInfo.mChosenSolution[1] = 1;
                    requestStackPop();
                });
                _guiContainer.pack(firstButton);

                auto secondButton = std::make_shared<GUI::Button>(*context.mFonts, 
                                                                  *context.mTextures, 
                                                                  *context.mSounds);
                secondButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f,
                                          windowCenter.y + windowSize.y / 2.f - 257.f);
                secondButton->setText(L"Атаковать монстра");
                secondButton->setCallback([this]()
                {
                    _playerInfo.mChosenSolution[1] = 2;
                    requestStackPop();
                });
                _guiContainer.pack(secondButton);

                auto thirdButton = std::make_shared<GUI::Button>(*context.mFonts, 
                                                                 *context.mTextures, 
                                                                 *context.mSounds);
                thirdButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, 
                                         windowCenter.y + windowSize.y / 2.f - 207.f);
                thirdButton->setText(L"Подождать");
                thirdButton->setCallback([this]()
                {
                    _playerInfo.mChosenSolution[1] = 3;
                    requestStackPop();
                });
                _guiContainer.pack(thirdButton);
            }
            else if (_playerInfo.mChosenSolution[0] == 2)
            {
                auto firstButton = std::make_shared<GUI::Button>(*context.mFonts,
                                                                 *context.mTextures, 
                                                                 *context.mSounds);
                firstButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, 
                    windowCenter.y + windowSize.y / 2.f - 257.f);
                firstButton->setText(L"Атаковать монстра");
                firstButton->setCallback([this]()
                {
                    _playerInfo.mChosenSolution[1] = 2;
                    requestStackPop();
                });
                _guiContainer.pack(firstButton);

                auto secondButton = std::make_shared<GUI::Button>(*context.mFonts, 
                                                                  *context.mTextures,
                                                                  *context.mSounds);
                secondButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f,
                                          windowCenter.y + windowSize.y / 2.f - 207.f);
                secondButton->setText(L"Подождать");
                secondButton->setCallback([this]()
                {
                    _playerInfo.mChosenSolution[1] = 3;
                    requestStackPop();
                });
                _guiContainer.pack(secondButton);
            }
            break;
        }
        default:
            addText("Invalid choosing type.");
            break;
    }

    setText(_textNumber);
}

void ChoosingState::addText(const sf::String& text)
{
    _dialogText.push_back(text);
}

void ChoosingState::setText(const std::size_t number)
{
    auto& text = _dialogText[number];
    /*
     for (std::size_t i = 0; i < text.getSize(); ++i)
    {
        if (i % static_cast<int>(getContext().window.getView().getSize().x /
            (mText.getCharacterSize() - 5.f)) == 0 && i > 0)
        {
            text.insert(i, "\n");
        }
    }
    */
    _text.setString(text);
}

void ChoosingState::draw()
{
    auto& window = *getContext().mWindow;
    window.setView(window.getDefaultView());

    const auto center = window.getView().getCenter();
    //auto size = window.getView().getSize();

    _text.setPosition(center.x - 200.f, center.y - 0.f);

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);

    window.draw(_text);
    window.draw(_guiContainer);
}

bool ChoosingState::update(const sf::Time)
{
    return false;
}

bool ChoosingState::handleEvent(const sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
    {
        _sound.play();
    }

    _guiContainer.handleEvent(event);
    return false;
}
