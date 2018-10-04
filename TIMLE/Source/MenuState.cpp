#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "../Include/MenuState.hpp"
#include "../Include/Button.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"


MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
{
    // Define some variables for convenience.
    const auto windowSize(context.mWindow->getView().getSize());
    const auto windowCenter(context.mWindow->getView().getCenter());

    if (mAudioManager.isPlaying())
    {
        mAudioManager.stopAllMusics();
    }
    mAudioManager.setMusic(Music::ID::MainMenuTheme);
    
    _backgroundSprite.setTexture(context.mTextures->get(Textures::ID::TitleScreen));

    _sound.setBuffer(context.mSounds->get(Sounds::ID::ButtonCLick));

    // Create the buttons for main menu.
    auto playButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
                                                    *context.mSounds);
    playButton->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, 
                            windowCenter.y - (windowSize.y / 2.f) + 300.f);
    playButton->setText(L"Играть");
    playButton->setCallback([this] ()
    {
        if (mAudioManager.isPlaying())
        {
            mAudioManager.stopAllMusics();
        }
        requestStackPop();
        requestStackPush(States::ID::Loading);
    });

    auto settingsButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
                                                        *context.mSounds);
    settingsButton->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, 
                                windowCenter.y - (windowSize.y / 2.f) + 350.f);
    settingsButton->setText(L"Настройки");
    settingsButton->setCallback([this] ()
    {
        requestStackPush(States::ID::Settings);
    });

    auto titreButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, 
                                                     *context.mSounds);
    titreButton->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, 
                             windowCenter.y - (windowSize.y / 2.f) + 400.f);
    titreButton->setText(L"Титры");
    titreButton->setCallback([this]()
    {
        requestStackPop();
        requestStackPush(States::ID::Titre);
    });

    auto exitButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, 
                                                    *context.mSounds);
    exitButton->setPosition(windowCenter.x - (windowSize.x / 2.f) + 100.f, 
                            windowCenter.y - (windowSize.y / 2.f) + 450.f);
    exitButton->setText(L"Выйти");
    exitButton->setCallback([this] ()
    {
        requestStackPop();
    });

    _guiContainer.pack(playButton);
    _guiContainer.pack(settingsButton);
    _guiContainer.pack(titreButton);
    _guiContainer.pack(exitButton);
}

void MenuState::draw()
{
    auto& window = *getContext().mWindow;

    window.setView(window.getDefaultView());

    window.draw(_backgroundSprite);
    window.draw(_guiContainer);
}

bool MenuState::update(const sf::Time)
{
    return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Return)
    {
        _sound.play();
    }
    
    _guiContainer.handleEvent(event);
    return false;
}
