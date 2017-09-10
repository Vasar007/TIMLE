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
			addText(L"Зная, что силы Зла начнут наступление с Теневых Земель, находящихся за Чёрным Хребтом на           дальнем востоке королевства, герой Стратклайда отправляется на Восточную долину, принявшую на    себя основной удар во время Великой Войны десять лет назад.", L"Повествование");
			addText(L"За десять лет, конечно, местные леса восстановиться не успели, но вместо гиблого пустыря,          напоминающего скорее мёртвую пустыню, сейчас это место представляло собой покрытые зеленью       холмы и луга. И именно сюда сейчас прибыли первые порождения Тьмы.", L"Повествование");
			break;
		case 2:
			addText(L"Проклятье...", L"Сэр Освальд");
			addText(L"В чём проблема?", L"Арантир");
			addText(L"Святой Дух, мои глаза не обманывают меня?!", L"Сэр Освальд");
			addText(L"Нет, сэр рыцарь. Так что такое?", L"Арантир");
			addText(L"Герой Стратклайда! Я уж начал думать, что всё пропало...", L"Сэр Освальд");
			addText(L"...", L"Арантир");
			addText(L"Прошу прощения, меня зовут сэр Освальд, я рыцарь ордена Священного Света. Вместе с моим давним    товарищем - сэром Генрихом, мы отозвались на призыв короля действовать против сил Зла на востоке и вдвоём двинулись в эти места. Когда мы наткнулись на эту пещеру, Генрих решил заглянуть внутрь,мне сказал ждать снаружи.", L"Сэр Освальд");
			addText(L"Не успело пройти и нескольких секунд, как я услышал его удаляющийся крик и в этот момент прямо    передо мной опустилась эта дверь... Я опасаюсь худшего. Я не хочу грузить вас чужими заботами,   господин, да к тому же это моя вина, что я отпустил Генриха туда одного... но всё же я был бы вамблагодарен, если бы вы смогли как-нибудь помочь....", L"Сэр Освальд");
			addText(L"М-м... Возможно я смог бы что-нибудь сделать... А может и нет.", L"Арантир");
			break;
		case 3:
			addText(L"На механизме вы видите причудиво закручивающееся углубление. Сюда бы подошло что-нибудь в форме шестиугольной звезды", L"Повествование");
			break;
		case 4:
			addText(L"На теле командира дворфов вы нашли странной формы камень в виде шестиугольной звезды", L"Повествование");
			break;
		case 5:
			addText(L"Вы вкладываете камень в разъём и он, крутясь и опускаясь, достигает дна механизма. Со стороны     раздаётся звук отходящей каменной двери", L"Повествование");
			break;
		case 6:
			addText(L"Прочь, тёмные твари! Я буду биться до конца!", L"Сэр Генрих");
			addText(L"Сэр Генрих?", L"Арантир");
			addText(L"Откуда вы знаете моё имя, отродья Преисподней!?", L"Сэр Генрих");
			addText(L"Сэр Освальд мне его сказал.", L"Арантир");
			addText(L"Освальд?", L"Сэр Генрих");
			// Artem: Тут он типа пару шагов делает, так?
			// Vasily: Ничего не знаю про шаги...
			addText(L"Святые лики, да вы же Герой Стратклайда!", L"Сэр Генрих");
			addText(L"Да-да, это я...", L"Арантир");
			addText(L"Слава Святым, я спасён. Немного прошёл во тьму, оступился и упал на самое дно пещеры. Там стал    отступать от приближающихся дворфов и гоблинов, а потом... А потом нескольких прирезал и вдруг   тьма. Думал, что всё, дни мои сочтены.", L"Сэр Генрих");
			addText(L"Сэр Освальд ждёт вас наверху.", L"Арантир");
			addText(L"Благодарю вас, господин. Сейчас передохну немного и пойду наверх.", L"Сэр Генрих");
			addText(L"Ага...", L"Арантир");
			break;
		case 7:
			addText(L"Вы нашли место сражения неизвестного рыцаря из ордена Священного Света и нескольких тёмных        дворфов.", L"Повествование");
			break;
		case 8:
			addText(L"Из груды темных камней перед вами вырос огромный голем.", L"Повествование");
			if (mPlayerInfo->mChosenSolution[0] == 1)
			{
				addText(L"Видимо о нём и говорилось на том листке, что был у дворфа. Амулет, который взяли с собой, залился ярким пурпурным светом. Видимо, он реагирует на присутствие тёмных сил или просто на магические  ауры. Руна находится на пьедестале позади монстра.", L"Повествование");
			}
			else if (mPlayerInfo->mChosenSolution[0] == 2)
			{
				addText(L"Трудно сказать наверняка, что он мог тут делать. Лишь в одном вы уверены наверняка – монстр здесь стоит не просто так. Позади него вы замечаете необычное свечение, но времени на разгядывание не  остаётся.", L"Повествование");
			}
			break;
		case 9:
			addText(L"Разобравшись с големом, вы смотрите на пьедестал с руной. Никаких ловушек вы не видите. Видимо,   монстр был единственным стражем этой вещи.", L"Повествование");
			addText(L"Приблизившись к алтарю, вы протягиваете руку и аккуратно берёте руну. Вокруг не начинается        землетрясение и не появляются толпы жутких тварей. Всё очень тихо. Однако вы чувствуете, что     магический камень начинает придавать вам сил. Вероятно, это лишь второстепенный эффект, ведь     стража для такой обычной зачарованной вещи уж вряд ли поставили бы.", L"Повествование");
			addText(L"Неожиданно возникшая вспышка света застаёт вас врасплох. «Ненавижу порталы, бл...» – всё, что     вы успели крикнуть напоследок. И вдруг оказываетесь среди трупов, откуда и начинали этот путь в  пещеру. «Чтобы я ещё раз спустился в такие руины. Да ни в жизнь больше» – проругались вы про     себя и продолжили свой путь.", L"Повествование");
			break;
		default:
			addText("Invalid dialog type.", "Error");
			break;
	}

	setText(mTextNumber);

	auto nextButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, *context.mSounds);
	nextButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, windowCenter.y + windowSize.y / 2.f - 107.f);
	nextButton->setText(L"Дальше");
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
				addText(L"На теле одного из дворфов вы находите бумажку, на которой написано следующее: «Идиоты, у вас есть ещё неделя, чтобы докопаться до хранилища и забрать эту чёртову руну. Если не успеете, Кархаваль всех тварям на корм пустит, поэтому давайте живей! И не забудьте, что там голем, просто скажите  ему: ''A d’yeabl aep arse''. Это то ли заклинание, то ли ещё хрень какая, которая его отключает».", L"Повествование");
				addText(L"В сжатой руке рыцаря вы находите маленький амулет с камнем пурпурного цвета. Последний ярко       светится. Чутьё говорит вам, что эта вещь зачарована.", L"Повествование");
			}
			else if (mPlayerInfo->mChosenSolution[0] == 2)
			{
				addText(L"Вы решаете оставить мёртвых в покое.", L"Повествование");
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
				addText(L"A d’yeabl aep arse.", L"Арантир");
				addText(L"С треском по телу голема начинают проходить массивные трещины. Однако через несколько секунд всё  останавливается – хранитель руны всё ещё стоит перед вами. Неудачно сработавшее заклятье не      смогло его разрушить. Монстр с грохотом начинает двигаться в вашу сторону.", L"Повествование");
				addText(L"Проклятье!", L"Арантир");
			}
			else if (mPlayerInfo->mChosenSolution[1] == 2)
			{
				addText(L"Я уничтожу тебя, порождение Бездны!", L"Арантир");
				addText(L"Хранитель руны без единого звука начинает с грохотом идти в вашу сторону.", L"Повествование");
			}
			else if (mPlayerInfo->mChosenSolution[1] == 3)
			{
				addText(L"Вы ждёте хоть какой-нибудь реакции от него.", L"Повествование");
				addText(L"Спустя десяток секунд ожидания хранитель руны без единого звука начинает с грохотом идти в вашу   сторону.", L"Повествование");
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
	skipButton->setText(L"Пропустить");
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