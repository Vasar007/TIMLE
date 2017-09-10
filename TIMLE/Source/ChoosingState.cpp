#include "../Include/ChoosingState.hpp"
#include "../Include/Button.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


ChoosingState::ChoosingState(StateStack& stack, Context context)
: State(stack, context)
, mTextNumber(0)
, mGUIContainer()
, mSound()
, mDialogText()
, mDialogTalking()
, mPlayerInfo(context.mPlayerInfo)
{
	sf::Vector2f windowSize(context.mWindow->getView().getSize());
	sf::Vector2f windowCenter(context.mWindow->getView().getCenter());

	mSound.setBuffer(context.mSounds->get(Sounds::ButtonCLick));

	mText.setFont(context.mFonts->get(Fonts::Main));
	mText.setCharacterSize(24);
	mText.setFillColor(sf::Color::White);

	switch(mPlayerInfo->mChoosingNumber)
	{
		case 1:
		{
			addText(L"Хотите ли вы обыскать тела?");
			auto firstButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
			firstButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, windowCenter.y + windowSize.y / 2.f - 257.f);
			firstButton->setText(L"Да");
			firstButton->setCallback([this]()
			{
				mPlayerInfo->mChosenSolution[0] = 1;
				requestStackPop();
			});
			mGUIContainer.pack(firstButton);

			auto secondButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
			secondButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, windowCenter.y + windowSize.y / 2.f - 207.f);
			secondButton->setText(L"Нет");
			secondButton->setCallback([this]()
			{
				mPlayerInfo->mChosenSolution[0] = 2;
				requestStackPop();
			});
			mGUIContainer.pack(secondButton);
			break;
		}
		case 2:
		{
			addText(L"Пока что голем не нападает. Что вы намерены сделать?");

			if (mPlayerInfo->mChosenSolution[0] == 1)
			{
				auto firstButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
				firstButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, windowCenter.y + windowSize.y / 2.f - 307.f);
				firstButton->setText(L"Сказать фразу");
				firstButton->setCallback([this]()
				{
					mPlayerInfo->mChosenSolution[1] = 1;
					requestStackPop();
				});
				mGUIContainer.pack(firstButton);

				auto secondButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
				secondButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, windowCenter.y + windowSize.y / 2.f - 257.f);
				secondButton->setText(L"Атаковать монстра");
				secondButton->setCallback([this]()
				{
					mPlayerInfo->mChosenSolution[1] = 2;
					requestStackPop();
				});
				mGUIContainer.pack(secondButton);

				auto thirdButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
				thirdButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, windowCenter.y + windowSize.y / 2.f - 207.f);
				thirdButton->setText(L"Подождать");
				thirdButton->setCallback([this]()
				{
					mPlayerInfo->mChosenSolution[1] = 3;
					requestStackPop();
				});
				mGUIContainer.pack(thirdButton);
			}
			else if (mPlayerInfo->mChosenSolution[0] == 2)
			{
				auto firstButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
				firstButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, windowCenter.y + windowSize.y / 2.f - 257.f);
				firstButton->setText(L"Атаковать монстра");
				firstButton->setCallback([this]()
				{
					mPlayerInfo->mChosenSolution[1] = 2;
					requestStackPop();
				});
				mGUIContainer.pack(firstButton);

				auto secondButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
				secondButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, windowCenter.y + windowSize.y / 2.f - 207.f);
				secondButton->setText(L"Подождать");
				secondButton->setCallback([this]()
				{
					mPlayerInfo->mChosenSolution[1] = 3;
					requestStackPop();
				});
				mGUIContainer.pack(secondButton);
			}
			break;
		}
		default:
			addText("Invalid choosing type.");
			break;
	}

	setText(mTextNumber);
}

void ChoosingState::addText(sf::String text)
{
	mDialogText.push_back(text);
}

void ChoosingState::setText(size_t number)
{
	sf::String& text = mDialogText[number];
	/*
	 for (size_t i = 0; i < text.getSize(); i++)
	{
		if (i % static_cast<int>(getContext().window.getView().getSize().x / (mText.getCharacterSize() - 5.f)) == 0 && i > 0)
			text.insert(i, "\n");
	}
	*/
	mText.setString(text);
}

void ChoosingState::draw()
{
	sf::RenderWindow& window = *getContext().mWindow;
	window.setView(window.getDefaultView());

	sf::Vector2f center = window.getView().getCenter();
	sf::Vector2f size = window.getView().getSize();

	mText.setPosition(center.x - 200.f, center.y - 0.f);

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);

	window.draw(mText);
	window.draw(mGUIContainer);
}

bool ChoosingState::update(sf::Time)
{
	return false;
}

bool ChoosingState::handleEvent(const sf::Event& event)
{
	if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
	{
		mSound.play();
	}

	mGUIContainer.handleEvent(event);
	return false;
}
