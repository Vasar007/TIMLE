#include "../Include/ChangingState.hpp"
#include "../Include/Button.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


ChanginState::ChanginState(StateStack& stack, Context context)
: State(stack, context)
, mTextNumber(0)
, mGUIContainer()
, mSound()
, mDialogText()
, mPlayerInfo(context.mPlayerInfo)
, mCurrentSettings(context.mCurrentSettings)
{
	/*
	Get info about display resolution.
	std::cout << sf::VideoMode::getDesktopMode().width << " " << 
		sf::VideoMode::getDesktopMode().height << std::endl;
	std::cout << "---------------" << std::endl;
	std::vector<sf::VideoMode> sample = sf::VideoMode::getFullscreenModes();
	for (size_t i = 0; i < sample.size(); i++)
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
	sf::Vector2f windowSize(context.mWindow->getView().getSize());
	sf::Vector2f windowCenter(context.mWindow->getView().getCenter());

	mSound.setBuffer(context.mSounds->get(Sounds::ButtonCLick));

	mText.setFont(context.mFonts->get(Fonts::Main));
	mText.setCharacterSize(24);
	mText.setFillColor(sf::Color::White);

	// Create a GUI with dependencies from pressed button.
	switch(mCurrentSettings->mPressedButton)
	{
		// Pressed button "Change Window Size".
		case 0:
		{
			addText(L"Выберите размер окна:");

			std::vector<sf::VideoMode> fullscreenModes = sf::VideoMode::getFullscreenModes();

			size_t counter = 0;

			for (size_t i = 0; i < fullscreenModes.size(); i++)
			{
				if ((fullscreenModes[i].width == 1920) && (fullscreenModes[i].height == 1080))
				{
					counter++;

					auto selectButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
					selectButton->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * counter));
					selectButton->setText("1920 x 1080");
					selectButton->setCallback([this, fullscreenModes, i]()
					{
						if ((mCurrentSettings->mWindowSize.x != fullscreenModes[i].width) && (mCurrentSettings->mWindowSize.y != fullscreenModes[i].height))
						{
							mCurrentSettings->mHasAnyChanges = true;
							mCurrentSettings->mWindowSize = sf::Vector2u(1920, 1080);
						}
						requestStackPop();
					});
					mGUIContainer.pack(selectButton);
				}
				if ((fullscreenModes[i].width == 1600) && (fullscreenModes[i].height == 900))
				{
					counter++;

					auto selectButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
					selectButton->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * counter));
					selectButton->setText("1600 x 900");
					selectButton->setCallback([this, fullscreenModes, i]()
					{
						if ((mCurrentSettings->mWindowSize.x != fullscreenModes[i].width) && (mCurrentSettings->mWindowSize.y != fullscreenModes[i].height))
						{
							mCurrentSettings->mHasAnyChanges = true;
							mCurrentSettings->mWindowSize = sf::Vector2u(1600, 900);
						}
						requestStackPop();
					});
					mGUIContainer.pack(selectButton);
				}
				if ((fullscreenModes[i].width == 1366) && (fullscreenModes[i].height == 768))
				{
					counter++;

					auto selectButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
					selectButton->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * counter));
					selectButton->setText("1366 x 768");
					selectButton->setCallback([this, fullscreenModes, i]()
					{
						if ((mCurrentSettings->mWindowSize.x != fullscreenModes[i].width) && (mCurrentSettings->mWindowSize.y != fullscreenModes[i].height))
						{
							mCurrentSettings->mHasAnyChanges = true;
							mCurrentSettings->mWindowSize = sf::Vector2u(1366, 768);
						}
						requestStackPop();
					});
					mGUIContainer.pack(selectButton);
				}
				if ((fullscreenModes[i].width == 1280) && (fullscreenModes[i].height == 1024))
				{
					counter++;

					auto selectButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
					selectButton->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * counter));
					selectButton->setText("1280 x 1024");
					selectButton->setCallback([this, fullscreenModes, i]()
					{
						if (((mCurrentSettings->mWindowSize.x != fullscreenModes[i].width) && (mCurrentSettings->mWindowSize.y != fullscreenModes[i].height)) 
							||((mCurrentSettings->mWindowSize.x == 1280) && (mCurrentSettings->mWindowSize.y == 720)))
						{
							mCurrentSettings->mHasAnyChanges = true;
							mCurrentSettings->mWindowSize = sf::Vector2u(1280, 1024);
						}
						requestStackPop();
					});
					mGUIContainer.pack(selectButton);
				}
				if ((fullscreenModes[i].width == 1280) && (fullscreenModes[i].height == 720))
				{
					counter++;

					auto selectButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
					selectButton->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * counter));
					selectButton->setText("1280 x 720");
					selectButton->setCallback([this, fullscreenModes, i]()
					{
						if (((mCurrentSettings->mWindowSize.x != fullscreenModes[i].width) && (mCurrentSettings->mWindowSize.y != fullscreenModes[i].height))
							|| ((mCurrentSettings->mWindowSize.x == 1280) && (mCurrentSettings->mWindowSize.y == 1024)))
						{
							mCurrentSettings->mHasAnyChanges = true;
							mCurrentSettings->mWindowSize = sf::Vector2u(1280, 720);
						}
						requestStackPop();
					});
					mGUIContainer.pack(selectButton);
				}
				if ((fullscreenModes[i].width == 1024) && (fullscreenModes[i].height == 768))
				{
					counter++;

					auto selectButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
					selectButton->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * counter));
					selectButton->setText("1024 x 768");
					selectButton->setCallback([this, fullscreenModes, i]()
					{
						if ((mCurrentSettings->mWindowSize.x != fullscreenModes[i].width) && (mCurrentSettings->mWindowSize.y != fullscreenModes[i].height))
						{
							mCurrentSettings->mHasAnyChanges = true;
							mCurrentSettings->mWindowSize = sf::Vector2u(1024, 768);
						}
						requestStackPop();
					});
					mGUIContainer.pack(selectButton);
				}
			}
			break;
		}
		// Pressed button "Change Window Style".
		case 1:
		{
			addText(L"Выберите режим окна:");

			auto selectButton1 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
			selectButton1->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * 1.f));
			selectButton1->setText(L"На полный экран");
			selectButton1->setCallback([this]()
			{
				if (mCurrentSettings->mWindowStyle != Fullscreen)
				{
					mCurrentSettings->mHasAnyChanges = true;
					mCurrentSettings->mWindowStyle = Fullscreen;
				}
				requestStackPop();
			});
			mGUIContainer.pack(selectButton1);

			auto selectButton2 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
			selectButton2->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * 2.f));
			selectButton2->setText(L"Оконный");
			selectButton2->setCallback([this]()
			{
				if (mCurrentSettings->mWindowStyle != Close)
				{
					mCurrentSettings->mHasAnyChanges = true;
					mCurrentSettings->mWindowStyle = Close;
				}
				requestStackPop();
			});
			mGUIContainer.pack(selectButton2);
			break;
		}
		// Pressed button "Change Music Volume".
		case 2:
		{
			addText(L"Выберите громкость музыки:");

			auto selectButton100 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
			selectButton100->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * 1.f));
			selectButton100->setText("100");
			selectButton100->setCallback([this]()
			{
				if (mCurrentSettings->mMusicVolume != 100.f)
				{
					mCurrentSettings->mHasAnyChanges = true;
					mCurrentSettings->mMusicVolume = 100.f;
				}
				requestStackPop();
			});
			mGUIContainer.pack(selectButton100);

			auto selectButton75 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
			selectButton75->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * 2.f));
			selectButton75->setText("75");
			selectButton75->setCallback([this]()
			{
				if (mCurrentSettings->mMusicVolume != 75.f)
				{
					mCurrentSettings->mHasAnyChanges = true;
					mCurrentSettings->mMusicVolume = 75.f;
				}
				requestStackPop();
			});
			mGUIContainer.pack(selectButton75);

			auto selectButton50 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
			selectButton50->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * 3.f));
			selectButton50->setText("50");
			selectButton50->setCallback([this]()
			{
				if (mCurrentSettings->mMusicVolume != 50.f)
				{
					mCurrentSettings->mHasAnyChanges = true;
					mCurrentSettings->mMusicVolume = 50.f;
				}
				requestStackPop();
			});
			mGUIContainer.pack(selectButton50);

			auto selectButton25 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
			selectButton25->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * 4.f));
			selectButton25->setText("25");
			selectButton25->setCallback([this]()
			{
				if (mCurrentSettings->mMusicVolume != 25.f)
				{
					mCurrentSettings->mHasAnyChanges = true;
					mCurrentSettings->mMusicVolume = 25.f;
				}
				requestStackPop();
			});
			mGUIContainer.pack(selectButton25);

			auto selectButton0 = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
			selectButton0->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * 5.f));
			selectButton0->setText("0");
			selectButton0->setCallback([this]()
			{
				if (mCurrentSettings->mMusicVolume != 0.f)
				{
					mCurrentSettings->mHasAnyChanges = true;
					mCurrentSettings->mMusicVolume = 0.f;
				}
				requestStackPop();
			});
			mGUIContainer.pack(selectButton0);
			break;
		}
		// Pressed button "Change Font Type".
		case 3:
		{
			addText(L"Выберите шрифт:");

			auto selectButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
			selectButton->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * 1.f));
			selectButton->setText("Noto Serif");
			selectButton->setCallback([this]()
			{
				if (mCurrentSettings->mFontType != Fonts::Main)
				{
					mCurrentSettings->mHasAnyChanges = true;
					mCurrentSettings->mFontType = Fonts::Main;
				}
				requestStackPop();
			});
			mGUIContainer.pack(selectButton);
			break;
		}
		// Pressed button "Change Language".
		case 4:
		{
			addText(L"Выберите язык:");

			auto selectButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
			selectButton->setPosition(windowCenter.x - 100.f, windowCenter.y - (windowSize.y / 2.f) + 100.f + (50.f * 1.f));
			selectButton->setText("Russian");
			selectButton->setCallback([this]()
			{
				if (mCurrentSettings->mLanguage != Russian)
				{
					mCurrentSettings->mHasAnyChanges = true;
					mCurrentSettings->mLanguage = Russian;
				}
				requestStackPop();
			});
			mGUIContainer.pack(selectButton);
			break;
		}
		// Pressed unknown button.
		default:
			addText("Invalid changing type. Unknown button.");
			break;
	}

	// Creates return button.
	auto backButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
	backButton->setPosition(windowCenter.x - 100.f, windowCenter.y + (windowSize.y / 2.f) - 100.f);
	backButton->setText(L"Вернуться в меню");
	backButton->setCallback(std::bind(&ChanginState::requestStackPop, this));

	mGUIContainer.pack(backButton);

	setText(mTextNumber);
}

void ChanginState::addText(sf::String text)
{
	mDialogText.push_back(text);
}

void ChanginState::setText(size_t number)
{
	sf::String& text = mDialogText[number];
	/*
	 for (size_t i = 0; i < text.getSize(); i++)
	{
		if (i % static_cast<int>(getContext().mWindow.getView().getSize().x / (mText.getCharacterSize() - 5.f)) == 0 && i > 0)
			text.insert(i, "\n");
	}
	*/
	mText.setString(text);
}

void ChanginState::draw()
{
	sf::RenderWindow& window = *getContext().mWindow;
	window.setView(window.getDefaultView());

	sf::Vector2f center = window.getView().getCenter();
	sf::Vector2f size = window.getView().getSize();

	mText.setPosition(center.x - 150.f, center.y - (size.y / 2.f) + 100.f);

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);

	window.draw(mText);
	window.draw(mGUIContainer);
}

bool ChanginState::update(sf::Time)
{
	return false;
}

bool ChanginState::handleEvent(const sf::Event& event)
{
	if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
	{
		mSound.play();
	}

	mGUIContainer.handleEvent(event);
	return false;
}
