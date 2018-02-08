#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "../Include/ChangingState.hpp"
#include "../Include/Button.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"


ChanginState::ChanginState(StateStack& stack, Context context)
: State(stack, context)
, _textNumber(0)
, _playerInfo(*context.mPlayerInfo)
, _currentSettings(context.mCurrentSettings)
{
	/*
		Get info about display resolution.
		std::cout << sf::VideoMode::getDesktopMode().width << " " << 
			sf::VideoMode::getDesktopMode().height << std::endl;
		std::cout << "---------------\n";
		std::vector<sf::VideoMode> sample = sf::VideoMode::getFullscreenModes();
		for (std::size_t i = 0; i < sample.size(); ++i)
			std::cout << sample[i].width << " " << sample[i].height << std::endl;
		
		All available screen sizes:
		///1920 1080
		1680 1050
		///1600 900
		1440 900
		1400 1050
		///1366 768
		1360 768
		///1280 1024
		1280 960
		1280 800
		1280 768
		///1280 720
		1280 600
		1152 864
		//1024 768
		///800 600
		640 480
		640 400
		512 384
		400 300
		320 240
		320 200
	*/

	// Define some variables for convenience.
	const auto windowSize(context.mWindow->getView().getSize());
	const auto windowCenter(context.mWindow->getView().getCenter());

	_sound.setBuffer(context.mSounds->get(Sounds::ID::ButtonCLick));

	_text.setFont(context.mFonts->get(Fonts::ID::Main));
	_text.setCharacterSize(24);
	_text.setFillColor(sf::Color::White);

	// Create a GUI with dependencies from pressed button.
	switch(_currentSettings->mPressedButton)
	{
		// Pressed button "Change Window Size".
		case 0:
		{
			addText(L"Выберите размер окна:");

			auto fullscreenModes = sf::VideoMode::getFullscreenModes();

			std::size_t counter = 0;

			for (const auto& fullscreenMode : fullscreenModes)
			{
				if (fullscreenMode.width == 1920 && fullscreenMode.height == 1080)
				{
					++counter;

					auto selectButton = std::make_shared<GUI::Button>(*context.mFonts,
																	  *context.mTextures,
																	  *context.mSounds);
					selectButton->setPosition(windowCenter.x - 100.f, 
											  windowCenter.y - (windowSize.y / 2.f) + 100.f +
												50.f * counter);
					selectButton->setText("1920 x 1080");
					selectButton->setCallback([this, &fullscreenMode]()
					{
						if (_currentSettings->mWindowSize.x != fullscreenMode.width &&
							_currentSettings->mWindowSize.y != fullscreenMode.height)
						{
							_currentSettings->mHasAnyChanges = true;
							_currentSettings->mWindowSize = sf::Vector2u(1920, 1080);
						}
						requestStackPop();
					});
					_guiContainer.pack(selectButton);
				}
				if (fullscreenMode.width == 1600 && fullscreenMode.height == 900)
				{
					++counter;

					auto selectButton = std::make_shared<GUI::Button>(*context.mFonts,
																	  *context.mTextures,
																	  *context.mSounds);
					selectButton->setPosition(windowCenter.x - 100.f, 
											  windowCenter.y - (windowSize.y / 2.f) + 100.f +
												50.f * counter);
					selectButton->setText("1600 x 900");
					selectButton->setCallback([this, &fullscreenMode]()
					{
						if (_currentSettings->mWindowSize.x != fullscreenMode.width &&
							_currentSettings->mWindowSize.y != fullscreenMode.height)
						{
							_currentSettings->mHasAnyChanges = true;
							_currentSettings->mWindowSize = sf::Vector2u(1600, 900);
						}
						requestStackPop();
					});
					_guiContainer.pack(selectButton);
				}
				if (fullscreenMode.width == 1366 && fullscreenMode.height == 768)
				{
					++counter;

					auto selectButton = std::make_shared<GUI::Button>(*context.mFonts,
																	  *context.mTextures, 
																	  *context.mSounds);
					selectButton->setPosition(windowCenter.x - 100.f, 
											  windowCenter.y - (windowSize.y / 2.f) + 100.f + 
												50.f * counter);
					selectButton->setText("1366 x 768");
					selectButton->setCallback([this, &fullscreenMode]()
					{
						if (_currentSettings->mWindowSize.x != fullscreenMode.width &&
							_currentSettings->mWindowSize.y != fullscreenMode.height)
						{
							_currentSettings->mHasAnyChanges = true;
							_currentSettings->mWindowSize = sf::Vector2u(1366, 768);
						}
						requestStackPop();
					});
					_guiContainer.pack(selectButton);
				}
				if (fullscreenMode.width == 1280 && fullscreenMode.height == 1024)
				{
					++counter;

					auto selectButton = std::make_shared<GUI::Button>(*context.mFonts, 
																	  *context.mTextures, 
																	  *context.mSounds);
					selectButton->setPosition(windowCenter.x - 100.f, 
											  windowCenter.y - (windowSize.y / 2.f) + 100.f + 
												50.f * counter);
					selectButton->setText("1280 x 1024");
					selectButton->setCallback([this, &fullscreenMode]()
					{
						if (((_currentSettings->mWindowSize.x != fullscreenMode.width) && 
							(_currentSettings->mWindowSize.y != fullscreenMode.height)) ||
							((_currentSettings->mWindowSize.x == 1280) &&
								(_currentSettings->mWindowSize.y == 720)))
						{
							_currentSettings->mHasAnyChanges = true;
							_currentSettings->mWindowSize = sf::Vector2u(1280, 1024);
						}
						requestStackPop();
					});
					_guiContainer.pack(selectButton);
				}
				if (fullscreenMode.width == 1280 && fullscreenMode.height == 720)
				{
					++counter;

					auto selectButton = std::make_shared<GUI::Button>(*context.mFonts,
																	  *context.mTextures,
																	  *context.mSounds);
					selectButton->setPosition(windowCenter.x - 100.f, 
											  windowCenter.y - (windowSize.y / 2.f) + 100.f + 
												50.f * counter);
					selectButton->setText("1280 x 720");
					selectButton->setCallback([this, &fullscreenMode]()
					{
						if (((_currentSettings->mWindowSize.x != fullscreenMode.width) &&
							(_currentSettings->mWindowSize.y != fullscreenMode.height)) ||
							((_currentSettings->mWindowSize.x == 1280) &&
								(_currentSettings->mWindowSize.y == 1024)))
						{
							_currentSettings->mHasAnyChanges = true;
							_currentSettings->mWindowSize = sf::Vector2u(1280, 720);
						}
						requestStackPop();
					});
					_guiContainer.pack(selectButton);
				}
				if (fullscreenMode.width == 1024 && fullscreenMode.height == 768)
				{
					++counter;

					auto selectButton = std::make_shared<GUI::Button>(*context.mFonts,
																	  *context.mTextures,
																	  *context.mSounds);
					selectButton->setPosition(windowCenter.x - 100.f, 
											  windowCenter.y - (windowSize.y / 2.f) + 100.f + 
												50.f * counter);
					selectButton->setText("1024 x 768");
					selectButton->setCallback([this, &fullscreenMode]()
					{
						if (_currentSettings->mWindowSize.x != fullscreenMode.width &&
							_currentSettings->mWindowSize.y != fullscreenMode.height)
						{
							_currentSettings->mHasAnyChanges = true;
							_currentSettings->mWindowSize = sf::Vector2u(1024, 768);
						}
						requestStackPop();
					});
					_guiContainer.pack(selectButton);
				}
			}
			break;
		}
		// Pressed button "Change Window Style".
		case 1:
		{
			addText(L"Выберите режим окна:");

			auto selectButton1 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, 
															   *context.mSounds);
			selectButton1->setPosition(windowCenter.x - 100.f, 
									   windowCenter.y - (windowSize.y / 2.f) + 100.f + 50.f * 1.f);
			selectButton1->setText(L"На полный экран");
			selectButton1->setCallback([this]()
			{
				if (_currentSettings->mWindowStyle != State::WindowStyle::Fullscreen)
				{
					_currentSettings->mHasAnyChanges = true;
					_currentSettings->mWindowStyle = State::WindowStyle::Fullscreen;
				}
				requestStackPop();
			});
			_guiContainer.pack(selectButton1);

			auto selectButton2 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, 
															   *context.mSounds);
			selectButton2->setPosition(windowCenter.x - 100.f, 
									   windowCenter.y - (windowSize.y / 2.f) + 100.f + 50.f * 2.f);
			selectButton2->setText(L"Оконный");
			selectButton2->setCallback([this]()
			{
				if (_currentSettings->mWindowStyle != State::WindowStyle::Close)
				{
					_currentSettings->mHasAnyChanges = true;
					_currentSettings->mWindowStyle = State::WindowStyle::Close;
				}
				requestStackPop();
			});
			_guiContainer.pack(selectButton2);
			break;
		}
		// Pressed button "Change Music Volume".
		case 2:
		{
			addText(L"Выберите громкость музыки:");

			auto selectButton100 = std::make_shared<GUI::Button>(*context.mFonts, 
																 *context.mTextures, 
																 *context.mSounds);
			selectButton100->setPosition(windowCenter.x - 100.f, 
										 windowCenter.y - (windowSize.y / 2.f) + 100.f +
											50.f * 1.f);
			selectButton100->setText("100");
			selectButton100->setCallback([this]()
			{
				if (_currentSettings->mMusicVolume != 100.f)
				{
					_currentSettings->mHasAnyChanges = true;
					_currentSettings->mMusicVolume = 100.f;
				}
				requestStackPop();
			});
			_guiContainer.pack(selectButton100);

			auto selectButton75 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
																*context.mSounds);
			selectButton75->setPosition(windowCenter.x - 100.f, 
										windowCenter.y - (windowSize.y / 2.f) + 100.f + 50.f * 2.f);
			selectButton75->setText("75");
			selectButton75->setCallback([this]()
			{
				if (_currentSettings->mMusicVolume != 75.f)
				{
					_currentSettings->mHasAnyChanges = true;
					_currentSettings->mMusicVolume = 75.f;
				}
				requestStackPop();
			});
			_guiContainer.pack(selectButton75);

			auto selectButton50 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
																*context.mSounds);
			selectButton50->setPosition(windowCenter.x - 100.f, 
										windowCenter.y - (windowSize.y / 2.f) + 100.f + 50.f * 3.f);
			selectButton50->setText("50");
			selectButton50->setCallback([this]()
			{
				if (_currentSettings->mMusicVolume != 50.f)
				{
					_currentSettings->mHasAnyChanges = true;
					_currentSettings->mMusicVolume = 50.f;
				}
				requestStackPop();
			});
			_guiContainer.pack(selectButton50);

			auto selectButton25 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
																*context.mSounds);
			selectButton25->setPosition(windowCenter.x - 100.f, 
										windowCenter.y - (windowSize.y / 2.f) + 100.f + 50.f * 4.f);
			selectButton25->setText("25");
			selectButton25->setCallback([this]()
			{
				if (_currentSettings->mMusicVolume != 25.f)
				{
					_currentSettings->mHasAnyChanges = true;
					_currentSettings->mMusicVolume = 25.f;
				}
				requestStackPop();
			});
			_guiContainer.pack(selectButton25);

			auto selectButton0 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
															   *context.mSounds);
			selectButton0->setPosition(windowCenter.x - 100.f, 
									   windowCenter.y - (windowSize.y / 2.f) + 100.f + 50.f * 5.f);
			selectButton0->setText("0");
			selectButton0->setCallback([this]()
			{
				if (_currentSettings->mMusicVolume != 0.f)
				{
					_currentSettings->mHasAnyChanges = true;
					_currentSettings->mMusicVolume = 0.f;
				}
				requestStackPop();
			});
			_guiContainer.pack(selectButton0);
			break;
		}
		// Pressed button "Change Font Type".
		case 3:
		{
			addText(L"Выберите шрифт:");

			auto selectButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
															  *context.mSounds);
			selectButton->setPosition(windowCenter.x - 100.f, 
									  windowCenter.y - (windowSize.y / 2.f) + 100.f + 50.f * 1.f);
			selectButton->setText("Noto Serif");
			selectButton->setCallback([this]()
			{
				if (_currentSettings->mFontType != Fonts::ID::Main)
				{
					_currentSettings->mHasAnyChanges = true;
					_currentSettings->mFontType = Fonts::ID::Main;
				}
				requestStackPop();
			});
			_guiContainer.pack(selectButton);
			break;
		}
		// Pressed button "Change Language".
		case 4:
		{
			addText(L"Выберите язык:");

			auto selectButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
															  *context.mSounds);
			selectButton->setPosition(windowCenter.x - 100.f, 
									  windowCenter.y - (windowSize.y / 2.f) + 100.f + 50.f * 1.f);
			selectButton->setText("Russian");
			selectButton->setCallback([this]()
			{
				if (_currentSettings->mLanguage != State::ActualLanguage::Russian)
				{
					_currentSettings->mHasAnyChanges = true;
					_currentSettings->mLanguage = State::ActualLanguage::Russian;
				}
				requestStackPop();
			});
			_guiContainer.pack(selectButton);
			break;
		}
		// Pressed button "Change Debug mode".
		case 5:
		{
			addText(L"Выберите режим работы:");

			auto selectButtonOn = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
															  *context.mSounds);
			selectButtonOn->setPosition(windowCenter.x - 100.f,
									  windowCenter.y - (windowSize.y / 2.f) + 100.f + 50.f * 1.f);
			selectButtonOn->setText(L"Включить");
			selectButtonOn->setCallback([this]()
			{
				if (_currentSettings->mDebugMode != State::DebugMode::DebugOn)
				{
					_currentSettings->mHasAnyChanges = true;
					_currentSettings->mDebugMode = State::DebugMode::DebugOn;
				}
				requestStackPop();
			});
			_guiContainer.pack(selectButtonOn);

			auto selectButtonOff = std::make_shared<GUI::Button>(*context.mFonts, 
																 *context.mTextures, 
																 *context.mSounds);
			selectButtonOff->setPosition(windowCenter.x - 100.f,
										windowCenter.y - (windowSize.y / 2.f) + 100.f + 50.f * 2.f);
			selectButtonOff->setText(L"Выключить");
			selectButtonOff->setCallback([this]()
			{
				if (_currentSettings->mDebugMode != State::DebugMode::DebugOff)
				{
					_currentSettings->mHasAnyChanges = true;
					_currentSettings->mDebugMode = State::DebugMode::DebugOff;
				}
				requestStackPop();
			});
			_guiContainer.pack(selectButtonOff);
			break;
		}
		// Pressed unknown button.
		default:
			addText("Invalid changing type. Unknown button.");
			break;
	}

	// Creates return button.
	auto backButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures,
													*context.mSounds);
	backButton->setPosition(windowCenter.x - 100.f, windowCenter.y + (windowSize.y / 2.f) - 100.f);
	backButton->setText(L"Вернуться в меню");
	backButton->setCallback(std::bind(&ChanginState::requestStackPop, this));

	_guiContainer.pack(backButton);

	setText(_textNumber);
}

void ChanginState::addText(const sf::String& text)
{
	_dialogText.push_back(text);
}

void ChanginState::setText(const std::size_t number)
{
	auto& text = _dialogText[number];
	/*
	 for (std::size_t i = 0; i < text.getSize(); ++i)
	{
		if (i % static_cast<int>(getContext().mWindow.getView().getSize().x /
				(mText.getCharacterSize() - 5.f)) == 0 && i > 0)
			text.insert(i, "\n");
	}
	*/
	_text.setString(text);
}

void ChanginState::draw()
{
	auto& window = *getContext().mWindow;
	window.setView(window.getDefaultView());

	const auto center = window.getView().getCenter();
	const auto size = window.getView().getSize();

	_text.setPosition(center.x - 150.f, center.y - (size.y / 2.f) + 100.f);

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);

	window.draw(_text);
	window.draw(_guiContainer);
}

bool ChanginState::update(const sf::Time)
{
	return false;
}

bool ChanginState::handleEvent(const sf::Event& event)
{
	if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
	{
		_sound.play();
	}

	_guiContainer.handleEvent(event);
	return false;
}
