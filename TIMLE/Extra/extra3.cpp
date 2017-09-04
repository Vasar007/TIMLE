///������� ���� ���������
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


///����� ����� ��������
class Entity {
public:
	float dx, dy, x, y, speed, moveTimer;			//��������� �� x � y (dx, dy), ����������, �������� (speed), ������
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

	sf::FloatRect getRect() {			//������� ��������� ��������������. ��� ��������� ,������� (������, ������)
		return sf::FloatRect(x, y, w, h);			//������� ��� �������� ������������ 
	}

	virtual void update(float time) = 0;
 };


///����� ������
class Player :public Entity {
public:
	int playerScore;			//���� ������ (playerScore)
	enum {left, right, up, down, jump, stay} state;			//��� ������������ - ��������� �������
	bool isShoot;

	Player(sf::Image &image, sf::String Name, Level &lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H)			//����������� � ����������� ��� ������
	{
		playerScore = 0;
		state = stay;
		isShoot = false;
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			isShoot = true;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)			//������� �������������� � ������
	{
		for (int i = 0; i<obj.size(); i++)			//���������� �� ��������
			if (getRect().intersects(obj[i].rect))			//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")			//���� ��������� �����������
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
			else if (dy > 0) {			// ���� �� �� ������������ �� � ������ ���������������� � dy > 0, �� onGround = false.
				onGround = false; 
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


///����� ������
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
		for (int i = 0; i<obj.size(); i++)			//���������� �� ��������
			if (getRect().intersects(obj[i].rect))			//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")			//���� ��������� �����������
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


///����� ����������� ��������
class MovingPlatform : public Entity {
public:
	MovingPlatform(sf::Image &image, sf::String Name, Level &lvl, float X, float Y, int W, int H) : Entity(image, Name, X, Y, W, H)
	{
		sprite.setTextureRect(sf::IntRect(0, 0, W, H));			//�������������
		dx = 0.08;			//����������� ���������
	}

	void update(float time)
	{
		x += dx * time;			//�������� �� �����������
		moveTimer += time;			//���������� ������
		if (moveTimer > 2000) {
			dx *= -1;
			moveTimer = 0;		//���� ������ 2 �������, �� ������ ����������� �������� ��������� �� ���������������
		}
		sprite.setPosition(x + (w / 2), y + (h / 2));			//����� ������� �������
	}
};


///����� ����
class Bullet :public Entity {
public:
	int direction;			//����������� ����

	Bullet(sf::Image &image, sf::String Name, Level &lvl, float X, float Y, int W, int H, int dir) :Entity(image, Name, X, Y, W, H)			//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
	{
		obj = lvl.GetObjects("solid");			//��������������, �������� ������ ������� ��� �������������� ���� � ������
		x = X;
		y = Y;
		direction = dir;
		speed = 0.8;
		w = h = 16;
		life = true;
		//���� ������������� � ������������
	}

	void update(float time)
	{
		switch (direction)
		{
		case 0: 
			dx = -speed; 
			dy = 0;   
			break;			//������� �������� state = left
		case 1: 
			dx = speed; 
			dy = 0;   
			break;			//������� �������� state = right
		case 2: 
			dx = 0; 
			dy = -speed;   
			break;			//������� �������� state = up
		case 3:
			dx = 0; 
			dy = -speed;   
			break;			//������� �������� �� ������� ��������� � �����������, ��������� �����
		case 4: 
			dx = 0; 
			dy = -speed;   
			break;			//������� �������� �� ������� ��������� � �����������, ��������� �����
		case 5: 
			dx = 0; 
			dy = -speed;   
			break;			//������� �������� �� ������� ��������� � �����������, ��������� �����
		}
		x += dx*time;			//���� �������� ���� �� �
		y += dy*time;			//�� �
		if (x <= 0) 
			x = 1;			// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������
		if (y <= 0) 
			y = 1;
		for (int i = 0; i < obj.size(); i++)			//������ �� �������� solid
		{
			if (getRect().intersects(obj[i].rect)) {			//���� ���� ������ ���������� � �����,
				life = false;			// �� ���� �������
			}
			if (moveTimer > 3000) {
				life = false;
			}
		}
		sprite.setPosition(x + (w / 2), y + (h / 2));			//�������� ������� ����
		moveTimer += time;			//���������� ������
	}
};



void gameRunning()			//�-��� ������������� ���� , ���� ��� ����������
{
	if (startGame()) {
		gameRunning();
	}			//���� startGame() == true, �� �������� ������ �-��� isGameRunning, ������� � ���� ������� ����� �������� startGame() 
}

///�������� ����� ���������
int main()
{
	/*
	///���� ������������� ���� ���������
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Game", sf::Style::Default);			//������������� ���� - Fullscreen
	view.reset(sf::FloatRect(0, 0, 1280, 720));			//������������� ������ � ����
	window.setFramerateLimit(60);
	menu(window);			//����� ����


	///���� ������������� �������� � ������� ��� ���� ��������
	Level lvl;			//������� ��������� ������ �������
	lvl.LoadFromFile("map2.tmx");			//��������� �����, ������ ������ �� � ������� ������� � ������������

	sf::Image heroImage;			//�������� �����
	heroImage.loadFromFile("Images/Textures/MilesTailsPrower.gif");
	heroImage.createMaskFromColor(sf::Color(0, 0, 0));

	sf::Image easyEnemyImage;			//�������� ������ �����
	easyEnemyImage.loadFromFile("Images/Textures/shamaich.png");
	easyEnemyImage.createMaskFromColor(sf::Color(255, 0, 0));

	sf::Image movePlatformImage;			//�������� ���������
	movePlatformImage.loadFromFile("Images/Textures/MovingPlatform.png");

	sf::Image BulletImage;			//����������� ��� ����
	BulletImage.loadFromFile("Images/Textures/bullet.png");			//��������� �������� � ������ �����������
	BulletImage.createMaskFromColor(sf::Color(0, 0, 0));			//����� ��� ���� �� ������� �����


	///���� ������������� ��������
	Object player = lvl.GetObject("player");			//������ ������ �� ����� �����.������ ���������� ������ � ������ ��� ������ ����
	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 40, 30);			//�������� ���������� �������������� player �� ����� � ���������� ������ ������
	LifeBar lifeBarPlayer;			//��������� ������ ������� ��������

	std::list<Entity*> entities;			//������, ���������� ����������� �������
	std::list<Entity*>::iterator it;			//��������, ����� ��������� �� ��������� ������
	std::list<Entity*>::iterator it2;			//������ �������� ��� �������������� ����� ��������� ������
	
	std::vector<Object> e = lvl.GetObjects("easyEnemy");			//��� ������ ����� �� tmx-����� ����� ��������� � �������
	for (int i = 0; i < e.size(); i++)			//���������� �� ��������� ������� � �������
		entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 200, 97));			//��������� � ������ ���� ������ � �����
	
	e = lvl.GetObjects("MovingPlatform");			//��������� ��� ���������
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new MovingPlatform(movePlatformImage, "MovingPlatform", lvl, e[i].rect.left, e[i].rect.top, 95, 22));			//���������� ��������� � ������, �������� ����������� ��� ������� ���������� ��������� (����� �� tmx �����), � ��� �� �������


	///���� ������������� �����
	sf::SoundBuffer shootBuffer;			//������ ����� ��� �����
	shootBuffer.loadFromFile("Sounds/shoot.ogg");			//��������� � ���� ����
	sf::Sound shoot(shootBuffer);			//������� ���� � ��������� � ���� ���� �� ������

	sf::Music music;			//������� ������ ������
	music.openFromFile("Sounds/music.ogg");			//��������� ����
	music.play();			//������������� ������
	music.setLoop(true);			//���������� ������������� ������


	///���� ������������� ����
	sf::Font font;			//�����
	font.loadFromFile("Other/CyrilicOld.ttf");
	sf::Text text("text", font, 20);			//����� � ��������� ������� � �������
	sf::Text textMenu("textMenu", font, 20);
	//text.setColor(sf::Color::Black);

	bool showMissionText = true;			//����������, ���������� �� ��������� ������ ������ �� ������, � ���� ������� ������ (tabPress = false;)
	sf::Image quest_image;			//�������� ������
	quest_image.loadFromFile("Images/Textures/missionbg.jpg");
	quest_image.createMaskFromColor(sf::Color(0, 0, 0));
	sf::Texture quest_texture;			//�������� ������
	quest_texture.loadFromImage(quest_image);
	sf::Sprite s_quest;				//������ ������
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(sf::IntRect(0, 0, 340, 510));
	s_quest.setScale(0.6f, 0.6f);			//���������� ������� �������


	///���� ������������� ��������
	sf::Clock clock;			//��������������� ������
	sf::Clock gameTimeClock;			//������ �������� �������
	int gameTime = 0;			//������� �������� �������
	float currentFrame = 0;			//������� ��� �������� ���������
	int createObjectForMapTimer = 0;			//������ ��� ��������� ��������


	///����������� ���� ���������
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

			window.setKeyRepeatEnabled(false);			//������� ��������� ������������ �������, ���� ������� ������������
			
			if (p.isShoot == true) {			//���� ����������, �� ���������� ����. enum �������� ��� int 
				p.isShoot = false; 
				entities.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x + (p.w / 2), p.y, 16, 16, p.state));
				shoot.play();			//������ ���� ����
			}

			if (event.type == sf::Event::KeyPressed) {			//���� �������� Tab, ���������� ����, �������� ��� ��� - ���������
				if (event.key.code == sf::Keyboard::Tab) {
					//window.setKeyRepeatEnabled(false);			//������� ��������� ������������ �������, ���� ������� ������������
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

		//createObjectForMapTimer += time;			//����������� �������
		//if (createObjectForMapTimer > 3000) {
			//randomMapGenerate();			//��������� ��������
			//createObjectForMapTimer = 0;			//��������� �������
		//}

		///���� "����������" ��������
		p.update(time);			//"��������" ������ � ������� �������
		lifeBarPlayer.update(p.health);			//��������� ������ ��������
		for (it = entities.begin(); it != entities.end();)			//�������� �� �������� ������ �� ������ �� �����
		{
			Entity *b = *it;			//��� ��������� ���������� ������ ���������
			b->update(time);			//�������� ������� update ��� ��������
			if (b->life == false) {			//���� ������ "����"
				it = entities.erase(it);			//������� ���
				delete b;			//����������� ������
				//entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, 900, 500, 200, 97));			//������ ������ �����
			}
			else {
				it++;			//���� ���, ��� ������ �� ������
			}
		}

		for (it = entities.begin(); it != entities.end(); it++)			//����� ���������� �� ��������� ������
		{
			if (((*it)->name == "MovingPlatform") & ((*it)->getRect().intersects(p.getRect()))) {			//���� ����� ���������� � �������� ������ � ��� ����� ������� movingplatform
				Entity *movPlat = *it;
				if ((p.dy > 0) | (p.onGround == false))			//��� ���� ����� ��������� � ��������� ����� ������, �.� ������ ����
					if (p.y + p.h<movPlat->y + movPlat->h) {			//���� ����� ��������� ���� ���������, �.� ��� ��� ���� ������� (�� �� ��� ��������� ��� �� ���������� � ����������)
						p.y = movPlat->y - p.h + 3; 
						p.x += movPlat->dx*time; 
						p.dy = 0; 
						p.onGround = true;			// �� ����������� ������ ���, ����� �� ��� �� ����� �� ���������
					}
			}

			if ((*it)->getRect().intersects(p.getRect())) {			//���� ������������� ������� ������� ������������ � �������
				if ((*it)->name == "EasyEnemy") {			//� ��� ��� EasyEnemy
					if ((p.dy > 0) & (p.onGround == false)) {			//���� �� � ������, ��
						(*it)->dx = 0;			//���� ���������������
						p.dy = -0.2;			//����� ����������� �� �����
						(*it)->health = 0;			//������� �����
					}
					else {
						p.health -= 5;			//����� ���� ������� ����
						p.dy = -0.5;			//����� ����������� �� �����
					}
				}
			}

			for (it2 = entities.begin(); it2 != entities.end(); it2++) 
			{
				if ((*it)->getRect() != (*it2)->getRect()) {			//��� ���� ��� ������ ���� ������ ��������������
					if (((*it)->getRect().intersects((*it2)->getRect())) & ((*it)->name == "EasyEnemy") & ((*it2)->name == "EasyEnemy")) {			//���� ����������� ��� ������� � ��� �����
						(*it)->dx *= -1;			//������ ����������� �������� �����
						(*it)->sprite.scale(-1, 1);			//�������� ������ �� �����������
					}
					if (((*it)->getRect().intersects((*it2)->getRect())) & ((*it)->name == "EasyEnemy") & ((*it2)->name == "Bullet")) {			//���� ����������� ��� ������� � ��� �����
						(*it)->life = false;
						(*it2)->life = false;
					}
				}
			}
		}

		window.setView(view);			//����������� ������
		window.clear(sf::Color(77, 83, 140));			//������� ���� 128, 106, 89
		//window.clear();

		///���������� ��������
		lvl.Draw(window);			//������ ����� �����
		for (it = entities.begin(); it != entities.end(); it++)
			window.draw((*it)->sprite);			//������ ��� entities �������
		window.draw(p.sprite);
		lifeBarPlayer.draw(window);			//������ ������� ��������

		std::ostringstream playerScoreString;			//������, ���������� ������� ���������� �����
		playerScoreString << p.playerScore;
		text.setString("����: " + playerScoreString.str());			//����� ������ ������
		text.setPosition(view.getCenter().x + 210, view.getCenter().y - 355);			//����� ������� ���������� ������
		window.draw(text);			//���������� ������ �����

		std::ostringstream playerHealthString, gameTimeString;			//������ ������ ���������� hp � ������� ����� ������
		playerHealthString << p.health;
		gameTimeString << gameTime;
		text.setString("��������: " + playerHealthString.str() + "\n����� ����: " + gameTimeString.str());			//����� ������ ������
		text.setPosition(view.getCenter().x - 600, view.getCenter().y - 355);			//����� ������� ���������� ������
		window.draw(text);			//���������� ������ ��������

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

		window.display();
	}*/
	gameRunning();			//��������� ������� ����
	return 0;
}