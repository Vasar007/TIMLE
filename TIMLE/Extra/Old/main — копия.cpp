///������� ���� ���������
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

///������� ����� ������
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

///������� �������
bool startGame(sf::RenderWindow &window, int &numberLevel, bool &showMainMenu)
{
	///���� ������������� ���� ���������
	view.reset(sf::FloatRect(0, 0, 1280, 720));	//������������� ������ � ����
	if (showMainMenu)
		menu(window);	//����� ����


	///���� ������������� �������� � ������� ��� ���� ��������
	Level lvl;	//������� ��������� ������ �������
	changeLevel(lvl, numberLevel);	//��� �������� ����� ��� ������� ������
	//lvl.LoadFromFile("map.tmx");	//��������� �����, ������ ������ �� � ������� ������� � ������������

	sf::Image heroImage;	//�������� �����
	heroImage.loadFromFile("Images/Textures/vanilla.png");
	sf::Image heroDeath;
	heroDeath.loadFromFile("Images/Textures/imp death.png");

	sf::Image ghostEnemyImage;	//�������� ��������
	ghostEnemyImage.loadFromFile("Images/Textures/ghost.png");
	sf::Image ghostEnemyImageDeath;	//�������� ������ ��������
	ghostEnemyImageDeath.loadFromFile("Images/Textures/lightning.png");

	sf::Image golemEnemyImage;	//�������� ������
	golemEnemyImage.loadFromFile("Images/Textures/golem-walk.png");
	sf::Image golemEnemyImageAttack;	//�������� ����� ������
	golemEnemyImageAttack.loadFromFile("Images/Textures/golem-atk.png");
	sf::Image golemEnemyImageDeath;	//�������� ������ ������
	golemEnemyImageDeath.loadFromFile("Images/Textures/golem-die.png");

	sf::Image soldierEnemyImage;	//�������� ������ �������
	soldierEnemyImage.loadFromFile("Images/Textures/darksoldiersheetupdate.png");

	sf::Image movePlatformImage;	//�������� ���������
	movePlatformImage.loadFromFile("Images/Textures/MovingPlatform.png");

	sf::Image BulletImage;	//����������� ��� ����
	BulletImage.loadFromFile("Images/Textures/mage-bullet-13x13.png");	//��������� �������� � ������ �����������
	//BulletImage.createMaskFromColor(sf::Color(0, 0, 0));	//����� ��� ���� �� ������� �����


	///���� ������������� ��������
	Object player = lvl.GetObject("player");	//������ ������ �� ����� �����.������ ���������� ������ � ������ ��� ������ ����
	Player p(heroImage, heroDeath, "Player1", lvl, player.rect.left, player.rect.top, 30, 61);	///�������� ���������� �������������� player �� ����� � ���������� ������ ������ 30 41
	LifeBar lifeBarPlayer;	//��������� ������ ������� ��������

	std::list<Entity*> entities;	//������, ���������� ����������� �������
	std::list<Entity*>::iterator it;	//��������, ����� ��������� �� ��������� ������
	std::list<Entity*>::iterator it2;	//������ �������� ��� �������������� ����� ��������� ������

	std::vector<Object> e = lvl.GetObjects("enemyGhost");	//��� ������ ����� �� tmx-����� ����� ��������� � �������
	for (int i = 0; i < e.size(); i++)	//���������� �� ��������� ������� � �������
		entities.push_back(new EnemyGhost(ghostEnemyImage, ghostEnemyImageDeath, "EnemyGhost", lvl, e[i].rect.left, e[i].rect.top, 102, 92));	//��������� � ������ ���� ������ � �����

	e = lvl.GetObjects("enemyGolem");	//��� ������ ����� �� tmx-����� ����� ��������� � �������
	for (int i = 0; i < e.size(); i++)	//���������� �� ��������� ������� � �������
		entities.push_back(new EnemyGolem(golemEnemyImage, golemEnemyImageAttack, golemEnemyImageDeath, "EnemyGolem", lvl, e[i].rect.left, e[i].rect.top, 48, 64));

	e = lvl.GetObjects("enemySoldier");	//��� ������ ����� �� tmx-����� ����� ��������� � �������
	for (int i = 0; i < e.size(); i++)	//���������� �� ��������� ������� � �������
		entities.push_back(new EnemyDarkSoldier(soldierEnemyImage, "EnemySoldier", lvl, e[i].rect.left, e[i].rect.top, 40, 60));

	e = lvl.GetObjects("MovingPlatform");	//��������� ��� ���������
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new MovingPlatform(movePlatformImage, "MovingPlatform", lvl, e[i].rect.left, e[i].rect.top, 95, 22));	//���������� ��������� � ������, �������� ����������� ��� ������� ���������� ��������� (����� �� tmx �����), � ��� �� �������


	///���� ������������� �����
	sf::SoundBuffer shootBuffer;	//������ ����� ��� �����
	shootBuffer.loadFromFile("Sounds/fairbol3.ogg");	//��������� � ���� ����
	sf::Sound shoot(shootBuffer);	//������� ���� � ��������� � ���� ���� �� ������

	sf::Music music;	//������� ������ ������
	music.openFromFile("Sounds/music.ogg");	//��������� ����
	music.play();	//������������� ������
	music.setLoop(true);	//���������� ������������� ������


	///���� ������������� ����
	sf::Font font;			//�����
	font.loadFromFile("Other/CyrilicOld.ttf");
	sf::Text text("text", font, 20);	//����� � ��������� ������� � �������
	sf::Text textMenu("textMenu", font, 20);
	//text.setColor(sf::Color::Black);

	bool showMissionText = true;		//����������, ���������� �� ��������� ������ ������ �� ������, � ���� ������� ������ (tabPress = false;)
	sf::Image quest_image;	//�������� ������
	quest_image.loadFromFile("Images/Textures/missionbg.jpg");
	quest_image.createMaskFromColor(sf::Color(0, 0, 0));
	sf::Texture quest_texture;	//�������� ������
	quest_texture.loadFromImage(quest_image);
	sf::Sprite s_quest;		//������ ������
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(sf::IntRect(0, 0, 340, 510));
	s_quest.setScale(0.6f, 0.6f);	//���������� ������� �������


	///���� ������������� ��������
	sf::Clock clock;	//��������������� ������
	sf::Clock gameTimeClock;	//������ �������� �������
	int gameTime = 0;	//������� �������� �������
	//float currentFrame = 0;	//������� ��� �������� ���������
	//int createObjectForMapTimer = 0;	//������ ��� ��������� ��������


	///����������� ���� ���������
	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds();	//��������������� ������
		clock.restart();
		time = time / 800;

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			window.setKeyRepeatEnabled(false);	//������� ��������� ������������ �������, ���� ������� ������������

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {	//���� backspace, �� ������������� ����
				showMainMenu = true;
				return true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {	//���� ������, �� ������� �� ����
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

			if (event.type == sf::Event::MouseButtonPressed)	//���� ������ ������� ����
				if (event.key.code == sf::Mouse::Left) {	//� ������ �����, �� �������� � ��� �� ������� ���������� ���� � ���� �����
					sf::Vector2i pixelPos = sf::Mouse::getPosition(window);	//�������� ����� �������
					sf::Vector2f pos = window.mapPixelToCoords(pixelPos);	//��������� �� � ������� (������ �� ����� ����)
					entities.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x + (p.w / 2), p.y, 13, 13, pos.x, pos.y));		//� �������� � ����������� ����. ��������� ����
					shoot.play();	//������ ���� ����
				}

			if (p.isShoot == true) {	//���� ����������, �� ���������� ����. enum �������� ��� int 
				p.isShoot = false;
				entities.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x, p.y, 13, 13, p.state));
				shoot.play();	//������ ���� ����
			}

			if (event.type == sf::Event::KeyPressed) {	//���� �������� Tab, ���������� ����, �������� ��� ��� - ���������
				if (event.key.code == sf::Keyboard::Tab) {
					//window.setKeyRepeatEnabled(false);	//������� ��������� ������������ �������, ���� ������� ������������
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
			gameTime = gameTimeClock.getElapsedTime().asSeconds();	//����� ����� ������ ������, ���� �� ��� � ������
		}

		//createObjectForMapTimer += time;	//����������� �������
		//if (createObjectForMapTimer > 3000) {
		//	createObjectForMapTimer = 0;	//��������� �������
		//}

		///���� "����������" ��������
		//p.update(time);	//"��������" ������ � ������� �������
		//lifeBarPlayer.update(p.health);	//��������� ������ ��������
		for (it = entities.begin(); it != entities.end();) {	//�������� �� �������� ������ �� ������ �� �����
			Entity *b = *it;	//��� ��������� ���������� ������ ���������
			b->update(time);	//�������� ������� update ��� ��������
			if (b->life == false) {	//���� ������ "����"
				it = entities.erase(it);	//������� ���
				delete b;	//����������� ������
				//entities.push_back(new Enemy(ghsotEnemyImage, "EnemyGhost", lvl, 900, 500, 102, 90));	//������ ������ �����
			}
			else {
				it++;	//���� ���, ��� ������ �� ������
			}
		}

		p.onPlatform = 0;
		for (it = entities.begin(); it != entities.end(); it++) {	//����� ���������� �� ��������� ������
			if (((*it)->name == "MovingPlatform") && ((*it)->getRect().intersects(p.getRect()))) {	//���� ����� ���������� � �������� ������ � ��� ����� ������� movingplatform
				Entity *movPlat = *it;
				if ((p.dy > 0) || (p.onGround == false))	//��� ���� ����� ��������� � ��������� ����� ������, �.� ������ ����
					if (p.y + p.h < movPlat->y + movPlat->h) {	//���� ����� ��������� ���� ���������, �.� ��� ��� ���� ������� (�� �� ��� ��������� ��� �� ���������� � ����������)
						p.y = movPlat->y - p.h + 3;
						p.x += movPlat->dx * time;
						p.dy = 0;
						p.onPlatform = movPlat->dx;
						p.onGround = true;	// �� ����������� ������ ���, ����� �� ��� �� ����� �� ���������
					}
			}

			if ((*it)->getRect().intersects(p.getRect())) {	//���� ������������� ������� ������� ������������ � �������
				if ((*it)->name == "EnemyGhost") {	//� ��� ��� enemyGhost
					if (p.life && (p.health > 0)) {	//���� �� � ������, ��
						if (!(*it)->isBack && (p.dx * (*it)->dx > 0)) {
							(*it)->isBack = true;
							(*it)->dx = -(*it)->dx;
							(*it)->sprite.scale(-1, 1);
						}
						(*it)->isAttacked = true;	//���� ���������������
						(*it)->CurrentFrame = 0;
						if ((*it)->isHitted) {
							p.health -= 7;	//����� ���� ������� ����
							p.dy = -0.1;	//����� ����������� �� �����
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
				if ((*it)->name == "EnemyGolem") {	//� ��� ��� MediumEnemy
					if (!(*it)->isStarted) {
						(*it)->isStarted = true;
					}
					else if (p.life && (p.health > 0)) {	//���� �� ����
						if (!(*it)->isBack && (p.dx * (*it)->dx > 0)) {
							(*it)->isBack = true;
							(*it)->dx = -(*it)->dx;
						}
						(*it)->isAttacked = true;	//���� ���������������
						(*it)->CurrentFrame = 0;
						if ((*it)->isHitted) {
							p.health -= 5;	//����� ���� ������� ����
							//p.dx = 3 * (*it)->dx;
							//p.dy = -0.5;	//����� ����������� �� �����
							(*it)->isBack = false;
							(*it)->isHitted = false;
							std::cout << "Hit\n";
						}
					}
					else {
						//(*it)->isAttacked = false;
					}
				}
				if ((*it)->name == "EnemySoldier") {	//� ��� ��� MediumEnemy
					if (p.life && (p.health > 0)) {	//���� �� ����
						if (!(*it)->isBack && (p.dx * (*it)->dx > 0)) {
							(*it)->isBack = true;
							(*it)->dx =  -(*it)->dx;
						}
						(*it)->isAttacked = true;	//���� ���������������
						(*it)->CurrentFrame = 0;
						if ((*it)->isHitted) {
							p.x = ((*it)->dx > 0? 1.2 : 0.8) * (*it)->x;
							p.y = 0.9 * (*it)->y;	//����� ����������� �� �����
							p.dy = -0.25;
							//p.health -= 5;	//����� ���� ������� ����
							//p.dx = 3 * (*it)->dx;
							(*it)->isBack = false;
							(*it)->isHitted = false;
							std::cout << "Hit\n";
						}
						//p.x = 0.97 * ((*it)->dx > 0? (*it)->x + 60 : (*it)->x);
						//p.dy = -0.5;	//����� ����������� �� �����
					}
					else {
						//(*it)->isAttacked = false;
					}
				}
			}

			for (it2 = entities.begin(); it2 != entities.end(); it2++) {
				if ((*it)->getRect() != (*it2)->getRect()) {	//��� ���� ��� ������ ���� ������ ��������������
					//if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EnemyGhost") && ((*it2)->name == "EnemyGhost")) {	//���� ����������� ��� ������� � ��� �����
						//(*it)->dx *= -1;	//������ ����������� �������� �����
						//(*it)->sprite.scale(-1, 1);	//�������� ������ �� �����������
						//(*it2)->dx *= -1;	//������ ����������� �������� �����
						//(*it2)->sprite.scale(-1, 1);	//�������� ������ �� �����������
					//}
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EnemyGhost") && ((*it2)->name == "Bullet") && ((*it)->health > 0)) {	//���� ����������� ��� ������� ���� � ����
						(*it)->health -= 50;	//������� �����
						(*it2)->life = false;	//������� ����
					}
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EnemyGolem") && ((*it2)->name == "Bullet") && ((*it)->health > 0) && ((*it)->isStarted)) {	//���� ����������� ��� ������� ���� � ����
						(*it)->health -= 50;	//������� �����
						(*it2)->life = false;	//������� ����
					}
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EnemySoldier") && ((*it2)->name == "Bullet") && ((*it)->health > 0)) {	//���� ����������� ��� ������� ���� � ����
						(*it)->health -= 1;	//������� �����
						(*it2)->life = false;	//������� ����
					}
				}
			}
		}
		lifeBarPlayer.update(p.health);	//��������� ������ ��������
		p.update(time);	//"��������" ������ � ������� �������
		window.setView(view);	//����������� ������
		window.clear(sf::Color(77, 83, 140));	//������� ���� 128, 106, 89
		//window.clear();

		///���������� ��������
		lvl.Draw(window);	//������ ����� �����
		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite);	//������ ��� entities �������
		}
		window.draw(p.sprite);
		lifeBarPlayer.draw(window);	//������ ������� ��������

		std::ostringstream playerScoreString;	//������, ���������� ������� ���������� �����
		playerScoreString << p.playerScore;
		text.setString("����: " + playerScoreString.str());	//����� ������ ������
		text.setPosition(view.getCenter().x + 210, view.getCenter().y - 355);	//����� ������� ���������� ������
		window.draw(text);	//���������� ������ �����

		std::ostringstream playerHealthString, gameTimeString;	//������ ������ ���������� hp � ������� ����� ������
		if (p.health < 0)
			p.health = 0;
		playerHealthString << p.health;
		gameTimeString << gameTime;
		text.setString("��������: " + playerHealthString.str() + "\n����� ����: " + gameTimeString.str());	//����� ������ ������
		text.setPosition(view.getCenter().x - 600, view.getCenter().y - 355);	//����� ������� ���������� ������
		window.draw(text);	//���������� ������ ��������

		if (!showMissionText) {	//���������� ������ ������ � ����
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
	}
}


///������� ������������� ���� , ���� ��� ����������
void gameRunning(sf::RenderWindow &window, int &numberLevel, bool &showMainMenu) {
	if (startGame(window, numberLevel, showMainMenu)) {	//��������� � ������ ������ ������ ����
		gameRunning(window, numberLevel, showMainMenu);
	}
}


///�������� ����� ���������
int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Game", sf::Style::Titlebar | sf::Style::Close);	//������������� ����
	//sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game", sf::Style::Fullscreen);
	window.setFramerateLimit(60);
	int numberLevel = 1;	//����� �������� ������
	bool showMainMenu = true;
	gameRunning(window, numberLevel, showMainMenu);	//��������� ������� ����
	return 0;
}