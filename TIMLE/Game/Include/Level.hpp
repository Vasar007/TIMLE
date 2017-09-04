#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <SFML/Graphics.hpp>
#include "../TinyXML/tinyxml.h"


struct Object 
{
	std::string							mName;	// Переменная name типа string
	std::string							mType;	// type типа string
	sf::Rect<float>						mRect;	// Тип Rect с нецелыми значениями
	std::map<std::string, std::string>	mProperties;	// Создаём ассоциативный массив. Ключ - строковый тип, значение тоже строковый тип
	sf::Sprite							mSprite;	// Спрайт объекта


	int									getPropertyInt(std::string name);	// Номер свойства объекта в списке
	float								getPropertyFloat(std::string name);
	std::string							getPropertyString(std::string name);
};


struct Layer 
{
	int						mOpacity;	// Непрозрачность слоя
	std::vector<sf::Sprite> mTiles;	// Вектор, состоящий из тайлов
};


class Level 
{
	private:
		int					mWidth;
		int					mHeight;
		int					mTileWidth;
		int					mTileHeight;	// Переменные, которые будет хранить даные из tmx-файла об уровне
		int					mFirstTileID;	// Узнаём положение первого тайла
		sf::Rect<float>		mDrawingBounds;	// Размер части карты, которую рисуем
		sf::Texture			mTtilesetImage;	// Текстура карты
		std::vector<Layer>	mLayers;	// Массив слоёв
	
	
	public:
		int					mLevelNumber;	// Номер уровня
		std::vector<Object>	mObjects;	// Массив типа Объекты, который прописан выше
	
	
	public:
							Level() = default;
		bool				loadFromFile(std::string filename);	// Возвращает false, если не удалось загрузить уровень
		Object				getObject(std::string name);
		std::vector<Object> getObjects(std::string name);	// Выдаём объект в уровень
		std::vector<Object> getAllObjects() const;	// Выдаём все объекты в уровень
		void				drawAll(sf::RenderWindow& window);
		void				draw(sf::RenderWindow& window);
		sf::Vector2i		getTileSize() const;	// Получаем размер тайла
};

#endif // LEVEL_HPP
