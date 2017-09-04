/*#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include "mission.h"
#include "level.h"

//общий класс объектов
class Entity {
public:
	float dx, dy, x, y, speed, moveTimer;			//ускорение по x и y (dx, dy), координаты, скорость (speed)
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

	sf::FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return sf::FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}

	virtual void update(float time) = 0;
 };

//класс игрока
class Player :public Entity {
//private:
public:
	int playerScore;			//очки игрока (playerScore)
	enum {left, right, up, down, jump, stay} state;			//тип перечисления - состояние объекта

	Player(sf::Image &image, sf::String Name, Level &lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H)			//конструктор с параметрами для класса
	{
		playerScore = 0;
		state = stay;
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
	}

	void checkCollisionWithMap(float Dx, float Dy)			//функция взаимодейтсвия с картой
	{
		for (int i = y / 32; i < (y + h) / 32; i++)			//проходимся по всем квадратам, контактирующих с игроком
			for (int j = x / 32; j < (x + w) / 32; j++)			//проходим по всем квадратам слево направо, с которыми взаимодействует персонаж
			{
				if (TileMap[i][j] == '0') {			//если персонаж приблизился к стене, то в зависимости от направления:
					if (Dy > 0) {			//если персонаж двигался вниз,
						y = i * 32 - h;			//то останавливаем его, т.е. получаем текущий квадрат и, при попытке пойти вниз, телепортируем его в этот квадрат
						dy = 0;
						onGround = true;
					}
					if (Dy < 0) {			//движение вниз аналогично движению вверх
						y = i * 32 + 32;
						dy = 0;
					}
					if (Dx > 0) {			//если персонаж двигался вправо, то останавливаем его аналогично движению вверх/вниз
						x = j * 32 - w;
					}
					if (Dx < 0) {			//движение влево аналогично движению вправо
						x = j * 32 + 32;
					}
				}
				if (TileMap[i][j] == 's') {			//если персонаж приблизился к камню:
				playerScore++;			//увеличиваем очки игрока
				TileMap[i][j] = ' ';			//и убираем камень
				}
				if (TileMap[i][j] == 'f') {			//если персонаж приблизился к цветку
				health -= 40;			//отнимаем 40 hp
				TileMap[i][j] = ' ';			//и убираем цветок
				}
				if (TileMap[i][j] == 'h') {			//если персонаж приблизился к сердцу
				health += 20;			//добавляем 20 hp
				TileMap[i][j] = ' ';			//и убираем сердце
				}
			}

		for (int i = 0; i<obj.size(); i++)			//проходимся по объектам
			if (getRect().intersects(obj[i].rect))			//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")			//если встретили препятствие
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
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


//класс врагов
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
		for (int i = y / 32; i < (y + h) / 32; i++)			//проходимся по всем квадратам, контактирующих с игроком
			for (int j = x / 32; j < (x + w) / 32; j++)			//проходим по всем квадратам слево направо, с которыми взаимодействует персонаж
			{
				if (TileMap[i][j] == '0') {			//если персонаж приблизился к стене, то в зависимости от направления:
					if (Dy > 0) {			//если персонаж двигался вниз,
						y = i * 32 - h;			//то останавливаем его, т.е. получаем текущий квадрат и, при попытке пойти вниз, телепортируем его в этот квадрат
						dy = 0;
						onGround = true;
					}
					if (Dy < 0) {			//движение вниз аналогично движению вверх
						y = i * 32 + 32;
						dy = 0;
					}
					if (Dx > 0) {			//если персонаж двигался вправо, то останавливаем его аналогично движению вверх/вниз
						x = j * 32 - w;
						dx = -0.1;
						sprite.scale(-1, 1);
					}
					if (Dx < 0) {			//движение влево аналогично движению вправо
						x = j * 32 + 32;
						dx = 0.1;
						sprite.scale(-1, 1);
					}
				}
			}

		for (int i = 0; i<obj.size(); i++)			//проходимся по объектам
			if (getRect().intersects(obj[i].rect))			//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")			//если встретили препятствие
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
				}
			}
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {
			moveTimer += time;
			if (moveTimer > 3000) {
				dx *= -1;
				moveTimer = 0;
			}

			checkCollisionWithMap(dx, 0);
			x += dx * time;
			sprite.setPosition(x + (w / 2), y + (h / 2));
			if (health <= 0) {
				life = false;
			}
		}
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Game", sf::Style::Default);			//инициализация окна - Fullscreen
	view.reset(sf::FloatRect(0, 0, 1280, 720));			//инициализация камеры в игре
	window.setFramerateLimit(60);

	Level lvl;			//создали экземпляр класса уровень
	lvl.LoadFromFile("map.tmx");			//загрузили карту, кнутри класса он с помощью методов её обрабатывает
	sf::Image heroImage;			//картинка героя
	heroImage.loadFromFile("Images/MilesTailsPrower.gif");
	heroImage.createMaskFromColor(sf::Color(0, 0, 0));
	sf::Image easyEnemyImage;			//картинка тупого врага
	easyEnemyImage.loadFromFile("Images/shamaich.png");
	easyEnemyImage.createMaskFromColor(sf::Color(255, 0, 0));

	Object player = lvl.GetObject("player");			//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	//Object easyEnemyObject = lvl.GetObject("easyEnemy");			//объект легкого врага на нашей карте.задаем координаты игроку в начале при помощи него
	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 40, 30);			//передаем координаты прямоугольника player из карты в координаты нашего игрока
	//Enemy easyEnemy(easyEnemyImage, "EasyEnemy", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 200, 97);			//передаем координаты прямоугольника easyEnemy из карты в координаты нашего врага

	sf::Image map_image;			//картинка карты
	map_image.loadFromFile("Images/map.png");
	sf::Texture map;			//текстуры карты
	map.loadFromImage(map_image);
	sf::Sprite s_map;			//спрайт карты
	s_map.setTexture(map);

	sf::Font font;			//шрифт
	font.loadFromFile("Other/CyrilicOld.ttf");
	sf::Text text("text", font, 20);			//текст с выбранным шрифтом и строкой
	sf::Text textMenu("textMenu", font, 20);
	//text.setColor(sf::Color::Black);

	sf::Clock clock;			//вспомогательный таймер
	sf::Clock gameTimeClock;			//таймер игрового времени
	int gameTime = 0;			//счётчик игрового времени
	float currentFrame = 0;			//счётчик для анимации персонажа
	int createObjectForMapTimer = 0;			//таймер для случайных действий
	
	bool showMissionText = true;			//переменная, отвечающая за появление текста миссии на экране, и флаг нажатия кнопки (tabPress = false;)
	sf::Image quest_image;			//картинка миссий
	quest_image.loadFromFile("Images/missionbg.jpg");
	quest_image.createMaskFromColor(sf::Color(0, 0, 0));
	sf::Texture quest_texture;			//текстура миссий
	quest_texture.loadFromImage(quest_image);
	sf::Sprite s_quest;				//спрайт миссий
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(sf::IntRect(0, 0, 340, 510));
	s_quest.setScale(0.6f, 0.6f);			//уменьшение размера спрайта

	std::list<Entity*> entities;			//список, содержащий определённые объекты
	std::list<Entity*>::iterator it;			//итератор, чтобы проходить по элементам списка
	std::vector<Object> e = lvl.GetObjects("easyEnemy");			//все объкты врага на tmx-карте будут храниться в векторе
	for (int i = 0; i < e.size(); i++)			//проходимся по элементам вектора с врагами
		entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 200, 97));			//добавляем в список всех врагов с карты

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

			if (event.type == sf::Event::KeyPressed) {			//если нажимаем Tab, появляется меню, нажимаем ещё раз - убирается
				if (event.key.code == sf::Keyboard::Tab) {
					window.setKeyRepeatEnabled(false);			//убираем повторное срабатывание события, если клавиша удерживается
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

		createObjectForMapTimer += time;			//наращивание таймера
		if (createObjectForMapTimer > 3000) {
			randomMapGenerate();			//генерация объектов
			createObjectForMapTimer = 0;			//обнуление таймера
		}

		p.update(time);			//"оживляем" объект с помощью таймера
		//easyEnemy.update(time);
		for (it = entities.begin(); it != entities.end(); it++)
		{
			(*it)->update(time);			//для всех элементов списка активируем функцию update
		}
		window.setView(view);			//перемещение камеры
		window.clear(sf::Color(77, 83, 140));			//очистка окна 128, 106, 89
		//window.clear();

		//старая прорисовка карты
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')
					s_map.setTextureRect(sf::IntRect(0, 0, 32, 32));			//обычный блок
				if (TileMap[i][j] == 's')
					s_map.setTextureRect(sf::IntRect(32, 0, 32, 32));			//камень
				if (TileMap[i][j] == '0')
					s_map.setTextureRect(sf::IntRect(64, 0, 32, 32));			//стена
				if (TileMap[i][j] == 'f')
					s_map.setTextureRect(sf::IntRect(96, 0, 32, 32));			//цветок
				if (TileMap[i][j] == 'h')
					s_map.setTextureRect(sf::IntRect(128, 0, 32, 32));			//сердце
				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
		
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

		std::ostringstream playerScoreString;			//строка, содержащая текущее количество очков
		playerScoreString << p.playerScore;
		text.setString("Собрано камней: " + playerScoreString.str());			//задаём строку тексту
		text.setPosition(view.getCenter().x + 210, view.getCenter().y - 360);			//задаём позицию выводимому тексту
		window.draw(text);			//прорисовка текста очков

		std::ostringstream playerHealthString, gameTimeString;			//строка вывода количества hp и времени жизни игрока
		playerHealthString << p.health;
		gameTimeString << gameTime;
		text.setString("Здоровье: " + playerHealthString.str() + "\nВремя игры: " + gameTimeString.str());			//задаём строку тексту
		text.setPosition(view.getCenter().x - 630, view.getCenter().y - 360);			//задаём позицию выводимому тексту
		window.draw(text);			//прорисовка текста здоровья

		//прорисовка объектов
		lvl.Draw(window);			//рисуем новую карту
		//window.draw(easyEnemy.sprite);
		for (it = entities.begin(); it != entities.end(); it++)
			window.draw((*it)->sprite);			//рисуем все entities объекты
		window.draw(p.sprite);
		window.display();
	}
	return 0;
}*/