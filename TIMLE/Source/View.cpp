#include "../Include/View.hpp"


void setPlayerCoordinateForView(float x, float y)    // ������� ��� ���������� ��������� ������
{
    float tempX = x, tempY = y;
    // ��� ���������� ���� 1280 � 720
    /*if (1750 < y && y < 2000) {
        tempY = 1800;
    }
    if (1000 < y && y < 1750) {
        tempY = 1400;
    }
    if (100 < y && y < 1000) {
        tempY = 500;
    }*/

    if (x < 640)
        tempX = 640;    // ������� �� ���� ����� �������
    if (y < 100)
        tempY = 100;    // ������� �� ���� ������� �������
    if (y > 2000)
        tempY = 2000;    // ������� �� ���� ������ �������
    if (x > 2560)
        tempX = 2560;    // ������� �� ���� ������ �������

    view.setCenter(tempX, tempY);    // ����������� ������
}
