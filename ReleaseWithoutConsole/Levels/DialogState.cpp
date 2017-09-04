#include "../Include/DialogState.hpp"
#include "../Include/Button.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


DialogState::DialogState(StateStack& stack, Context context)
: State(stack, context)
, textNumber(0)
, mTexture(context.textures->get(Textures::DialogBox))
, mSprite()
, mText()
, mTalking()
, mGUIContainer()
, mDialogText()
, mDialogTalking()
{
	sf::Vector2f windowSize(context.window->getView().getSize());
	sf::Vector2f windowCenter(context.window->getView().getCenter());

	mTalking.setFont(context.fonts->get(Fonts::Main));
	mTalking.setCharacterSize(24);

	mText.setFont(context.fonts->get(Fonts::Main));
	mText.setCharacterSize(18);
	mText.setFillColor(sf::Color::Black);
	mSprite.setTexture(mTexture);

	mSprite.setScale(windowSize.x / mSprite.getLocalBounds().width, 1.5f);

	switch(context.playerInfo->mDialogNumber)
	{
	case 1:
		addText("Hello, World! My name is Strange, I'm Archer from Strathclyde.", "Arantir");
		addText(L"��� ������� ����. �������, ��� �� ��������� ������������...", L"�������");
		addText("It's English. It's normal that there are not any errors.", "Arantir");
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
		addText(L"��� ������� ����. �������, ��� �� ��������� ������������...", L"�������");
		break;
	default:
		addText("It's English. It's normal that there are not any errors.", "Arantir");
		break;

	}

	setText(textNumber);

	auto nextButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	nextButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, windowCenter.y + windowSize.y / 2.f - 107.f);
	nextButton->setText(L"������");
	nextButton->setCallback([this] ()
	{
		if (textNumber == mDialogText.size() - 1)
		{
			requestStackPop();
		}
		else
		{
			textNumber++;
			setText(textNumber);
		}
	});
	mGUIContainer.pack(nextButton);

	auto skipButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
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
	sf::RenderWindow& window = *getContext().window;
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
	/*	
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
	*/

	mGUIContainer.handleEvent(event);
	return false;
}