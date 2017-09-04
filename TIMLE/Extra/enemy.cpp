#include "enemy.h"

Enemy::Enemy(sf::Image &image, sf::String Name, Level &lvl, float X, float Y, int W, int H) : Entity(image, Name, X, Y, W, H)
{
	obj = lvl.GetObjects("solid");	//инициализируем.получаем нужные объекты для взаимодействия врага с картой
	if (name == "EasyEnemy") {
		sprite.setTextureRect(sf::IntRect(0, 0, w, h));
		sprite.scale(-1, 1);
		dx = 0.1;	//объект всегда имеет скорость
	}

	if (name == "MediumEnemy") {
		sprite.setTextureRect(sf::IntRect(0, 64, 64, 64));
		dx = 0.05;	//объект всегда имеет скорость
	}
	CurrentAttack = 0;
	isTurned = false;
}

Enemy::Enemy(sf::Image &image, sf::Image &imageAttack, sf::String Name, Level &lvl, float X, float Y, int W, int H) : Entity(image, imageAttack, Name, X, Y, W, H)
{
	obj = lvl.GetObjects("solid");	//инициализируем.получаем нужные объекты для взаимодействия врага с картой
	if (name == "EasyEnemy") {
		sprite.setTextureRect(sf::IntRect(0, 0, w, h));
		sprite.scale(-1, 1);
		dx = 0.1;	//объект всегда имеет скорость
	}

	if (name == "MediumEnemy") {
		sprite.setTextureRect(sf::IntRect(0, 192, 64, 64));
		dx = 0.05;	//объект всегда имеет скорость
	}
	CurrentAttack = 0; CurrentDeath = 0; counter = 0;
	isTurned = false;
}

Enemy::Enemy(sf::Image &image, sf::Image &imageAttack, sf::Image &imageDeath, sf::String Name, Level &lvl, float X, float Y, int W, int H) : Entity(image, imageAttack, imageDeath, Name, X, Y, W, H)
{
	obj = lvl.GetObjects("solid");	//инициализируем.получаем нужные объекты для взаимодействия врага с картой
	if (name == "EasyEnemy") {
		sprite.setTextureRect(sf::IntRect(0, 0, w, h));
		sprite.scale(-1, 1);
		dx = 0.1;	//объект всегда имеет скорость
	}

	if (name == "MediumEnemy") {
		sprite.setTextureRect(sf::IntRect(0, 192, 64, 64));
		dx = 0.05;	//объект всегда имеет скорость
	}
	CurrentAttack = 0; CurrentDeath = 0; counter = 0;
	isTurned = false;
}

void Enemy::checkCollisionWithMap(float Dx, float Dy)
{
	for (int i = 0; i<obj.size(); i++)	//проходимся по объектам
		if (getRect().intersects(obj[i].rect)) {	//проверяем пересечение игрока с объектом
			if (obj[i].name == "solid") {	//если встретили препятствие
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
					dx = -0.05;
					isTurned = true;
					if (name == "EasyEnemy") {
						//sprite.scale(-1, 1);
						dx = -0.1;
					}
				}
				if (Dx<0) {
					x = obj[i].rect.left + obj[i].rect.width;
					dx = 0.05;
					isTurned = true;
					if (name == "EasyEnemy") {
						//sprite.scale(-1, 1);
						dx = 0.1;
					}
				}
			}
		}
}

void Enemy::update(float time)
{
	if (name == "EasyEnemy") {
		if (isTurned)
			moveTimer += time;
		if (moveTimer > 2000 && isTurned) {
			//dx *= -1;
			moveTimer = 0;
			isTurned = false;
			sprite.scale(-1, 1);
		}

		if (life && (health > 0)) {
			checkCollisionWithMap(dx, 0);
			if (!isAttacked && !isTurned) {
				x += dx * time;
				sprite.setPosition(x + (w / 2) + (dx > 0 ? +25 : -25), y + (h / 2));
			}
			else if (!isTurned) {
				sprite.setPosition(x + (w / 2) + (dx > 0 ? +25 : -25), y + (h / 2));
			}
			else {
				sprite.setPosition(x + (w / 2) + (dx < 0 ? +25 : -25), y + (h / 2));
			}

			/// animation
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 3) {
				CurrentFrame -= 3;
			}
			if (isAttacked) {
				CurrentAttack += 0.005 * time;
				if (CurrentAttack > 10) {
					CurrentAttack -= 10;
					isAttacked = false;
				}
				else if (int(CurrentAttack) == 8) {
					isHitted = true;
				}
				else {
					isHitted = false;
				}
				if (dx > 0) {
					sprite.setTextureRect(sf::IntRect(102 * int(CurrentAttack), 0, 102, 90));
				}
				else if (dx < 0) {
					sprite.setTextureRect(sf::IntRect(102 * int(CurrentAttack), 0, 102, 90));
				}
			}
			else {
				sprite.setTexture(texture);
				CurrentAttack = 0;
				if (dx > 0) {
					sprite.setTextureRect(sf::IntRect(102 * int(CurrentFrame), 0, 102, 90));
				}
				else if (dx < 0) {
					sprite.setTextureRect(sf::IntRect(102 * int(CurrentFrame), 0, 102, 90));
				}
			}
		}

		if (health <= 0) {
			CurrentDeath += 0.0035 * time;
			dx = 0; dy = 0;
			if (CurrentDeath > 6) {
				CurrentDeath = 6;
				life = false;
			}
			sprite.setTexture(textureAttack);
			sprite.setPosition(x + (w / 2), y + (h / 2) + 35);
			sprite.setTextureRect(sf::IntRect(64 * (int(CurrentDeath) < 3? int(CurrentDeath) : int(CurrentDeath) + 5), 0, 64, 50));
		}
	}

	if (name == "MediumEnemy") {
		moveTimer += time;
		if (moveTimer > 2000 && isTurned) {
			//dx *= -1;
			moveTimer = 0;
			isTurned = false;
		}
		else if (moveTimer > 1000 && !isTurned) {
			//dx *= -1;
			moveTimer = 0;
			if (health <= 0)
				counter++;
		}
		
		if (life && (health > 0)) {
			checkCollisionWithMap(dx, 0);
			if (!isAttacked && !isTurned)
				x += dx * time;
			sprite.setPosition(x + (w / 2), y + (h / 2) + 5);

			/// animation
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 7) {
				CurrentFrame -= 7;
			}
			if (isAttacked) {
				CurrentAttack += 0.005 * time;
				if (CurrentAttack > 7) {
					CurrentAttack -= 7;
					isAttacked = false;
				}
				else if (int(CurrentAttack) == 4) {
					isHitted = true;
				}
				else {
					isHitted = false;
				}
				sprite.setTexture(textureAttack);
				sprite.setPosition(x + (w / 2) + (dx > 0 ? 0 : -12), y + (h / 2) - 5);
				if (dx > 0) {
					sprite.setTextureRect(sf::IntRect(64 * int(CurrentAttack), 311, 64, 72));
				}
				else if (dx < 0) {
					sprite.setTextureRect(sf::IntRect(64 * int(CurrentAttack), 119, 64, 72));
				}
			}
			else {
				sprite.setTexture(texture);
				CurrentAttack = 0;
				if (isTurned) {
					CurrentFrame = 0;
					sprite.setTextureRect(sf::IntRect(64 * int(CurrentFrame) + (dx < 0 ? 0 : 17), (dx < 0 ? 192 : 64), 48, 64));
				}
				if (dx > 0 && !isTurned) {
					sprite.setTextureRect(sf::IntRect(64 * int(CurrentFrame), 192, 48, 64));
				}
				else if (dx < 0 && !isTurned) {
					sprite.setTextureRect(sf::IntRect(64 * int(CurrentFrame) + 17, 64, 48, 64));
				}
			}
		}
		
		if (health <= 0) {
			CurrentDeath += 0.0035 * time;
			dx = 0; dy = 0;
			if (CurrentDeath > 5) {
				CurrentDeath = 5;
				if (counter == 5)
					life = false;
			}
			sprite.setTexture(textureDeath);
			sprite.setPosition(x + (w / 2), y + (h / 2) + 7);
			sprite.setTextureRect(sf::IntRect(64 * int(CurrentDeath), 64, 64, 64));
		}
		//sprite.setScale(1.5f, 1.5f);
	}
}