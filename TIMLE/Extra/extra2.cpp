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

//����� ����� ��������
class Entity {
public:
	float dx, dy, x, y, speed, moveTimer;			//��������� �� x � y (dx, dy), ����������, �������� (speed)
	int w, h, health;			//������ � ������(w, h), ���������� ��������
	bool life, isMove, onGround;			//����� ������, ���������� ��������, ���������� �� �����
	sf::Texture texture;			//�������� �������
	sf::Sprite sprite;			//������ �������
	sf::String name;			//��� �������
	std::vector<Object> obj;			//������ �������� �����
	Entity(sf::Image &image, sf::String Name, float X, float Y, int W, int H)
	{
		x = X; y = Y; w = W; h = H; name = Name; 
		moveTimer = 0; speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = true;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	sf::FloatRect getRect() {//�-��� ��������� ��������������. ��� �����,������� (���,�����).
		return sf::FloatRect(x, y, w, h);//��� �-��� ����� ��� �������� ������������ 
	}

	virtual void update(float time) = 0;
 };

//����� ������
class Player :public Entity {
//private:
public:
	int playerScore;			//���� ������ (playerScore)
	enum {left, right, up, down, jump, stay} state;			//��� ������������ - ��������� �������

	Player(sf::Image &image, sf::String Name, Level &lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H)			//����������� � ����������� ��� ������
	{
		playerScore = 0;
		state = stay;
		obj = lev.GetAllObjects();			//�������������� � �������� ��� ������� ��� �������������� ��������� � ������
		if (name == "Player1") {
			sprite.setTextureRect(sf::IntRect(4, 19, w, h));
		}
	}

	void control()
	{
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) & (onGround)) {
			state = jump;
			dy = -0.6;
			onGround = false;			//������ != �� �����
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

	void checkCollisionWithMap(float Dx, float Dy)			//������� �������������� � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)			//���������� �� ���� ���������, �������������� � �������
			for (int j = x / 32; j < (x + w) / 32; j++)			//�������� �� ���� ��������� ����� �������, � �������� ��������������� ��������
			{
				if (TileMap[i][j] == '0') {			//���� �������� ����������� � �����, �� � ����������� �� �����������:
					if (Dy > 0) {			//���� �������� �������� ����,
						y = i * 32 - h;			//�� ������������� ���, �.�. �������� ������� ������� �, ��� ������� ����� ����, ������������� ��� � ���� �������
						dy = 0;
						onGround = true;
					}
					if (Dy < 0) {			//�������� ���� ���������� �������� �����
						y = i * 32 + 32;
						dy = 0;
					}
					if (Dx > 0) {			//���� �������� �������� ������, �� ������������� ��� ���������� �������� �����/����
						x = j * 32 - w;
					}
					if (Dx < 0) {			//�������� ����� ���������� �������� ������
						x = j * 32 + 32;
					}
				}
				if (TileMap[i][j] == 's') {			//���� �������� ����������� � �����:
				playerScore++;			//����������� ���� ������
				TileMap[i][j] = ' ';			//� ������� ������
				}
				if (TileMap[i][j] == 'f') {			//���� �������� ����������� � ������
				health -= 40;			//�������� 40 hp
				TileMap[i][j] = ' ';			//� ������� ������
				}
				if (TileMap[i][j] == 'h') {			//���� �������� ����������� � ������
				health += 20;			//��������� 20 hp
				TileMap[i][j] = ' ';			//� ������� ������
				}
			}

		for (int i = 0; i<obj.size(); i++)			//���������� �� ��������
			if (getRect().intersects(obj[i].rect))			//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")			//���� ��������� �����������
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}

	void update(float time)			//������� "���������" ������
	{
		control();
		switch (state)			//� ����������� �� �����������
		{
		case left:			//��� �����
			dx = -speed;
			break;
		case right:			//��� ������
			dx = speed;
			break;
		case up:			//��� �����

			break;
		case down:			//��� ����
			dx = 0;
			break;
		case jump:			//�������

			break;
		case stay:			//����� �� �����

			break;
		}
		x += dx * time;			//�������� ��������� �� �������
		checkCollisionWithMap(dx, 0);			//������������ ������������ �� x
		y += dy * time;
		checkCollisionWithMap(0, dy);			//������������ ������������ �� y
		if (health <= 0) {
			life = false;
			//sprite.setColor(sf::Color::Red);
		}
		if (!isMove)
			speed = 0;			//�������� ��������, ����� ������ �����������
		sprite.setPosition(x + (w / 2), y + (h / 2));			//������� ������ � ������� (x, y)
		if (life) {
			setPlayerCoordinateForView(x, y);			//������� ���������� ������ � ������� ���������� �������
		}
		dy += 0.0015 * time;			//���������� � �����
	}
};


//����� ������
class Enemy :public Entity {
public:
	Enemy(sf::Image &image, sf::String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H)
	{
		obj = lvl.GetObjects("solid");			//��������������.�������� ������ ������� ��� �������������� ����� � ������
		if (name == "EasyEnemy") {
			sprite.setTextureRect(sf::IntRect(0, 0, w, h));
			dx = 0.1;			//������ ������ ����� ��������
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = y / 32; i < (y + h) / 32; i++)			//���������� �� ���� ���������, �������������� � �������
			for (int j = x / 32; j < (x + w) / 32; j++)			//�������� �� ���� ��������� ����� �������, � �������� ��������������� ��������
			{
				if (TileMap[i][j] == '0') {			//���� �������� ����������� � �����, �� � ����������� �� �����������:
					if (Dy > 0) {			//���� �������� �������� ����,
						y = i * 32 - h;			//�� ������������� ���, �.�. �������� ������� ������� �, ��� ������� ����� ����, ������������� ��� � ���� �������
						dy = 0;
						onGround = true;
					}
					if (Dy < 0) {			//�������� ���� ���������� �������� �����
						y = i * 32 + 32;
						dy = 0;
					}
					if (Dx > 0) {			//���� �������� �������� ������, �� ������������� ��� ���������� �������� �����/����
						x = j * 32 - w;
						dx = -0.1;
						sprite.scale(-1, 1);
					}
					if (Dx < 0) {			//�������� ����� ���������� �������� ������
						x = j * 32 + 32;
						dx = 0.1;
						sprite.scale(-1, 1);
					}
				}
			}

		for (int i = 0; i<obj.size(); i++)			//���������� �� ��������
			if (getRect().intersects(obj[i].rect))			//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")			//���� ��������� �����������
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
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Game", sf::Style::Default);			//������������� ���� - Fullscreen
	view.reset(sf::FloatRect(0, 0, 1280, 720));			//������������� ������ � ����
	window.setFramerateLimit(60);

	Level lvl;			//������� ��������� ������ �������
	lvl.LoadFromFile("map.tmx");			//��������� �����, ������ ������ �� � ������� ������� � ������������
	sf::Image heroImage;			//�������� �����
	heroImage.loadFromFile("Images/MilesTailsPrower.gif");
	heroImage.createMaskFromColor(sf::Color(0, 0, 0));
	sf::Image easyEnemyImage;			//�������� ������ �����
	easyEnemyImage.loadFromFile("Images/shamaich.png");
	easyEnemyImage.createMaskFromColor(sf::Color(255, 0, 0));

	Object player = lvl.GetObject("player");			//������ ������ �� ����� �����.������ ���������� ������ � ������ ��� ������ ����
	//Object easyEnemyObject = lvl.GetObject("easyEnemy");			//������ ������� ����� �� ����� �����.������ ���������� ������ � ������ ��� ������ ����
	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 40, 30);			//�������� ���������� �������������� player �� ����� � ���������� ������ ������
	//Enemy easyEnemy(easyEnemyImage, "EasyEnemy", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 200, 97);			//�������� ���������� �������������� easyEnemy �� ����� � ���������� ������ �����

	sf::Image map_image;			//�������� �����
	map_image.loadFromFile("Images/map.png");
	sf::Texture map;			//�������� �����
	map.loadFromImage(map_image);
	sf::Sprite s_map;			//������ �����
	s_map.setTexture(map);

	sf::Font font;			//�����
	font.loadFromFile("Other/CyrilicOld.ttf");
	sf::Text text("text", font, 20);			//����� � ��������� ������� � �������
	sf::Text textMenu("textMenu", font, 20);
	//text.setColor(sf::Color::Black);

	sf::Clock clock;			//��������������� ������
	sf::Clock gameTimeClock;			//������ �������� �������
	int gameTime = 0;			//������� �������� �������
	float currentFrame = 0;			//������� ��� �������� ���������
	int createObjectForMapTimer = 0;			//������ ��� ��������� ��������
	
	bool showMissionText = true;			//����������, ���������� �� ��������� ������ ������ �� ������, � ���� ������� ������ (tabPress = false;)
	sf::Image quest_image;			//�������� ������
	quest_image.loadFromFile("Images/missionbg.jpg");
	quest_image.createMaskFromColor(sf::Color(0, 0, 0));
	sf::Texture quest_texture;			//�������� ������
	quest_texture.loadFromImage(quest_image);
	sf::Sprite s_quest;				//������ ������
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(sf::IntRect(0, 0, 340, 510));
	s_quest.setScale(0.6f, 0.6f);			//���������� ������� �������

	std::list<Entity*> entities;			//������, ���������� ����������� �������
	std::list<Entity*>::iterator it;			//��������, ����� ��������� �� ��������� ������
	std::vector<Object> e = lvl.GetObjects("easyEnemy");			//��� ������ ����� �� tmx-����� ����� ��������� � �������
	for (int i = 0; i < e.size(); i++)			//���������� �� ��������� ������� � �������
		entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 200, 97));			//��������� � ������ ���� ������ � �����

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();			//��������������� ������
		clock.restart();
		time = time / 800;
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed) {			//���� �������� Tab, ���������� ����, �������� ��� ��� - ���������
				if (event.key.code == sf::Keyboard::Tab) {
					window.setKeyRepeatEnabled(false);			//������� ��������� ������������ �������, ���� ������� ������������
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
			gameTime = gameTimeClock.getElapsedTime().asSeconds();			//����� ����� ������ ������, ���� �� ��� � ������
		}

		createObjectForMapTimer += time;			//����������� �������
		if (createObjectForMapTimer > 3000) {
			randomMapGenerate();			//��������� ��������
			createObjectForMapTimer = 0;			//��������� �������
		}

		p.update(time);			//"��������" ������ � ������� �������
		//easyEnemy.update(time);
		for (it = entities.begin(); it != entities.end(); it++)
		{
			(*it)->update(time);			//��� ���� ��������� ������ ���������� ������� update
		}
		window.setView(view);			//����������� ������
		window.clear(sf::Color(77, 83, 140));			//������� ���� 128, 106, 89
		//window.clear();

		//������ ���������� �����
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')
					s_map.setTextureRect(sf::IntRect(0, 0, 32, 32));			//������� ����
				if (TileMap[i][j] == 's')
					s_map.setTextureRect(sf::IntRect(32, 0, 32, 32));			//������
				if (TileMap[i][j] == '0')
					s_map.setTextureRect(sf::IntRect(64, 0, 32, 32));			//�����
				if (TileMap[i][j] == 'f')
					s_map.setTextureRect(sf::IntRect(96, 0, 32, 32));			//������
				if (TileMap[i][j] == 'h')
					s_map.setTextureRect(sf::IntRect(128, 0, 32, 32));			//������
				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
		
		if (!showMissionText) {			//���������� ������ ������ � ����
			std::ostringstream playerHealthStringMenu, task;
			playerHealthStringMenu << p.health;
			task << getTextMission(getCurrentMission(p.x));
			textMenu.setString("��������: " + playerHealthStringMenu.str() + "\n" + task.str());
			textMenu.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);
			s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);
			window.draw(s_quest);
			window.draw(textMenu);
		}

		std::ostringstream playerScoreString;			//������, ���������� ������� ���������� �����
		playerScoreString << p.playerScore;
		text.setString("������� ������: " + playerScoreString.str());			//����� ������ ������
		text.setPosition(view.getCenter().x + 210, view.getCenter().y - 360);			//����� ������� ���������� ������
		window.draw(text);			//���������� ������ �����

		std::ostringstream playerHealthString, gameTimeString;			//������ ������ ���������� hp � ������� ����� ������
		playerHealthString << p.health;
		gameTimeString << gameTime;
		text.setString("��������: " + playerHealthString.str() + "\n����� ����: " + gameTimeString.str());			//����� ������ ������
		text.setPosition(view.getCenter().x - 630, view.getCenter().y - 360);			//����� ������� ���������� ������
		window.draw(text);			//���������� ������ ��������

		//���������� ��������
		lvl.Draw(window);			//������ ����� �����
		//window.draw(easyEnemy.sprite);
		for (it = entities.begin(); it != entities.end(); it++)
			window.draw((*it)->sprite);			//������ ��� entities �������
		window.draw(p.sprite);
		window.display();
	}
	return 0;
}*/