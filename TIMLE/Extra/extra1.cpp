/*#include <iostream>
#include <sstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include "mission.h"

class Player {
	//private:
	//float x, y;			//���������� ������(x, y)
public:
	float w, h, dx, dy, speed, x, y;			//������ � ������ (w, h), ��������� �� x � y (dx, dy), �������� (speed)
	int dir, playerScore, health;			//����������� (direction) �������� ������, ���� ������ (playerScore), ���������� ��������
	bool life, isMove, isSelect;			//����� ������, ���������� ��������, ����� �������
	sf::String File;			//���� � �����������
	sf::Image image;			//�������� ������
	sf::Texture texture;			//�������� ������
	sf::Sprite sprite;			//������ ������

	Player(sf::String F, float X, float Y, float W, float H)			//����������� � ����������� ��� ������
	{
		dx = 0; dy = 0; speed = 0; dir = 0; speed = 0; playerScore = 0; health = 100;
		life = true; isMove = false; isSelect = false;
		File = F;			//��� ����� � ����������
		w = W; h = H;
		image.loadFromFile("Images/" + File);
		//image.createMaskFromColor(sf::Color(41, 33, 59));
		image.createMaskFromColor(sf::Color(0, 0, 255));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;			//���������� ��������� �������
		sprite.setTextureRect(sf::IntRect(0, 0, w, h));			//���������� ������ ���� �������� � �������
		sprite.setOrigin(w / 2, h / 2);			//����� ������� ��������� � �������� ������ ���� � �����
	}

	void update(float time)			//������� "���������" ������
	{
		switch (dir)			//� ����������� �� �����������
		{
		case 0:			//��� ������ �����
			dx = speed;
			dy = 0;
			break;
		case 1:			//��� ������ ������
			dx = -speed;
			dy = 0;
			break;
		case 2:			//��� ������ ����
			dx = 0;
			dy = speed;
			break;
		case 3:			//��� ������ �����
			dx = 0;
			dy = -speed;
			break;
		}
		x += dx * time;			//�������� ��������� �� �������
		y += dy * time;
		speed = 0;			//�������� ��������, ����� ������ �����������
		sprite.setPosition(x, y);			//������� ������ � ������� (x, y)
		interactionWithMap();			//�������������� � ������
		if (health <= 0) {
			life = false;
			speed = 0;
			sprite.setColor(sf::Color::Red);
		}
	}

	void interactionWithMap()			//������� �������������� � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)			//���������� �� ���� ���������, �������������� � �������
			for (int j = x / 32; j < (x + w) / 32; j++)			//�������� �� ���� ��������� ����� �������, � �������� ��������������� ��������
			{
				if (TileMap[i][j] == '0') {			//���� �������� ����������� � �����, �� � ����������� �� �����������:
					if (dy > 0) {			//���� �������� �������� ����,
						y = i * 32 - h;			//�� ������������� ���, �.�. �������� ������� ������� �, ��� ������� ����� ����, ������������� ��� � ���� �������
					}
					if (dy < 0) {			//���������� �������� �����
						y = i * 32 + 32;
					}
					if (dx > 0) {			//���� �������� �������� ������, �� ������������� ��� ���������� �������� �����/����
						x = j * 32 - w;
					}
					if (dx < 0) {			//���������� �������� ������
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
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Game", sf::Style::Default);			//������������� ���� - Fullscreen
	view.reset(sf::FloatRect(0, 0, 1280, 720));			//������������� ������ � ����
	window.setFramerateLimit(60);

	Player p("heroForRotate.png", 250, 250, 136, 74);			//������ ������ ������ Player (250, 250, 96.0, 96.0)
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
	//text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	sf::Clock clock;			//��������������� ������
	sf::Clock gameTimeClock;			//������ �������� �������
	int gameTime = 0;			//������� �������� �������
	float CurrentFrame = 0;			//������� ��� �������� ���������
	int createObjectForMapTimer = 0;			//������ ��� ��������� ��������
	int buttonTimer = 0;			//������ ������� ������

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

	bool isMove = false;			//���������� ��� ������ ���� �� �������
	float dX = 0, dY = 0;			//���������� ��� ������������� ��������� ������� �� x � y

	int tempX = 0, tempY = 0;			//��������� ���������� x � y, ���������� ����� ������� ������ ����
	float distance = 0;			//���������� �� ������� �� �������

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();			//��������������� ������
		clock.restart();
		time = time / 800;

		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);			//�������� ���������� �������
		sf::Vector2f pos = window.mapPixelToCoords(pixelPos);			//��������� ���������� ������� � ������� ����������

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

			if (event.type == sf::Event::MouseButtonPressed)			//���� ������ ����� ������ ����
				if (event.key.code == sf::Mouse::Left)
					if (p.sprite.getGlobalBounds().contains(pos.x, pos.y)) {			//� ���������� ������� �������� � ������
						p.sprite.setColor(sf::Color::Green);			//���������� ������ � ������ ����
						p.isSelect = true;			//������ ��������� ���������
					}

			if (p.isSelect)
				if (event.type == sf::Event::MouseButtonPressed)			//���� ������ ����� ������ ����
					if (event.key.code == sf::Mouse::Right) {
						p.isMove = true;			//��������� �������� �������
						p.isSelect = false;			//������ ��������� �����������
						p.sprite.setColor(sf::Color::White);			//���������� ������� ���� �������
						tempX = pos.x;			//��������� ���������� ������� �������
						tempY = pos.y;
						float dX1 = pos.x - p.x;			//������, ������������ ������, ������� ���������� ������ � ������
						float dY1 = pos.y - p.y;			//�� �� � ���������� y
						float rotation = (atan2(dY1, dX1)) * 180 / 3.14159265;			//�������� ���� � ��������� � ��������� ��� � �������
						p.sprite.setRotation(rotation);
					}

			if (event.type == sf::Event::MouseButtonPressed)			//���� ������ ����� ������ ����
			if (event.key.code == sf::Mouse::Left)
			if (p.sprite.getGlobalBounds().contains(pos.x, pos.y)) {			//� ���������� ������� �������� � ������
			dX = pos.x - p.sprite.getPosition().x;			//�������� �� ��������� ������� ���������� ������� ��� ������������ �������
			dY = pos.y - p.sprite.getPosition().y;
			isMove = true;			//��������� ������� ������
			}

			if (event.type == sf::Event::MouseButtonReleased)			//���� ����� ������ ���� ��������
			if (event.key.code == sf::Mouse::Left) {
			isMove = false;			//��������� ������� ������
			p.sprite.setColor(sf::Color::White);			//���������� ������� ����
			}
		}

		if (p.isMove) {
			distance = sqrt((tempX - p.x) * (tempX - p.x) + (tempY - p.y) * (tempY - p.y));			//������� ��������� �� ������� ����� �������
			if (distance > 2) {			//������� ������� �� ����� �������� ������� �������
				p.x += 0.1 * time * (tempX - p.x) / distance;			//���������� ������ �� x ������� �������
				p.y += 0.1 * time * (tempY - p.y) / distance;			//���������� ������ �� y ������� �������
			}
			else {
				p.isMove = false;			//��������� �������� �������
			}
		}

		if (isMove) {			//���� ��������� ������� ������
		p.sprite.setColor(sf::Color::Green);			//������������� ������ � ������ ����
		p.x = pos.x - dX;
		p.y = pos.y - dY;
		//p.sprite.setPosition(pos.x - dX, pos.y - dY);			//�������� ������� �������
		}

		if (p.life) {
			gameTime = gameTimeClock.getElapsedTime().asSeconds();			//����� ����� ������ ������, ���� �� ��� � ������
		}
		else {
			view.rotate(0.01);
		}

		//���������� ���������� � ���������
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
			getPlayerCoordinateForView(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());			//������� ���������� ������ � ������� ���������� �������
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		p.sprite.setColor(sf::Color::Red);
		}

		sf::Vector2i localPosition = sf::Mouse::getPosition(window);			//������, ������������� ���������� ������� ����
		if (localPosition.x < 3) {			//���� ������ � ����� ���� ����, �� ������� ������ �����
			view.move(-0.2 * time, 0);
		}
		if (localPosition.x > window.getSize().x - 3) {			//���� ������ � ������ ���� ����, �� ������� ������ ������
			view.move(0.2 * time, 0);
		}
		if (localPosition.y > window.getSize().y - 3) {			//���� ������ � ������ ���� ����, �� ������� ������ ����
			view.move(0, 0.2 * time);
		}
		if (localPosition.y < 3) {			//���� ������ � ������� ���� ����, �� ������� ������ �����
			view.move(0, -0.2 * time);
		}

		createObjectForMapTimer += time;			//����������� �������
		if (createObjectForMapTimer > 3000) {
			randomMapGenerate();			//��������� ��������
			createObjectForMapTimer = 0;			//��������� �������
		}

		p.update(time);			//"��������" ������ � ������� �������
		viewMap(time);			//������� ����������� ������ ��� ������
		changeView();
		window.setView(view);			//����������� ������

		window.clear(sf::Color(128, 106, 89));			//������� ����
														//window.clear();

		window.getSystemHandle();

		//���������� �����
		if (getCurrentMission(p.getPlayerCoordinateX()) == 0) {
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
		}
		else if (getCurrentMission(p.getPlayerCoordinateX()) == 1) {
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
				{
					if (TileMap[i][j] == ' ')
						s_map.setTextureRect(sf::IntRect(64, 0, 32, 32));			//������� ����
					if (TileMap[i][j] == 's')
						s_map.setTextureRect(sf::IntRect(32, 0, 32, 32));			//������
					if (TileMap[i][j] == '0')
						s_map.setTextureRect(sf::IntRect(0, 0, 32, 32));			//�����
					if (TileMap[i][j] == 'f')
						s_map.setTextureRect(sf::IntRect(96, 0, 32, 32));			//������
					if (TileMap[i][j] == 'h')
						s_map.setTextureRect(sf::IntRect(128, 0, 32, 32));			//������
					s_map.setPosition(j * 32, i * 32);
					window.draw(s_map);
				}
		}

		if (!showMissionText) {			//���������� ������ ������ � ����
			std::ostringstream playerHealthStringMenu, task;
			playerHealthStringMenu << p.health;
			task << getTextMission(getCurrentMission(p.getPlayerCoordinateX()));
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

									//���������� ������
		window.draw(p.sprite);

		window.display();
	}

	return 0;
}*/

//extra cod from view.h
/*void viewMap(float time)			//������� ��� ����������� ������ �� ����� �� �������
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		view.move(-0.1 * time, 0);			//���������� ������ ��� ������ �����
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		view.move(0.1 * time, 0);			//���������� ������ ��� ������ ������
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		view.move(0, -0.1 * time);			//���������� ������ ��� ������ �����
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		view.move(0, 0.1 * time);			//���������� ������ ��� ������ ����
	}
}

void changeView()			//�������, ���������� ��������� ������
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		view.zoom(1.0100f);			//������������, ���������
		//view.zoom(1.0006f);			//������������ ���������
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		//view.setRotation(90);		//����� ���� �������� ������
		view.rotate(1);			//���������� ������������ ������
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
		view.setSize(800, 640);			//������������� �������� ������ ������
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		view.setSize(640, 480);			//������������� ������� ������ ������
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		view.setViewport(sf::FloatRect(0, 0, 0.5f, 1));			//�������� ����������� ������
	}
}*/