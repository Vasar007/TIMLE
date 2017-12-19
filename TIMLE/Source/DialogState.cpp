#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "../Include/DialogState.hpp"
#include "../Include/Button.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"


DialogState::DialogState(StateStack& stack, Context context)
: State(stack, context)
, _textNumber(0u)
, _choosing(false)
, _texture(context.mTextures->get(Textures::ID::DialogBox))
, _sprite()
, _text()
, _talking()
, _guiContainer()
, _sound()
, _dialogText()
, _dialogTalking()
, _playerInfo(*context.mPlayerInfo)
{
	const auto windowSize(context.mWindow->getView().getSize());
	const auto windowCenter(context.mWindow->getView().getCenter());

	_sound.setBuffer(context.mSounds->get(Sounds::ID::ButtonCLick));

	_talking.setFont(context.mFonts->get(Fonts::ID::Main));
	_talking.setCharacterSize(24);

	_text.setFont(context.mFonts->get(Fonts::ID::Main));
	_text.setCharacterSize(18);
	_text.setFillColor(sf::Color::Black);
	_sprite.setTexture(_texture);

	_sprite.setScale(windowSize.x / _sprite.getLocalBounds().width, 1.5f);

	switch(_playerInfo.mDialogNumber)
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
			if (_playerInfo.mChosenSolution[0] == 1)
			{
				addText(L"������ � �� � ���������� �� ��� ������, ��� ��� � ������. ������, ������� ����� � �����, ������� ����� ��������� ������. ������, �� ��������� �� ����������� ����� ��� ��� ������ �� ����������  ����. ���� ��������� �� ���������� ������ �������.", L"�������������");
			}
			else if (_playerInfo.mChosenSolution[0] == 2)
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

	setText(_textNumber);

	auto nextButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, 
													*context.mSounds);
	nextButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, 
							windowCenter.y + windowSize.y / 2.f - 107.f);
	nextButton->setText(L"������");
	nextButton->setCallback([this] ()
	{
		if (_playerInfo.mDialogNumber == 7 && _playerInfo.mChosenSolution[0] == 0)
		{
			_playerInfo.mChoosingNumber = 1;
			requestStackPush(States::ID::Choosing);
			_choosing = true;
		}
		else if (_playerInfo.mDialogNumber == 7 && _playerInfo.mChosenSolution[0] != 0 &&
			_textNumber == 0)
		{
			if (_playerInfo.mChosenSolution[0] == 1)
			{
				addText(L"�� ���� ������ �� ������� �� �������� �������, �� ������� �������� ���������: �������, � ��� ���� ��� ������, ����� ���������� �� ��������� � ������� ��� ������� ����. ���� �� �������, ��������� ���� ������ �� ���� ������, ������� ������� �����! � �� ��������, ��� ��� �����, ������ �������  ���: ''A d�yeabl aep arse''. ��� �� �� ����������, �� �� ��� ����� �����, ������� ��� ���������.", L"�������������");
				addText(L"� ������ ���� ������ �� �������� ��������� ������ � ������ ���������� �����. ��������� ����       ��������. ����� ������� ���, ��� ��� ���� ����������.", L"�������������");
			}
			else if (_playerInfo.mChosenSolution[0] == 2)
			{
				addText(L"�� ������� �������� ������ � �����.", L"�������������");
			}
			++_textNumber;
			setText(_textNumber);
			_choosing = false;
		}

		else if (_playerInfo.mDialogNumber == 8 && _playerInfo.mChosenSolution[1] == 0 &&
			_textNumber == 1)
		{
			_playerInfo.mChoosingNumber = 2;
			requestStackPush(States::ID::Choosing);
			_choosing = true;
		}
		else if (_playerInfo.mDialogNumber == 8 && _playerInfo.mChosenSolution[1] != 0 &&
			_textNumber == 1)
		{
			if (_playerInfo.mChosenSolution[1] == 1)
			{
				addText(L"A d�yeabl aep arse.", L"�������");
				addText(L"� ������� �� ���� ������ �������� ��������� ��������� �������. ������ ����� ��������� ������ ��  ��������������� � ��������� ���� �� ��� ����� ����� ����. �������� ����������� �������� ��      ������ ��� ���������. ������ � �������� �������� ��������� � ���� �������.", L"�������������");
				addText(L"���������!", L"�������");
			}
			else if (_playerInfo.mChosenSolution[1] == 2)
			{
				addText(L"� �������� ����, ���������� ������!", L"�������");
				addText(L"��������� ���� ��� ������� ����� �������� � �������� ���� � ���� �������.", L"�������������");
			}
			else if (_playerInfo.mChosenSolution[1] == 3)
			{
				addText(L"�� ���� ���� �����-������ ������� �� ����.", L"�������������");
				addText(L"������ ������� ������ �������� ��������� ���� ��� ������� ����� �������� � �������� ���� � ����   �������.", L"�������������");
			}
			++_textNumber;
			setText(_textNumber);
			_choosing = false;
		}



		else if (_textNumber == _dialogText.size() - 1 && !_choosing)
		{
			requestStackPop();
		}
		else
		{
			++_textNumber;
			setText(_textNumber);
		}
	});
	_guiContainer.pack(nextButton);

	auto skipButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, 
													*context.mSounds);
	skipButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, 
							windowCenter.y + windowSize.y / 2.f - 57.f);
	skipButton->setText(L"����������");
	skipButton->setCallback([this]()
	{
		requestStackPop();
	});
	_guiContainer.pack(skipButton);
}

void DialogState::addText(const sf::String text, const sf::String talking)
{
	_dialogText.push_back(text);
	_dialogTalking.push_back(talking);
}

void DialogState::setText(const std::size_t number)
{
	auto& text = _dialogText[number];
	for (std::size_t i = 0; i < text.getSize(); ++i)
	{
		if (i % static_cast<int>(_sprite.getGlobalBounds().width /
			(_text.getCharacterSize() - 5.f)) == 0 && i > 0)
		{
			text.insert(i, "\n");
		}
	}
	_text.setString(text);
	_talking.setString(_dialogTalking[number]);
}

void DialogState::draw()
{
	auto& window = *getContext().mWindow;
	window.setView(window.getDefaultView());

	const auto center = window.getView().getCenter();
	const auto size = window.getView().getSize();

	_sprite.setScale(size.x / _sprite.getLocalBounds().width, 1.5f);

	_text.setPosition(center.x - size.x / 2.f + 20.f,
					  center.y + size.y / 2.f - _sprite.getGlobalBounds().height + 10.f);
	_talking.setPosition(center.x - size.x / 2.f + 20.f,
						 center.y + size.y / 2.f - _sprite.getGlobalBounds().height - 30.f);
	_sprite.setPosition(center.x - size.x / 2.f,
						center.y + size.y / 2.f - _sprite.getGlobalBounds().height);

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);

	window.draw(_sprite);
	window.draw(_text);
	window.draw(_talking);
	window.draw(_guiContainer);
}

bool DialogState::update(const sf::Time)
{
	return false;
}

bool DialogState::handleEvent(const sf::Event& event)
{
	if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
	{
		_sound.play();
	}

	_guiContainer.handleEvent(event);
	return false;
}
