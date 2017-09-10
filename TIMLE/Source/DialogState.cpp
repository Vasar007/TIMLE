#include "../Include/DialogState.hpp"
#include "../Include/Button.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


DialogState::DialogState(StateStack& stack, Context context)
: State(stack, context)
, mTextNumber(0)
, mChoosing(false)
, mTexture(context.mTextures->get(Textures::DialogBox))
, mSprite()
, mText()
, mTalking()
, mGUIContainer()
, mSound()
, mDialogText()
, mDialogTalking()
, mPlayerInfo(context.mPlayerInfo)
{
	sf::Vector2f windowSize(context.mWindow->getView().getSize());
	sf::Vector2f windowCenter(context.mWindow->getView().getCenter());

	mSound.setBuffer(context.mSounds->get(Sounds::ButtonCLick));

	mTalking.setFont(context.mFonts->get(Fonts::Main));
	mTalking.setCharacterSize(24);

	mText.setFont(context.mFonts->get(Fonts::Main));
	mText.setCharacterSize(18);
	mText.setFillColor(sf::Color::Black);
	mSprite.setTexture(mTexture);

	mSprite.setScale(windowSize.x / mSprite.getLocalBounds().width, 1.5f);

	switch(mPlayerInfo->mDialogNumber)
	{
		case 1:
			addText(L"����, ��� ���� ��� ������ ����������� � ������� ������, ����������� �� ׸���� ������� ��           ������� ������� �����������, ����� ����������� ������������ �� ��������� ������, ��������� ��    ���� �������� ���� �� ����� ������� ����� ������ ��� �����.", L"�������������");
			addText(L"�� ������ ���, �������, ������� ���� �������������� �� ������, �� ������ ������� �������,          ������������� ������ ������ �������, ������ ��� ����� ������������ ����� �������� �������       ����� � ����. � ������ ���� ������ ������� ������ ���������� ����.", L"�������������");
			break;
		case 2:
			addText(L"���������...", L"��� �������");
			addText(L"� ��� ��������?", L"�������");
			addText(L"������ ���, ��� ����� �� ���������� ����?!", L"��� �������");
			addText(L"���, ��� ������. ��� ��� �����?", L"�������");
			addText(L"����� �����������! � �� ����� ������, ��� �� �������...", L"��� �������");
			addText(L"...", L"�������");
			addText(L"����� ��������, ���� ����� ��� �������, � ������ ������ ���������� �����. ������ � ���� ������    ��������� - ����� ��������, �� ���������� �� ������ ������ ����������� ������ ��� ��� �� ������� � ����� ��������� � ��� �����. ����� �� ���������� �� ��� ������, ������ ����� ��������� ������,��� ������ ����� �������.", L"��� �������");
			addText(L"�� ������ ������ � ���������� ������, ��� � ������� ��� ����������� ���� � � ���� ������ �����    ������ ���� ���������� ��� �����... � �������� �������. � �� ���� ������� ��� ������ ��������,   ��������, �� � ���� �� ��� ��� ����, ��� � �������� ������� ���� ������... �� �� �� � ��� �� �������������, ���� �� �� ������ ���-������ ������....", L"��� �������");
			addText(L"�-�... �������� � ���� �� ���-������ �������... � ����� � ���.", L"�������");
			break;
		case 3:
			addText(L"�� ��������� �� ������ ��������� ��������������� ����������. ���� �� ������� ���-������ � ����� ������������� ������", L"�������������");
			break;
		case 4:
			addText(L"�� ���� ��������� ������� �� ����� �������� ����� ������ � ���� ������������� ������", L"�������������");
			break;
		case 5:
			addText(L"�� ����������� ������ � ������ � ��, ������� � ���������, ��������� ��� ���������. �� �������     �������� ���� ��������� �������� �����", L"�������������");
			break;
		case 6:
			addText(L"�����, ����� �����! � ���� ������ �� �����!", L"��� ������");
			addText(L"��� ������?", L"�������");
			addText(L"������ �� ������ �� ���, ������� �����������!?", L"��� ������");
			addText(L"��� ������� ��� ��� ������.", L"�������");
			addText(L"�������?", L"��� ������");
			// Artem: ��� �� ���� ���� ����� ������, ���?
			// Vasily: ������ �� ���� ��� ����...
			addText(L"������ ����, �� �� �� ����� �����������!", L"��� ������");
			addText(L"��-��, ��� �...", L"�������");
			addText(L"����� ������, � �����. ������� ������ �� ����, ��������� � ���� �� ����� ��� ������. ��� ����    ��������� �� �������������� ������� � ��������, � �����... � ����� ���������� �������� � �����   ����. �����, ��� ��, ��� ��� �������.", L"��� ������");
			addText(L"��� ������� ��� ��� �������.", L"�������");
			addText(L"��������� ���, ��������. ������ ��������� ������� � ����� ������.", L"��� ������");
			addText(L"���...", L"�������");
			break;
		case 7:
			addText(L"�� ����� ����� �������� ������������ ������ �� ������ ���������� ����� � ���������� �����        �������.", L"�������������");
			break;
		case 8:
			addText(L"�� ����� ������ ������ ����� ���� ����� �������� �����.", L"�������������");
			if (mPlayerInfo->mChosenSolution[0] == 1)
			{
				addText(L"������ � �� � ���������� �� ��� ������, ��� ��� � ������. ������, ������� ����� � �����, ������� ����� ��������� ������. ������, �� ��������� �� ����������� ����� ��� ��� ������ �� ����������  ����. ���� ��������� �� ���������� ������ �������.", L"�������������");
			}
			else if (mPlayerInfo->mChosenSolution[0] == 2)
			{
				addText(L"������ ������� ���������, ��� �� ��� ��� ������. ���� � ����� �� ������� ��������� � ������ ����� ����� �� ������ ���. ������ ���� �� ��������� ��������� ��������, �� ������� �� ������������ ��  �������.", L"�������������");
			}
			break;
		case 9:
			addText(L"������������ � �������, �� �������� �� ��������� � �����. ������� ������� �� �� ������. ������,   ������ ��� ������������ ������� ���� ����.", L"�������������");
			addText(L"������������� � ������, �� ������������ ���� � ��������� ����� ����. ������ �� ����������        ������������� � �� ���������� ����� ������ ������. �� ����� ����. ������ �� ����������, ���     ���������� ������ �������� ��������� ��� ���. ��������, ��� ���� �������������� ������, ����     ������ ��� ����� ������� ������������ ���� �� ���� �� ��������� ��.", L"�������������");
			addText(L"���������� ��������� ������� ����� ������ ��� ��������. ��������� �������, ��...� � ��, ���     �� ������ �������� ����������. � ����� ������������ ����� ������, ������ � �������� ���� ���� �  ������. ������ � ��� ��� ��������� � ����� �����. �� �� � ����� ������ � ����������� �� ���     ���� � ���������� ���� ����.", L"�������������");
			break;
		default:
			addText("Invalid dialog type.", "Error");
			break;
	}

	setText(mTextNumber);

	auto nextButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
	nextButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, windowCenter.y + windowSize.y / 2.f - 107.f);
	nextButton->setText(L"������");
	nextButton->setCallback([this] ()
	{
		if ((mPlayerInfo->mDialogNumber == 7) && (mPlayerInfo->mChosenSolution[0] == 0))
		{
			mPlayerInfo->mChoosingNumber = 1;
			requestStackPush(States::Choosing);
			mChoosing = true;
		}
		else if ((mPlayerInfo->mDialogNumber == 7) && (mPlayerInfo->mChosenSolution[0] != 0) && (mTextNumber == 0))
		{
			if (mPlayerInfo->mChosenSolution[0] == 1)
			{
				addText(L"�� ���� ������ �� ������� �� �������� �������, �� ������� �������� ���������: �������, � ��� ���� ��� ������, ����� ���������� �� ��������� � ������� ��� ������� ����. ���� �� �������, ��������� ���� ������ �� ���� ������, ������� ������� �����! � �� ��������, ��� ��� �����, ������ �������  ���: ''A d�yeabl aep arse''. ��� �� �� ����������, �� �� ��� ����� �����, ������� ��� ���������.", L"�������������");
				addText(L"� ������ ���� ������ �� �������� ��������� ������ � ������ ���������� �����. ��������� ����       ��������. ����� ������� ���, ��� ��� ���� ����������.", L"�������������");
			}
			else if (mPlayerInfo->mChosenSolution[0] == 2)
			{
				addText(L"�� ������� �������� ������ � �����.", L"�������������");
			}
			mTextNumber++;
			setText(mTextNumber);
			mChoosing = false;
		}

		else if ((mPlayerInfo->mDialogNumber == 8) && (mPlayerInfo->mChosenSolution[1] == 0) && (mTextNumber == 1))
		{
			mPlayerInfo->mChoosingNumber = 2;
			requestStackPush(States::Choosing);
			mChoosing = true;
		}
		else if ((mPlayerInfo->mDialogNumber == 8) && (mPlayerInfo->mChosenSolution[1] != 0) && (mTextNumber == 1))
		{
			if (mPlayerInfo->mChosenSolution[1] == 1)
			{
				addText(L"A d�yeabl aep arse.", L"�������");
				addText(L"� ������� �� ���� ������ �������� ��������� ��������� �������. ������ ����� ��������� ������ ��  ��������������� � ��������� ���� �� ��� ����� ����� ����. �������� ����������� �������� ��      ������ ��� ���������. ������ � �������� �������� ��������� � ���� �������.", L"�������������");
				addText(L"���������!", L"�������");
			}
			else if (mPlayerInfo->mChosenSolution[1] == 2)
			{
				addText(L"� �������� ����, ���������� ������!", L"�������");
				addText(L"��������� ���� ��� ������� ����� �������� � �������� ���� � ���� �������.", L"�������������");
			}
			else if (mPlayerInfo->mChosenSolution[1] == 3)
			{
				addText(L"�� ���� ���� �����-������ ������� �� ����.", L"�������������");
				addText(L"������ ������� ������ �������� ��������� ���� ��� ������� ����� �������� � �������� ���� � ����   �������.", L"�������������");
			}
			mTextNumber++;
			setText(mTextNumber);
			mChoosing = false;
		}



		else if ((mTextNumber == mDialogText.size() - 1) && !mChoosing)
		{
			requestStackPop();
		}
		else
		{
			mTextNumber++;
			setText(mTextNumber);
		}
	});
	mGUIContainer.pack(nextButton);

	auto skipButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
	skipButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, windowCenter.y + windowSize.y / 2.f - 57.f);
	skipButton->setText(L"����������");
	skipButton->setCallback([this]()
	{
		requestStackPop();
	});
	mGUIContainer.pack(skipButton);
}

void DialogState::addText(sf::String text, sf::String talking)
{
	mDialogText.push_back(text);
	mDialogTalking.push_back(talking);
}

void DialogState::setText(size_t number)
{
	sf::String& text = mDialogText[number];
	for (size_t i = 0; i < text.getSize(); i++)
	{
		if (i % static_cast<int>(mSprite.getGlobalBounds().width / (mText.getCharacterSize() - 5.f)) == 0 && i > 0)
			text.insert(i, "\n");
	}
	mText.setString(text);
	mTalking.setString(mDialogTalking[number]);
}

void DialogState::draw()
{
	sf::RenderWindow& window = *getContext().mWindow;
	window.setView(window.getDefaultView());

	sf::Vector2f center = window.getView().getCenter();
	sf::Vector2f size = window.getView().getSize();

	mSprite.setScale(size.x / mSprite.getLocalBounds().width, 1.5f);

	mText.setPosition(center.x - size.x / 2.f + 20.f, center.y + size.y / 2.f - mSprite.getGlobalBounds().height + 10.f);
	mTalking.setPosition(center.x - size.x / 2.f + 20.f, center.y + size.y / 2.f - mSprite.getGlobalBounds().height - 30.f);
	mSprite.setPosition(center.x - size.x / 2.f, center.y + size.y / 2.f - mSprite.getGlobalBounds().height);

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);

	window.draw(mSprite);
	window.draw(mText);
	window.draw(mTalking);
	window.draw(mGUIContainer);
}

bool DialogState::update(sf::Time)
{
	return false;
}

bool DialogState::handleEvent(const sf::Event& event)
{
	if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
	{
		mSound.play();
	}

	mGUIContainer.handleEvent(event);
	return false;
}