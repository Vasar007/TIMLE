#include "../Include/View.hpp"


void setPlayerCoordinateForView(float x, float y)    // Функция для считывания координат игрока
{
    float tempX = x, tempY = y;
    // Для разрешения окна 1280 х 720
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
        tempX = 640;    // Убираем из вида левую сторону
    if (y < 100)
        tempY = 100;    // Убираем из вида верхнюю сторону
    if (y > 2000)
        tempY = 2000;    // Убираем из вида нижнюю сторону
    if (x > 2560)
        tempX = 2560;    // Убираем из вида правую сторону

    view.setCenter(tempX, tempY);    // Перемещение камеры
}
