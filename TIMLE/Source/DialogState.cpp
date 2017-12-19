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
			if (_playerInfo.mChosenSolution[0] == 1)
			{
				addText(L"Видимо о нём и говорилось на том листке, что был у дворфа. Амулет, который взяли с собой, залился ярким пурпурным светом. Видимо, он реагирует на присутствие тёмных сил или просто на магические  ауры. Руна находится на пьедестале позади монстра.", L"Повествование");
			}
			else if (_playerInfo.mChosenSolution[0] == 2)
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

	setText(_textNumber);

	auto nextButton = std::make_shared<GUI::Button>(*context.mFonts, *context.mTextures, 
													*context.mSounds);
	nextButton->setPosition(windowCenter.x + windowSize.x / 2.f - 220.f, 
							windowCenter.y + windowSize.y / 2.f - 107.f);
	nextButton->setText(L"Дальше");
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
				addText(L"На теле одного из дворфов вы находите бумажку, на которой написано следующее: «Идиоты, у вас есть ещё неделя, чтобы докопаться до хранилища и забрать эту чёртову руну. Если не успеете, Кархаваль всех тварям на корм пустит, поэтому давайте живей! И не забудьте, что там голем, просто скажите  ему: ''A d’yeabl aep arse''. Это то ли заклинание, то ли ещё хрень какая, которая его отключает».", L"Повествование");
				addText(L"В сжатой руке рыцаря вы находите маленький амулет с камнем пурпурного цвета. Последний ярко       светится. Чутьё говорит вам, что эта вещь зачарована.", L"Повествование");
			}
			else if (_playerInfo.mChosenSolution[0] == 2)
			{
				addText(L"Вы решаете оставить мёртвых в покое.", L"Повествование");
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
				addText(L"A d’yeabl aep arse.", L"Арантир");
				addText(L"С треском по телу голема начинают проходить массивные трещины. Однако через несколько секунд всё  останавливается – хранитель руны всё ещё стоит перед вами. Неудачно сработавшее заклятье не      смогло его разрушить. Монстр с грохотом начинает двигаться в вашу сторону.", L"Повествование");
				addText(L"Проклятье!", L"Арантир");
			}
			else if (_playerInfo.mChosenSolution[1] == 2)
			{
				addText(L"Я уничтожу тебя, порождение Бездны!", L"Арантир");
				addText(L"Хранитель руны без единого звука начинает с грохотом идти в вашу сторону.", L"Повествование");
			}
			else if (_playerInfo.mChosenSolution[1] == 3)
			{
				addText(L"Вы ждёте хоть какой-нибудь реакции от него.", L"Повествование");
				addText(L"Спустя десяток секунд ожидания хранитель руны без единого звука начинает с грохотом идти в вашу   сторону.", L"Повествование");
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
	skipButton->setText(L"Пропустить");
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
