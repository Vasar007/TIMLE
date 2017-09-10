///����, ���������� �� ����� � ����
#pragma once

#ifndef MAP_H
#define MAP_H

const int HEIGHT_MAP = 25;	//������ �����
const int WIDTH_MAP = 40;	//������ �����

sf::String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0              s    0000               0",
	"0           000000000000               0",
	"0           000000000000               0",
	"0   f    h  000000000000               0",
	"0000000000000000000000000000000000000000",
};

void randomMapGenerate()
{
	int randomElementX = 0, randomElementY = 0, countStone = 2;	//��������� �������� �� ����������� � ���������, ���������� ������
	srand(time(0));
	
	while (countStone > 0)
	{
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);	//��������� ����� �� x �� 1 �� ������ ����� - 1
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);	//��������� ����� �� y �� 1 �� ������ ����� - 1

		if (TileMap[randomElementY][randomElementX] == ' ') {
			TileMap[randomElementY][randomElementX] = 's';
			countStone--;
		}
	}
}


#endif /* MAP_H */