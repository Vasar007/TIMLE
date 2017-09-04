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
	std::string							mName;	// ���������� name ���� string
	std::string							mType;	// type ���� string
	sf::Rect<float>						mRect;	// ��� Rect � �������� ����������
	std::map<std::string, std::string>	mProperties;	// ������ ������������� ������. ���� - ��������� ���, �������� ���� ��������� ���
	sf::Sprite							mSprite;	// ������ �������


	int									getPropertyInt(std::string name);	// ����� �������� ������� � ������
	float								getPropertyFloat(std::string name);
	std::string							getPropertyString(std::string name);
};


struct Layer 
{
	int						mOpacity;	// �������������� ����
	std::vector<sf::Sprite> mTiles;	// ������, ��������� �� ������
};


class Level 
{
	private:
		int					mWidth;
		int					mHeight;
		int					mTileWidth;
		int					mTileHeight;	// ����������, ������� ����� ������� ����� �� tmx-����� �� ������
		int					mFirstTileID;	// ����� ��������� ������� �����
		sf::Rect<float>		mDrawingBounds;	// ������ ����� �����, ������� ������
		sf::Texture			mTtilesetImage;	// �������� �����
		std::vector<Layer>	mLayers;	// ������ ����
	
	
	public:
		int					mLevelNumber;	// ����� ������
		std::vector<Object>	mObjects;	// ������ ���� �������, ������� �������� ����
	
	
	public:
							Level() = default;
		bool				loadFromFile(std::string filename);	// ���������� false, ���� �� ������� ��������� �������
		Object				getObject(std::string name);
		std::vector<Object> getObjects(std::string name);	// ����� ������ � �������
		std::vector<Object> getAllObjects() const;	// ����� ��� ������� � �������
		void				drawAll(sf::RenderWindow& window);
		void				draw(sf::RenderWindow& window);
		sf::Vector2i		getTileSize() const;	// �������� ������ �����
};

#endif // LEVEL_HPP
