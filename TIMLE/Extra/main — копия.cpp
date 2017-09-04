///главный файл программы
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "Player/view.h"
#include "Interface/mission.h"
#include "Interface/level.h"
#include "Player/lifebar.h"
#include "Interface/menu.h"
#include "entity.h"
#include "Player/player.h"
#include "Enemies/enemy.h"
#include "Enemies/enemy ghost.h"
#include "Enemies/enemy golem.h"
#include "Enemies/enemy darksoldier.h"
#include "Objects/bullet.h"
#include "Objects/moving platform.h"

sf::View view;

///функция смены уровня
void changeLevel(Level &lvl, int &numberLevel)
{
	switch (numberLevel) {
	case 1:
		lvl.LoadFromFile("map.tmx"); //Levels/
		break;
	case 2:
		lvl.LoadFromFile("map2.tmx");
		break;
	case 3:
		lvl.LoadFromFile("map3.tmx");
		break;
	default:
		std::cout << "Error! Out of range level." << std::endl;
		numberLevel = 1;
		break;
	}
}

///игровая функция
bool startGame(sf::RenderWindow &window, int &numberLevel, bool &showMainMenu)
{
	///блок инициализации окна программы
	view.reset(sf::FloatRect(0, 0, 1280, 720));	//инициализация камеры в игре
	if (showMainMenu)
		menu(window);	//вызов меню


	///блок инициализации картинок и текстур для всех объектов
	Level lvl;	//создали экземпляр класса уровень
	changeLevel(lvl, numberLevel);	//для загрузки карты для нужного уровня
	//lvl.LoadFromFile("map.tmx");	//загрузили карту, кнутри класса он с помощью методов её обрабатывает

	sf::Image heroImage;	//картинка героя
	heroImage.loadFromFile("Images/Textures/vanilla.png");
	sf::Image heroDeath;
	heroDeath.loadFromFile("Images/Textures/imp death.png");

	sf::Image ghostEnemyImage;	//картинка призрака
	ghostEnemyImage.loadFromFile("Images/Textures/ghost.png");
	sf::Image ghostEnemyImageDeath;	//картинка смерти призрака
	ghostEnemyImageDeath.loadFromFile("Images/Textures/lightning.png");

	sf::Image golemEnemyImage;	//картинка голема
	golemEnemyImage.loadFromFile("Images/Textures/golem-walk.png");
	sf::Image golemEnemyImageAttack;	//картинка атаки голема
	golemEnemyImageAttack.loadFromFile("Images/Textures/golem-atk.png");
	sf::Image golemEnemyImageDeath;	//картинка смерти голема
	golemEnemyImageDeath.loadFromFile("Images/Textures/golem-die.png");

	sf::Image soldierEnemyImage;	//картинка тёмного солдата
	soldierEnemyImage.loadFromFile("Images/Textures/darksoldiersheetupdate.png");

	sf::Image movePlatformImage;	//картинка платформы
	movePlatformImage.loadFromFile("Images/Textures/MovingPlatform.png");

	sf::Image BulletImage;	//изображение для пули
	BulletImage.loadFromFile("Images/Textures/mage-bullet-13x13.png");	//загрузили картинку в объект изображения
	//BulletImage.createMaskFromColor(sf::Color(0, 0, 0));	//маска для пули по черному цвету


	///блок инициализации объектов
	Object player = lvl.GetObject("player");	//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	Player p(heroImage, heroDeath, "Player1", lvl, player.rect.left, player.rect.top, 30, 61);	///передаем координаты прямоугольника player из карты в координаты нашего игрока 30 41
	LifeBar lifeBarPlayer;	//экземпляр класса полоски здоровья

	std::list<Entity*> entities;	//список, содержащий определённые объекты
	std::list<Entity*>::iterator it;	//итератор, чтобы проходить по элементам списка
	std::list<Entity*>::iterator it2;	//второй итератор для взаимодействия между объектами списка

	std::vector<Object> e = lvl.GetObjects("enemyGhost");	//все объкты врага на tmx-карте будут храниться в векторе
	for (int i = 0; i < e.size(); i++)	//проходимся по элементам вектора с врагами
		entities.push_back(new EnemyGhost(ghostEnemyImage, ghostEnemyImageDeath, "EnemyGhost", lvl, e[i].rect.left, e[i].rect.top, 102, 92));	//добавляем в список всех врагов с карты

	e = lvl.GetObjects("enemyGolem");	//все объкты врага на tmx-карте будут храниться в векторе
	for (int i = 0; i < e.size(); i++)	//проходимся по элементам вектора с врагами
		entities.push_back(new EnemyGolem(golemEnemyImage, golemEnemyImageAttack, golemEnemyImageDeath, "EnemyGolem", lvl, e[i].rect.left, e[i].rect.top, 48, 64));

	e = lvl.GetObjects("enemySoldier");	//все объкты врага на tmx-карте будут храниться в векторе
	for (int i = 0; i < e.size(); i++)	//проходимся по элементам вектора с врагами
		entities.push_back(new EnemyDarkSoldier(soldierEnemyImage, "EnemySoldier", lvl, e[i].rect.left, e[i].rect.top, 40, 60));

	e = lvl.GetObjects("MovingPlatform");	//добавляем все платформы
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new MovingPlatform(movePlatformImage, "MovingPlatform", lvl, e[i].rect.left, e[i].rect.top, 95, 22));	//закидываем платформу в список, передаем изображение имя уровень координаты появления (взяли из tmx карты), а так же размеры


	///блок инициализации звука
	sf::SoundBuffer shootBuffer;	//создаём буфер для звука
	shootBuffer.loadFromFile("Sounds/fairbol3.ogg");	//загружаем в него звук
	sf::Sound shoot(shootBuffer);	//создаем звук и загружаем в него звук из буфера

	sf::Music music;	//создаем объект музыки
	music.openFromFile("Sounds/music.ogg");	//загружаем файл
	music.play();	//воспроизводим музыку
	music.setLoop(true);	//бесконечно воспроизводим музыку


	///блок инициализации меню
	sf::Font font;			//шрифт
	font.loadFromFile("Other/CyrilicOld.ttf");
	sf::Text text("text", font, 20);	//текст с выбранным шрифтом и строкой
	sf::Text textMenu("textMenu", font, 20);
	//text.setColor(sf::Color::Black);

	bool showMissionText = true;		//переменная, отвечающая за появление текста миссии на экране, и флаг нажатия кнопки (tabPress = false;)
	sf::Image quest_image;	//картинка миссий
	quest_image.loadFromFile("Images/Textures/missionbg.jpg");
	quest_image.createMaskFromColor(sf::Color(0, 0, 0));
	sf::Texture quest_texture;	//текстура миссий
	quest_texture.loadFromImage(quest_image);
	sf::Sprite s_quest;		//спрайт миссий
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(sf::IntRect(0, 0, 340, 510));
	s_quest.setScale(0.6f, 0.6f);	//уменьшение размера спрайта


	///блок инициализации таймеров
	sf::Clock clock;	//вспомогательный таймер
	sf::Clock gameTimeClock;	//таймер игрового времени
	int gameTime = 0;	//счётчик игрового времени
	//float currentFrame = 0;	//счётчик для анимации персонажа
	//int createObjectForMapTimer = 0;	//таймер для случайных действий


	///исполняемый блок программы
	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds();	//вспомогательный таймер
		clock.restart();
		time = time / 800;

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			window.setKeyRepeatEnabled(false);	//убираем повторное срабатывание события, если клавиша удерживается

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {	//если backspace, то перезагружаем игру
				showMainMenu = true;
				return true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {	//если эскейп, то выходим из игры
				showMainMenu = false;
				return false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
				lvl.levelNumber++;
				numberLevel++;
				if (numberLevel > 3)
					numberLevel = 1;
				showMainMenu = false;
				return true;
			}

			if (event.type == sf::Event::MouseButtonPressed)	//если нажата клавиша мыши
				if (event.key.code == sf::Mouse::Left) {	//а именно левая, то стреляем и тут же снимаем координаты мыши в этом месте
					sf::Vector2i pixelPos = sf::Mouse::getPosition(window);	//забираем коорд курсора
					sf::Vector2f pos = window.mapPixelToCoords(pixelPos);	//переводим их в игровые (уходим от коорд окна)
					entities.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x + (p.w / 2), p.y, 13, 13, pos.x, pos.y));		//и передаем в конструктор пули. создается пуля
					shoot.play();	//играем звук пули
				}

			if (p.isShoot == true) {	//если выстрелили, то появляется пуля. enum передаем как int 
				p.isShoot = false;
				entities.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x, p.y, 13, 13, p.state));
				shoot.play();	//играем звук пули
			}

			if (event.type == sf::Event::KeyPressed) {	//если нажимаем Tab, появляется меню, нажимаем ещё раз - убирается
				if (event.key.code == sf::Keyboard::Tab) {
					//window.setKeyRepeatEnabled(false);	//убираем повторное срабатывание события, если клавиша удерживается
					switch (showMissionText)
					{
					case true:
						showMissionText = false;
						break;
					case false:
						textMenu.setString("");
						showMissionText = true;
						break;
					}
				}
			}
			window.setKeyRepeatEnabled(true);
		}

		if (p.life) {
			gameTime = gameTimeClock.getElapsedTime().asSeconds();	//время жизни игрока тикает, пока он жив и здоров
		}

		//createObjectForMapTimer += time;	//наращивание таймера
		//if (createObjectForMapTimer > 3000) {
		//	createObjectForMapTimer = 0;	//обнуление таймера
		//}

		///блок "обновления" объектов
		//p.update(time);	//"оживляем" объект с помощью таймера
		//lifeBarPlayer.update(p.health);	//обновляем полосу здоровья
		for (it = entities.begin(); it != entities.end();) {	//проходим по объектам врагов от начала до конца
			Entity *b = *it;	//для упрощения используем другой указатель
			b->update(time);	//вызываем функцию update для объектов
			if (b->life == false) {	//если объект "мёртв"
				it = entities.erase(it);	//удаляем его
				delete b;	//освобождаем память
				//entities.push_back(new Enemy(ghsotEnemyImage, "EnemyGhost", lvl, 900, 500, 102, 90));	//создаём нового врага
			}
			else {
				it++;	//если нет, идём дальше по списку
			}
		}

		p.onPlatform = 0;
		for (it = entities.begin(); it != entities.end(); it++) {	//вновь проходимся по элементам списка
			if (((*it)->name == "MovingPlatform") && ((*it)->getRect().intersects(p.getRect()))) {	//если игрок столкнулся с объектом списка и имя этого объекта movingplatform
				Entity *movPlat = *it;
				if ((p.dy > 0) || (p.onGround == false))	//при этом игрок находится в состоянии после прыжка, т.е падает вниз
					if (p.y + p.h < movPlat->y + movPlat->h) {	//если игрок находится выше платформы, т.е это его ноги минимум (тк мы уже проверяли что он столкнулся с платформой)
						p.y = movPlat->y - p.h + 3;
						p.x += movPlat->dx * time;
						p.dy = 0;
						p.onPlatform = movPlat->dx;
						p.onGround = true;	// то выталкиваем игрока так, чтобы он как бы стоял на платформе
					}
			}

			if ((*it)->getRect().intersects(p.getRect())) {	//если прямоугольник спрайта объекта пересекается с игроком
				if ((*it)->name == "EnemyGhost") {	//и его имя enemyGhost
					if (p.life && (p.health > 0)) {	//если мы в прыжке, то
						if (!(*it)->isBack && (p.dx * (*it)->dx > 0)) {
							(*it)->isBack = true;
							(*it)->dx = -(*it)->dx;
							(*it)->sprite.scale(-1, 1);
						}
						(*it)->isAttacked = true;	//враг останавливается
						(*it)->CurrentFrame = 0;
						if ((*it)->isHitted) {
							p.health -= 7;	//иначе враг наносит урон
							p.dy = -0.1;	//игрок отскакивает от врага
							//p.speed = 0.5 * (*it)->dx;
							(*it)->isBack = false;
							(*it)->isHitted = false;
							std::cout << "Hit\n";
						}
					}
					else {
						//(*it)->isAttacked = false;
					}
				}
				if ((*it)->name == "EnemyGolem") {	//и его имя MediumEnemy
					if (!(*it)->isStarted) {
						(*it)->isStarted = true;
					}
					else if (p.life && (p.health > 0)) {	//если мы живы
						if (!(*it)->isBack && (p.dx * (*it)->dx > 0)) {
							(*it)->isBack = true;
							(*it)->dx = -(*it)->dx;
						}
						(*it)->isAttacked = true;	//враг останавливается
						(*it)->CurrentFrame = 0;
						if ((*it)->isHitted) {
							p.health -= 5;	//иначе враг наносит урон
							//p.dx = 3 * (*it)->dx;
							//p.dy = -0.5;	//игрок отскакивает от врага
							(*it)->isBack = false;
							(*it)->isHitted = false;
							std::cout << "Hit\n";
						}
					}
					else {
						//(*it)->isAttacked = false;
					}
				}
				if ((*it)->name == "EnemySoldier") {	//и его имя MediumEnemy
					if (p.life && (p.health > 0)) {	//если мы живы
						if (!(*it)->isBack && (p.dx * (*it)->dx > 0)) {
							(*it)->isBack = true;
							(*it)->dx =  -(*it)->dx;
						}
						(*it)->isAttacked = true;	//враг останавливается
						(*it)->CurrentFrame = 0;
						if ((*it)->isHitted) {
							p.x = ((*it)->dx > 0? 1.2 : 0.8) * (*it)->x;
							p.y = 0.9 * (*it)->y;	//игрок отскакивает от врага
							p.dy = -0.25;
							//p.health -= 5;	//иначе враг наносит урон
							//p.dx = 3 * (*it)->dx;
							(*it)->isBack = false;
							(*it)->isHitted = false;
							std::cout << "Hit\n";
						}
						//p.x = 0.97 * ((*it)->dx > 0? (*it)->x + 60 : (*it)->x);
						//p.dy = -0.5;	//игрок отскакивает от врага
					}
					else {
						//(*it)->isAttacked = false;
					}
				}
			}

			for (it2 = entities.begin(); it2 != entities.end(); it2++) {
				if ((*it)->getRect() != (*it2)->getRect()) {	//при этом это должны быть разные прямоугольники
					//if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EnemyGhost") && ((*it2)->name == "EnemyGhost")) {	//если столкнулись два объекта и они враги
						//(*it)->dx *= -1;	//меняем направление движения врага
						//(*it)->sprite.scale(-1, 1);	//отражаем спрайт по горизонтали
						//(*it2)->dx *= -1;	//меняем направление движения врага
						//(*it2)->sprite.scale(-1, 1);	//отражаем спрайт по горизонтали
					//}
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EnemyGhost") && ((*it2)->name == "Bullet") && ((*it)->health > 0)) {	//если столкнулись два объекта враг и пуля
						(*it)->health -= 50;	//убиваем врага
						(*it2)->life = false;	//убиваем пулю
					}
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EnemyGolem") && ((*it2)->name == "Bullet") && ((*it)->health > 0) && ((*it)->isStarted)) {	//если столкнулись два объекта враг и пуля
						(*it)->health -= 50;	//убиваем врага
						(*it2)->life = false;	//убиваем пулю
					}
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EnemySoldier") && ((*it2)->name == "Bullet") && ((*it)->health > 0)) {	//если столкнулись два объекта враг и пуля
						(*it)->health -= 1;	//убиваем врага
						(*it2)->life = false;	//убиваем пулю
					}
				}
			}
		}
		lifeBarPlayer.update(p.health);	//обновляем полосу здоровья
		p.update(time);	//"оживляем" объект с помощью таймера
		window.setView(view);	//перемещение камеры
		window.clear(sf::Color(77, 83, 140));	//очистка окна 128, 106, 89
		//window.clear();

		///прорисовка объектов
		lvl.Draw(window);	//рисуем новую карту
		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite);	//рисуем все entities объекты
		}
		window.draw(p.sprite);
		lifeBarPlayer.draw(window);	//рисуем полоску здоровья

		std::ostringstream playerScoreString;	//строка, содержащая текущее количество очков
		playerScoreString << p.playerScore;
		text.setString("Очки: " + playerScoreString.str());	//задаём строку тексту
		text.setPosition(view.getCenter().x + 210, view.getCenter().y - 355);	//задаём позицию выводимому тексту
		window.draw(text);	//прорисовка текста очков

		std::ostringstream playerHealthString, gameTimeString;	//строка вывода количества hp и времени жизни игрока
		if (p.health < 0)
			p.health = 0;
		playerHealthString << p.health;
		gameTimeString << gameTime;
		text.setString("Здоровье: " + playerHealthString.str() + "\nВремя игры: " + gameTimeString.str());	//задаём строку тексту
		text.setPosition(view.getCenter().x - 600, view.getCenter().y - 355);	//задаём позицию выводимому тексту
		window.draw(text);	//прорисовка текста здоровья

		if (!showMissionText) {	//прорисовка текста миссии и меню
			std::ostringstream playerHealthStringMenu, task;
			playerHealthStringMenu << p.health;
			task << getTextMission(getCurrentMission(p.x));
			textMenu.setString("Здоровье: " + playerHealthStringMenu.str() + "\n" + task.str());
			textMenu.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);
			s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);
			window.draw(s_quest);
			window.draw(textMenu);
		}

		window.display();
	}
}


///функция перезагружает игру , если это необходимо
void gameRunning(sf::RenderWindow &window, int &numberLevel, bool &showMainMenu) {
	if (startGame(window, numberLevel, showMainMenu)) {	//принимает с какого уровня начать игру
		gameRunning(window, numberLevel, showMainMenu);
	}
}


///основная часть программы
int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Game", sf::Style::Titlebar | sf::Style::Close);	//инициализация окна
	//sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game", sf::Style::Fullscreen);
	window.setFramerateLimit(60);
	int numberLevel = 1;	//номер текущего уровня
	bool showMainMenu = true;
	gameRunning(window, numberLevel, showMainMenu);	//запускаем процесс игры
	return 0;
}