/*#include <iostream>
#include <sstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include "mission.h"

class Player {
	//private:
	//float x, y;			//координаты игрока(x, y)
public:
	float w, h, dx, dy, speed, x, y;			//ширина и высота (w, h), ускорение по x и y (dx, dy), скорость (speed)
	int dir, playerScore, health;			//направление (direction) движения игрока, очки игрока (playerScore), количество здоровья
	bool life, isMove, isSelect;			//жизнь игрока, разрешение движения, выбор объекта
	sf::String File;			//файл с расширением
	sf::Image image;			//картинка игрока
	sf::Texture texture;			//текстура игрока
	sf::Sprite sprite;			//спрайт игрока

	Player(sf::String F, float X, float Y, float W, float H)			//конструктор с параметрами для класса
	{
		dx = 0; dy = 0; speed = 0; dir = 0; speed = 0; playerScore = 0; health = 100;
		life = true; isMove = false; isSelect = false;
		File = F;			//имя файла и расширение
		w = W; h = H;
		image.loadFromFile("Images/" + File);
		//image.createMaskFromColor(sf::Color(41, 33, 59));
		image.createMaskFromColor(sf::Color(0, 0, 255));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;			//координаты появления спрайта
		sprite.setTextureRect(sf::IntRect(0, 0, w, h));			//появляется только одна картинка в спрайте
		sprite.setOrigin(w / 2, h / 2);			//центр спрайта переносим с верхнего девого угла в центр
	}

	void update(float time)			//функция "оживления" класса
	{
		switch (dir)			//в зависимости от направления
		{
		case 0:			//идёт только влево
			dx = speed;
			dy = 0;
			break;
		case 1:			//идёт только вправо
			dx = -speed;
			dy = 0;
			break;
		case 2:			//идёт только вниз
			dx = 0;
			dy = speed;
			break;
		case 3:			//идёт только вверх
			dx = 0;
			dy = -speed;
			break;
		}
		x += dx * time;			//смещение координат по времени
		y += dy * time;
		speed = 0;			//обнуляем скорость, чтобы объект остановился
		sprite.setPosition(x, y);			//выводим спрайт в позицию (x, y)
		interactionWithMap();			//взаимодействие с картой
		if (health <= 0) {
			life = false;
			speed = 0;
			sprite.setColor(sf::Color::Red);
		}
	}

	void interactionWithMap()			//функция взаимодейтсвия с картой
	{
		for (int i = y / 32; i < (y + h) / 32; i++)			//проходимся по всем квадратам, контактирующих с игроком
			for (int j = x / 32; j < (x + w) / 32; j++)			//проходим по всем квадратам слево направо, с которыми взаимодействует персонаж
			{
				if (TileMap[i][j] == '0') {			//если персонаж приблизился к стене, то в зависимости от направления:
					if (dy > 0) {			//если персонаж двигался вниз,
						y = i * 32 - h;			//то останавливаем его, т.е. получаем текущий квадрат и, при попытке пойти вниз, телепортируем его в этот квадрат
					}
					if (dy < 0) {			//аналогично движению вверх
						y = i * 32 + 32;
					}
					if (dx > 0) {			//если персонаж двигался вправо, то останавливаем его аналогично движению вверх/вниз
						x = j * 32 - w;
					}
					if (dx < 0) {			//аналогично движению вправо
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
	}

	float getPlayerCoordinateX()
	{
		return x;
	}

	float getPlayerCoordinateY()
	{
		return y;
	}

	float getWidth()
	{
		return w;
	}

	void setWidth(float width)
	{
		w = width;
	}

	float getHeight()
	{
		return h;
	}

	void setHeight(float height)
	{
		h = height;
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Game", sf::Style::Default);			//инициализация окна - Fullscreen
	view.reset(sf::FloatRect(0, 0, 1280, 720));			//инициализация камеры в игре
	window.setFramerateLimit(60);

	Player p("heroForRotate.png", 250, 250, 136, 74);			//создаём объект класса Player (250, 250, 96.0, 96.0)
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
	//text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	sf::Clock clock;			//вспомогательный таймер
	sf::Clock gameTimeClock;			//таймер игрового времени
	int gameTime = 0;			//счётчик игрового времени
	float CurrentFrame = 0;			//счётчик для анимации персонажа
	int createObjectForMapTimer = 0;			//таймер для случайных действий
	int buttonTimer = 0;			//таймер нажатия кнопки

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

	bool isMove = false;			//переменная для щелчка мыши по спрайту
	float dX = 0, dY = 0;			//переменная для корректировки координат нажатия по x и y

	int tempX = 0, tempY = 0;			//временные координаты x и y, появляются после нажатия кнопки мыши
	float distance = 0;			//расстояние от объекта до курсора

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();			//вспомогательный таймер
		clock.restart();
		time = time / 800;

		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);			//получаем координаты курсора
		sf::Vector2f pos = window.mapPixelToCoords(pixelPos);			//переводим координаты курсора в игровые координаты

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

			if (event.type == sf::Event::MouseButtonPressed)			//если нажата левая кнопка мыши
				if (event.key.code == sf::Mouse::Left)
					if (p.sprite.getGlobalBounds().contains(pos.x, pos.y)) {			//и координата курсора попадает в спрайт
						p.sprite.setColor(sf::Color::Green);			//окрашиваем спрайт в зелёный цвет
						p.isSelect = true;			//объект считается выбранным
					}

			if (p.isSelect)
				if (event.type == sf::Event::MouseButtonPressed)			//если нажата левая кнопка мыши
					if (event.key.code == sf::Mouse::Right) {
						p.isMove = true;			//разрешаем движение спрайта
						p.isSelect = false;			//объект считается невыбранным
						p.sprite.setColor(sf::Color::White);			//возвращаем обычный цвет спрайту
						tempX = pos.x;			//считываем координаты нажатия курсора
						tempY = pos.y;
						float dX1 = pos.x - p.x;			//вектор, коллинеарный прямой, которая пересекает спрайт и курсор
						float dY1 = pos.y - p.y;			//он же и координата y
						float rotation = (atan2(dY1, dX1)) * 180 / 3.14159265;			//получаем угол в радинанах и переводим его в градусы
						p.sprite.setRotation(rotation);
					}

			if (event.type == sf::Event::MouseButtonPressed)			//если нажата левая кнопка мыши
			if (event.key.code == sf::Mouse::Left)
			if (p.sprite.getGlobalBounds().contains(pos.x, pos.y)) {			//и координата курсора попадает в спрайт
			dX = pos.x - p.sprite.getPosition().x;			//отнимаем от координат курсора координаты спрайта для корректироки нажатия
			dY = pos.y - p.sprite.getPosition().y;
			isMove = true;			//разрешаем двигать спрайт
			}

			if (event.type == sf::Event::MouseButtonReleased)			//если левая кнопка мыши отпущена
			if (event.key.code == sf::Mouse::Left) {
			isMove = false;			//запрещаем двигать спрайт
			p.sprite.setColor(sf::Color::White);			//возвращаем прежний цвет
			}
		}

		if (p.isMove) {
			distance = sqrt((tempX - p.x) * (tempX - p.x) + (tempY - p.y) * (tempY - p.y));			//считаем дистанцию по формуле длины вектора
			if (distance > 2) {			//убираем дёргание во время конечной позиции спрайта
				p.x += 0.1 * time * (tempX - p.x) / distance;			//перемещаем спрайт по x вектора нормали
				p.y += 0.1 * time * (tempY - p.y) / distance;			//перемещаем спрайт по y вектора нормали
			}
			else {
				p.isMove = false;			//запрещаем движение спрайта
			}
		}

		if (isMove) {			//если разрешено двигать спрайт
		p.sprite.setColor(sf::Color::Green);			//перекрашиваем спрайт в зелёный цвет
		p.x = pos.x - dX;
		p.y = pos.y - dY;
		//p.sprite.setPosition(pos.x - dX, pos.y - dY);			//изменяем позицию спрайта
		}

		if (p.life) {
			gameTime = gameTimeClock.getElapsedTime().asSeconds();			//время жизни игрока тикает, пока он жив и здоров
		}
		else {
			view.rotate(0.01);
		}

		//управление персонажем с анимацией
		if (p.life) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				p.dir = 1;
				p.speed = 0.1;
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) {
					CurrentFrame -= 3;
				}
				p.sprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 96, 96, 96));			//96*r., 96, 96, 96 | 96*r + 6, 136, 89, 55
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				p.dir = 0;
				p.speed = 0.1;
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) {
					CurrentFrame -= 3;
				}
				p.sprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 192, 96, 96));			//96*r., 192, 96, 96 | 96*r + 6, 231, 89, 55
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				p.dir = 3;
				p.speed = 0.1;
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) {
					CurrentFrame -= 3;
				}
				p.sprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 307, 96, 96));			//96*r., 288, 96, 96 | 96*r + 27, 305, 39, 89
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				p.dir = 2;
				p.speed = 0.1;
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) {
					CurrentFrame -= 3;
				}
				p.sprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 0, 96, 96));			//96*r., 0, 96, 96 | 96*r + 27, 6, 39, 89
			}
			getPlayerCoordinateForView(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());			//передаём координаты игрока в функцию управления камерой
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		p.sprite.setColor(sf::Color::Red);
		}

		sf::Vector2i localPosition = sf::Mouse::getPosition(window);			//вектор, отслеживающий координаты курсора мыши
		if (localPosition.x < 3) {			//если курсор в левом крае окна, то двигаем камеру влево
			view.move(-0.2 * time, 0);
		}
		if (localPosition.x > window.getSize().x - 3) {			//если курсор в правом крае окна, то двигаем камеру вправо
			view.move(0.2 * time, 0);
		}
		if (localPosition.y > window.getSize().y - 3) {			//если курсор в нижнем крае окна, то двигаем камеру вниз
			view.move(0, 0.2 * time);
		}
		if (localPosition.y < 3) {			//если курсор в верхнем крае окна, то двигаем камеру вверх
			view.move(0, -0.2 * time);
		}

		createObjectForMapTimer += time;			//наращивание таймера
		if (createObjectForMapTimer > 3000) {
			randomMapGenerate();			//генерация объектов
			createObjectForMapTimer = 0;			//обнуление таймера
		}

		p.update(time);			//"оживляем" объект с помощью таймера
		viewMap(time);			//функция перемещения камеры над картой
		changeView();
		window.setView(view);			//перемещение камеры

		window.clear(sf::Color(128, 106, 89));			//очистка окна
														//window.clear();

		window.getSystemHandle();

		//прорисовка карты
		if (getCurrentMission(p.getPlayerCoordinateX()) == 0) {
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
		}
		else if (getCurrentMission(p.getPlayerCoordinateX()) == 1) {
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
				{
					if (TileMap[i][j] == ' ')
						s_map.setTextureRect(sf::IntRect(64, 0, 32, 32));			//обычный блок
					if (TileMap[i][j] == 's')
						s_map.setTextureRect(sf::IntRect(32, 0, 32, 32));			//камень
					if (TileMap[i][j] == '0')
						s_map.setTextureRect(sf::IntRect(0, 0, 32, 32));			//стена
					if (TileMap[i][j] == 'f')
						s_map.setTextureRect(sf::IntRect(96, 0, 32, 32));			//цветок
					if (TileMap[i][j] == 'h')
						s_map.setTextureRect(sf::IntRect(128, 0, 32, 32));			//сердце
					s_map.setPosition(j * 32, i * 32);
					window.draw(s_map);
				}
		}

		if (!showMissionText) {			//прорисовка текста миссии и меню
			std::ostringstream playerHealthStringMenu, task;
			playerHealthStringMenu << p.health;
			task << getTextMission(getCurrentMission(p.getPlayerCoordinateX()));
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

									//прорисовка игрока
		window.draw(p.sprite);

		window.display();
	}

	return 0;
}*/

//extra cod from view.h
/*void viewMap(float time)			//функция для перемещения камеры по карте по времени
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		view.move(-0.1 * time, 0);			//перемещаем камеру над картой влево
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		view.move(0.1 * time, 0);			//перемещаем камеру над картой вправо
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		view.move(0, -0.1 * time);			//перемещаем камеру над картой вверх
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		view.move(0, 0.1 * time);			//перемещаем камеру над картой вниз
	}
}

void changeView()			//функция, изменяющая параметры камеры
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		view.zoom(1.0100f);			//масштабируем, уменьшаем
		//view.zoom(1.0006f);			//масштабируем медленнее
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		//view.setRotation(90);		//задаём угол поворота камеры
		view.rotate(1);			//постепенно поворачиваем камеру
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
		view.setSize(800, 640);			//устанавливаем исходный размер камеры
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		view.setSize(640, 480);			//устанавливаем меньший размер камеры
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		view.setViewport(sf::FloatRect(0, 0, 0.5f, 1));			//создание раздельного экрана
	}
}*/