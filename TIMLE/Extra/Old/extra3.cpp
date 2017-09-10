///главный файл программы
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "map.h"
#include "view.h"
#include "mission.h"
#include "level.h"
#include "lifebar.h"
#include "menu.h"
#include "game.h"


///общий класс объектов
class Entity {
public:
	float dx, dy, x, y, speed, moveTimer;			//ускорение по x и y (dx, dy), координаты, скорость (speed), таймер
	int w, h, health;			//ширина и высота(w, h), количество здоровья
	bool life, isMove, onGround;			//жизнь игрока, разрешение движения, нахождение на земле
	sf::Texture texture;			//текстура объекта
	sf::Sprite sprite;			//спрайт объекта
	sf::String name;			//имя объекта
	std::vector<Object> obj;			//вектор объектов карты
	Entity(sf::Image &image, sf::String Name, float X, float Y, int W, int H)
	{
		x = X; y = Y; w = W; h = H; name = Name; 
		moveTimer = 0; speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = true;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	sf::FloatRect getRect() {			//функция получения прямоугольника. его координат ,размеры (ширина, высота)
		return sf::FloatRect(x, y, w, h);			//функция для проверки столкновений 
	}

	virtual void update(float time) = 0;
 };


///класс игрока
class Player :public Entity {
public:
	int playerScore;			//очки игрока (playerScore)
	enum {left, right, up, down, jump, stay} state;			//тип перечисления - состояние объекта
	bool isShoot;

	Player(sf::Image &image, sf::String Name, Level &lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H)			//конструктор с параметрами для класса
	{
		playerScore = 0;
		state = stay;
		isShoot = false;
		obj = lev.GetAllObjects();			//инициализируем и получаем все объекты для взаимодействия персонажа с картой
		if (name == "Player1") {
			sprite.setTextureRect(sf::IntRect(4, 19, w, h));
		}
	}

	void control()
	{
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) & (onGround)) {
			state = jump;
			dy = -0.6;
			onGround = false;			//прыжок != на земле
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			state = down;
			//speed = 0.1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			state = left;
			speed = 0.1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			state = right;
			speed = 0.1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			isShoot = true;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)			//функция взаимодейтсвия с картой
	{
		for (int i = 0; i<obj.size(); i++)			//проходимся по объектам
			if (getRect().intersects(obj[i].rect))			//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")			//если встретили препятствие
				{
					if (Dy>0) {
						y = obj[i].rect.top - h;  
						dy = 0; 
						onGround = true; 
					}
					if (Dy<0) { 
						y = obj[i].rect.top + obj[i].rect.height;   
						dy = 0; 
					}
					if (Dx>0) { 
						x = obj[i].rect.left - w; 
					}
					if (Dx<0) { 
						x = obj[i].rect.left + obj[i].rect.width; 
					}
				}
			}
			else if (dy > 0) {			// если мы не пересекаемся не с какими прямоугольниками и dy > 0, то onGround = false.
				onGround = false; 
			}
	}

	void update(float time)			//функция "оживления" класса
	{
		control();
		switch (state)			//в зависимости от направления
		{
		case left:			//идёт влево
			dx = -speed;
			break;
		case right:			//идёт вправо
			dx = speed;
			break;
		case up:			//идёт вверх

			break;
		case down:			//идёт вниз
			dx = 0;
			break;
		case jump:			//прыгает

			break;
		case stay:			//стоит на месте

			break;
		}
		x += dx * time;			//смещение координат по времени
		checkCollisionWithMap(dx, 0);			//обрабатываем столкновения по x
		y += dy * time;
		checkCollisionWithMap(0, dy);			//обрабатываем столкновения по y
		if (health <= 0) {
			life = false;
			//sprite.setColor(sf::Color::Red);
		}
		if (!isMove)
			speed = 0;			//обнуляем скорость, чтобы объект остановился
		sprite.setPosition(x + (w / 2), y + (h / 2));			//выводим спрайт в позицию (x, y)
		if (life) {
			setPlayerCoordinateForView(x, y);			//передаём координаты игрока в функцию управления камерой
		}
		dy += 0.0015 * time;			//притяжение к земле
	}
};


///класс врагов
class Enemy :public Entity {
public:
	Enemy(sf::Image &image, sf::String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H)
	{
		obj = lvl.GetObjects("solid");			//инициализируем.получаем нужные объекты для взаимодействия врага с картой
		if (name == "EasyEnemy") {
			sprite.setTextureRect(sf::IntRect(0, 0, w, h));
			dx = 0.1;			//объект всегда имеет скорость
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i<obj.size(); i++)			//проходимся по объектам
			if (getRect().intersects(obj[i].rect))			//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")			//если встретили препятствие
				{
					if (Dy>0) { 
						y = obj[i].rect.top - h;  
						dy = 0; 
						onGround = true; 
					}
					if (Dy<0) { 
						y = obj[i].rect.top + obj[i].rect.height;   
						dy = 0; 
					}
					if (Dx>0) { 
						x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); 
					}
					if (Dx<0) { 
						x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); 
					}
				}
			}
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {
			/*moveTimer += time;
			if (moveTimer > 3000) {
				dx *= -1;
				moveTimer = 0;
			}*/

			checkCollisionWithMap(dx, 0);
			x += dx * time;
			sprite.setPosition(x + (w / 2), y + (h / 2));
			if (health <= 0) {
				life = false;
			}
		}
	}
};


///класс двигающихся платформ
class MovingPlatform : public Entity {
public:
	MovingPlatform(sf::Image &image, sf::String Name, Level &lvl, float X, float Y, int W, int H) : Entity(image, Name, X, Y, W, H)
	{
		sprite.setTextureRect(sf::IntRect(0, 0, W, H));			//прямоугольник
		dx = 0.08;			//изначальное ускорение
	}

	void update(float time)
	{
		x += dx * time;			//движение по горизонтали
		moveTimer += time;			//наращиваем таймер
		if (moveTimer > 2000) {
			dx *= -1;
			moveTimer = 0;		//если прошло 2 секунды, то меняем направление движения платформы на противоположное
		}
		sprite.setPosition(x + (w / 2), y + (h / 2));			//задаём позицию спрайту
	}
};


///класс пуль
class Bullet :public Entity {
public:
	int direction;			//направление пули

	Bullet(sf::Image &image, sf::String Name, Level &lvl, float X, float Y, int W, int H, int dir) :Entity(image, Name, X, Y, W, H)			//всё так же, только взяли в конце состояние игрока (int dir)
	{
		obj = lvl.GetObjects("solid");			//инициализируем, получаем нужные объекты для взаимодействия пули с картой
		x = X;
		y = Y;
		direction = dir;
		speed = 0.8;
		w = h = 16;
		life = true;
		//выше инициализация в конструкторе
	}

	void update(float time)
	{
		switch (direction)
		{
		case 0: 
			dx = -speed; 
			dy = 0;   
			break;			//интовое значение state = left
		case 1: 
			dx = speed; 
			dy = 0;   
			break;			//интовое значение state = right
		case 2: 
			dx = 0; 
			dy = -speed;   
			break;			//интовое значение state = up
		case 3:
			dx = 0; 
			dy = -speed;   
			break;			//интовое значение не имеющее отношения к направлению, стрельнем вверх
		case 4: 
			dx = 0; 
			dy = -speed;   
			break;			//интовое значение не имеющее отношения к направлению, стрельнем вверх
		case 5: 
			dx = 0; 
			dy = -speed;   
			break;			//интовое значение не имеющее отношения к направлению, стрельнем вверх
		}
		x += dx*time;			//само движение пули по х
		y += dy*time;			//по у
		if (x <= 0) 
			x = 1;			// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
		if (y <= 0) 
			y = 1;
		for (int i = 0; i < obj.size(); i++)			//проход по объектам solid
		{
			if (getRect().intersects(obj[i].rect)) {			//если этот объект столкнулся с пулей,
				life = false;			// то пуля умирает
			}
			if (moveTimer > 3000) {
				life = false;
			}
		}
		sprite.setPosition(x + (w / 2), y + (h / 2));			//задается позицию пуле
		moveTimer += time;			//наращиваем таймер
	}
};



void gameRunning()			//ф-ция перезагружает игру , если это необходимо
{
	if (startGame()) {
		gameRunning();
	}			//если startGame() == true, то вызываем занова ф-цию isGameRunning, которая в свою очередь опять вызывает startGame() 
}

///основная часть программы
int main()
{
	/*
	///блок инициализации окна программы
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Game", sf::Style::Default);			//инициализация окна - Fullscreen
	view.reset(sf::FloatRect(0, 0, 1280, 720));			//инициализация камеры в игре
	window.setFramerateLimit(60);
	menu(window);			//вызов меню


	///блок инициализации картинок и текстур для всех объектов
	Level lvl;			//создали экземпляр класса уровень
	lvl.LoadFromFile("map2.tmx");			//загрузили карту, кнутри класса он с помощью методов её обрабатывает

	sf::Image heroImage;			//картинка героя
	heroImage.loadFromFile("Images/Textures/MilesTailsPrower.gif");
	heroImage.createMaskFromColor(sf::Color(0, 0, 0));

	sf::Image easyEnemyImage;			//картинка тупого врага
	easyEnemyImage.loadFromFile("Images/Textures/shamaich.png");
	easyEnemyImage.createMaskFromColor(sf::Color(255, 0, 0));

	sf::Image movePlatformImage;			//картинка платформы
	movePlatformImage.loadFromFile("Images/Textures/MovingPlatform.png");

	sf::Image BulletImage;			//изображение для пули
	BulletImage.loadFromFile("Images/Textures/bullet.png");			//загрузили картинку в объект изображения
	BulletImage.createMaskFromColor(sf::Color(0, 0, 0));			//маска для пули по черному цвету


	///блок инициализации объектов
	Object player = lvl.GetObject("player");			//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 40, 30);			//передаем координаты прямоугольника player из карты в координаты нашего игрока
	LifeBar lifeBarPlayer;			//экземпляр класса полоски здоровья

	std::list<Entity*> entities;			//список, содержащий определённые объекты
	std::list<Entity*>::iterator it;			//итератор, чтобы проходить по элементам списка
	std::list<Entity*>::iterator it2;			//второй итератор для взаимодействия между объектами списка
	
	std::vector<Object> e = lvl.GetObjects("easyEnemy");			//все объкты врага на tmx-карте будут храниться в векторе
	for (int i = 0; i < e.size(); i++)			//проходимся по элементам вектора с врагами
		entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 200, 97));			//добавляем в список всех врагов с карты
	
	e = lvl.GetObjects("MovingPlatform");			//добавляем все платформы
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new MovingPlatform(movePlatformImage, "MovingPlatform", lvl, e[i].rect.left, e[i].rect.top, 95, 22));			//закидываем платформу в список, передаем изображение имя уровень координаты появления (взяли из tmx карты), а так же размеры


	///блок инициализации звука
	sf::SoundBuffer shootBuffer;			//создаём буфер для звука
	shootBuffer.loadFromFile("Sounds/shoot.ogg");			//загружаем в него звук
	sf::Sound shoot(shootBuffer);			//создаем звук и загружаем в него звук из буфера

	sf::Music music;			//создаем объект музыки
	music.openFromFile("Sounds/music.ogg");			//загружаем файл
	music.play();			//воспроизводим музыку
	music.setLoop(true);			//бесконечно воспроизводим музыку


	///блок инициализации меню
	sf::Font font;			//шрифт
	font.loadFromFile("Other/CyrilicOld.ttf");
	sf::Text text("text", font, 20);			//текст с выбранным шрифтом и строкой
	sf::Text textMenu("textMenu", font, 20);
	//text.setColor(sf::Color::Black);

	bool showMissionText = true;			//переменная, отвечающая за появление текста миссии на экране, и флаг нажатия кнопки (tabPress = false;)
	sf::Image quest_image;			//картинка миссий
	quest_image.loadFromFile("Images/Textures/missionbg.jpg");
	quest_image.createMaskFromColor(sf::Color(0, 0, 0));
	sf::Texture quest_texture;			//текстура миссий
	quest_texture.loadFromImage(quest_image);
	sf::Sprite s_quest;				//спрайт миссий
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(sf::IntRect(0, 0, 340, 510));
	s_quest.setScale(0.6f, 0.6f);			//уменьшение размера спрайта


	///блок инициализации таймеров
	sf::Clock clock;			//вспомогательный таймер
	sf::Clock gameTimeClock;			//таймер игрового времени
	int gameTime = 0;			//счётчик игрового времени
	float currentFrame = 0;			//счётчик для анимации персонажа
	int createObjectForMapTimer = 0;			//таймер для случайных действий


	///исполняемый блок программы
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();			//вспомогательный таймер
		clock.restart();
		time = time / 800;
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			window.setKeyRepeatEnabled(false);			//убираем повторное срабатывание события, если клавиша удерживается
			
			if (p.isShoot == true) {			//если выстрелили, то появляется пуля. enum передаем как int 
				p.isShoot = false; 
				entities.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x + (p.w / 2), p.y, 16, 16, p.state));
				shoot.play();			//играем звук пули
			}

			if (event.type == sf::Event::KeyPressed) {			//если нажимаем Tab, появляется меню, нажимаем ещё раз - убирается
				if (event.key.code == sf::Keyboard::Tab) {
					//window.setKeyRepeatEnabled(false);			//убираем повторное срабатывание события, если клавиша удерживается
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
		}

		if (p.life) {
			gameTime = gameTimeClock.getElapsedTime().asSeconds();			//время жизни игрока тикает, пока он жив и здоров
		}

		//createObjectForMapTimer += time;			//наращивание таймера
		//if (createObjectForMapTimer > 3000) {
			//randomMapGenerate();			//генерация объектов
			//createObjectForMapTimer = 0;			//обнуление таймера
		//}

		///блок "обновления" объектов
		p.update(time);			//"оживляем" объект с помощью таймера
		lifeBarPlayer.update(p.health);			//обновляем полосу здоровья
		for (it = entities.begin(); it != entities.end();)			//проходим по объектам врагов от начала до конца
		{
			Entity *b = *it;			//для упрощения используем другой указатель
			b->update(time);			//вызываем функцию update для объектов
			if (b->life == false) {			//если объект "мёртв"
				it = entities.erase(it);			//удаляем его
				delete b;			//освобождаем память
				//entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, 900, 500, 200, 97));			//создаём нового врага
			}
			else {
				it++;			//если нет, идём дальше по списку
			}
		}

		for (it = entities.begin(); it != entities.end(); it++)			//вновь проходимся по элементам списка
		{
			if (((*it)->name == "MovingPlatform") & ((*it)->getRect().intersects(p.getRect()))) {			//если игрок столкнулся с объектом списка и имя этого объекта movingplatform
				Entity *movPlat = *it;
				if ((p.dy > 0) | (p.onGround == false))			//при этом игрок находится в состоянии после прыжка, т.е падает вниз
					if (p.y + p.h<movPlat->y + movPlat->h) {			//если игрок находится выше платформы, т.е это его ноги минимум (тк мы уже проверяли что он столкнулся с платформой)
						p.y = movPlat->y - p.h + 3; 
						p.x += movPlat->dx*time; 
						p.dy = 0; 
						p.onGround = true;			// то выталкиваем игрока так, чтобы он как бы стоял на платформе
					}
			}

			if ((*it)->getRect().intersects(p.getRect())) {			//если прямоугольник спрайта объекта пересекается с игроком
				if ((*it)->name == "EasyEnemy") {			//и его имя EasyEnemy
					if ((p.dy > 0) & (p.onGround == false)) {			//если мы в прыжке, то
						(*it)->dx = 0;			//враг останавливается
						p.dy = -0.2;			//игрок отскакивает от врага
						(*it)->health = 0;			//убиваем врага
					}
					else {
						p.health -= 5;			//иначе враг наносит урон
						p.dy = -0.5;			//игрок отскакивает от врага
					}
				}
			}

			for (it2 = entities.begin(); it2 != entities.end(); it2++) 
			{
				if ((*it)->getRect() != (*it2)->getRect()) {			//при этом это должны быть разные прямоугольники
					if (((*it)->getRect().intersects((*it2)->getRect())) & ((*it)->name == "EasyEnemy") & ((*it2)->name == "EasyEnemy")) {			//если столкнулись два объекта и они враги
						(*it)->dx *= -1;			//меняем направление движения врага
						(*it)->sprite.scale(-1, 1);			//отражаем спрайт по горизонтали
					}
					if (((*it)->getRect().intersects((*it2)->getRect())) & ((*it)->name == "EasyEnemy") & ((*it2)->name == "Bullet")) {			//если столкнулись два объекта и они враги
						(*it)->life = false;
						(*it2)->life = false;
					}
				}
			}
		}

		window.setView(view);			//перемещение камеры
		window.clear(sf::Color(77, 83, 140));			//очистка окна 128, 106, 89
		//window.clear();

		///прорисовка объектов
		lvl.Draw(window);			//рисуем новую карту
		for (it = entities.begin(); it != entities.end(); it++)
			window.draw((*it)->sprite);			//рисуем все entities объекты
		window.draw(p.sprite);
		lifeBarPlayer.draw(window);			//рисуем полоску здоровья

		std::ostringstream playerScoreString;			//строка, содержащая текущее количество очков
		playerScoreString << p.playerScore;
		text.setString("Очки: " + playerScoreString.str());			//задаём строку тексту
		text.setPosition(view.getCenter().x + 210, view.getCenter().y - 355);			//задаём позицию выводимому тексту
		window.draw(text);			//прорисовка текста очков

		std::ostringstream playerHealthString, gameTimeString;			//строка вывода количества hp и времени жизни игрока
		playerHealthString << p.health;
		gameTimeString << gameTime;
		text.setString("Здоровье: " + playerHealthString.str() + "\nВремя игры: " + gameTimeString.str());			//задаём строку тексту
		text.setPosition(view.getCenter().x - 600, view.getCenter().y - 355);			//задаём позицию выводимому тексту
		window.draw(text);			//прорисовка текста здоровья

		if (!showMissionText) {			//прорисовка текста миссии и меню
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
	}*/
	gameRunning();			//запускаем процесс игры
	return 0;
}