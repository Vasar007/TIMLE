#include "../Include/TitreState.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


TitreState::TitreState(StateStack& stack, Context context)
: State(stack, context)
, mText()
, mTextEffectTime(sf::Time::Zero)
, mTextAppearenceTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.mTextures->get(Textures::TitleScreen));

	mText.setFont(context.mFonts->get(Fonts::Main));
	mText.setString(L"SoftForAll                                                      Создатели:                                                     Васильев Василий                                               Зыков Артём                                                    Гусев Данила                                                   Нажмите любую кнопку для продолжения");
	setText();
	centerOrigin(mText);
	mText.setPosition(sf::Vector2f(context.mWindow->getSize().x / 2.f, 
								   context.mWindow->getSize().y / 2.f + 350.f));
	mText.setOutlineColor(sf::Color::Black);
}

void TitreState::draw()
{
	sf::RenderWindow& window = *getContext().mWindow;
	window.draw(mBackgroundSprite);

	window.draw(mText);
}

bool TitreState::update(sf::Time dt)
{
	mTextEffectTime += dt;
	mTextAppearenceTime += dt;

	if (mTextEffectTime >= sf::seconds(0.01f))
	{
		mText.setPosition(mText.getPosition().x, mText.getPosition().y - 
						  static_cast<float>(mTextEffectTime.asSeconds()) * 100.f);
		mTextEffectTime = sf::Time::Zero;
	}

	if (mTextAppearenceTime > sf::seconds(10.f))
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}

	return true;
}

bool TitreState::handleEvent(const sf::Event& event)
{
	// If any key is pressed, trigger the next screen.
	if (event.type == sf::Event::KeyReleased)
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}

	return true;
}

void TitreState::setText()
{
	sf::String text = mText.getString();
	for (size_t i = 0; i < text.getSize(); i++)
	{
		if (i % static_cast<int>(mBackgroundSprite.getGlobalBounds().width /
			(mText.getCharacterSize() - 5.f)) == 0 && i > 0)
		{
			text.insert(i, "\n");
		}
	}
	mText.setString(text);
}
