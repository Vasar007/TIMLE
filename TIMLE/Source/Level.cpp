#include "../Include/Level.hpp"


int Object::getPropertyInt(const std::string& name)
{
	// ���������� ����� �������� � ������
	return atoi(mProperties[name].c_str());
}

float Object::getPropertyFloat(const std::string& name)
{
	return static_cast<float>(strtod(mProperties[name].c_str(), nullptr));
}

std::string Object::getPropertyString(const std::string& name)
{
	// �������� ��� � ���� ������
	return mProperties[name];
}


Level::Level()
: _width(0)
, _height(0)
, _tileWidth(0)
, _tileHeight(0)
, _firstTileID(0)
, mLevelNumber(0u)
{
}

bool Level::loadFromFile(const std::string& filename)
{
	//��������� ���� � TiXmlDocument
	TiXmlDocument levelFile(filename.c_str());

	if (!levelFile.LoadFile())
	{
		// ���� �� ������� ��������� �����, ������ ������
		std::cout << "Loading level \"" << filename << "\" failed.\n";
		return false;
	}

	TiXmlElement *map = levelFile.FirstChildElement("map");

	// ������ �����: <map version="1.0" orientation="orthogonal"
	// width="10" height="10" tilewidth="34" tileheight="34">

	// ��������� �� ����� ����� �� ��������, ������� �������� ��� ������ � ������� ���������
	_width		= atoi(map->Attribute("width"));
	_height		= atoi(map->Attribute("height"));
	_tileWidth	= atoi(map->Attribute("tilewidth"));
	_tileHeight = atoi(map->Attribute("tileheight"));

	// ����� �������� �������� � ������������� ������� �����
	TiXmlElement *tilesetElement = map->FirstChildElement("tileset");
	_firstTileID = atoi(tilesetElement->Attribute("firstgid"));

	// source - ���� �� �������� � ���������� image
	TiXmlElement *image			= tilesetElement->FirstChildElement("image");
	const std::string imagepath = image->Attribute("source");

	// �������� ��������� �������
	sf::Image img;

	if (!img.loadFromFile(imagepath))
	{
		// ���� �� ������� ��������� �������-������� ������ � �������
		std::cout << "Failed to load tile sheet.\n";
		return false;
	}

	// ��� ����� �����.������ ��� �����
	img.createMaskFromColor(sf::Color(152, 172, 226));
	_tilesetImage.loadFromImage(img);
	//�����������
	_tilesetImage.setSmooth(false);

	// �������� ���������� �������� � ����� ��������
	const int columns	= _tilesetImage.getSize().x / _tileWidth;
	const int rows		= _tilesetImage.getSize().y / _tileHeight;

	// ������ �� ��������������� ����������� (TextureRect)
	std::vector<sf::IntRect> subRects;

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < columns; ++x)
		{
			sf::IntRect rect;

			rect.top	= y * _tileHeight;
			rect.height = _tileHeight;
			rect.left	= x * _tileWidth;
			rect.width	= _tileWidth;

			subRects.push_back(rect);
		}
	}

	// ������ �� ������
	TiXmlElement *layerElement = map->FirstChildElement("layer");
	while (layerElement) 
	{
		Layer layer;

		// ���� ������������ opacity, �� ������ ������������ ����, ����� �� ��������� �����������
		if (layerElement->Attribute("opacity") != nullptr)
		{
			const auto opacity	= static_cast<float>(strtod(layerElement->Attribute("opacity"), nullptr));
			layer.mOpacity		= 255 * static_cast<int>(opacity);
		}
		else
		{
			layer.mOpacity	= 255;
		}

		// ��������� <data>
		TiXmlElement *layerDataElement = layerElement->FirstChildElement("data");

		if (layerDataElement == nullptr)
		{
			std::cout << "Bad map. No layer information found.\n";
		}

		// ��������� <tile> - �������� ������ ������� ����
		TiXmlElement *tileElement = layerDataElement->FirstChildElement("tile");

		if (tileElement == nullptr)
		{
			std::cout << "Bad map. No tile information found.\n";
			return false;
		}

		int x = 0;
		int y = 0;

		while (tileElement)
		{
			const int tileGID		= atoi(tileElement->Attribute("gid"));
			const int subRectToUse	= tileGID - _firstTileID;

			// ������������� TextureRect ������� �����
			if (subRectToUse >= 0)
			{
				sf::Sprite sprite;
				sprite.setTexture(_tilesetImage);
				sprite.setTextureRect(subRects.at(subRectToUse));
				sprite.setPosition(static_cast<float>(x * _tileWidth), 
								   static_cast<float>(y * _tileHeight));
				sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(layer.mOpacity)));

				// ���������� � ���� ������� ������
				layer.mTiles.push_back(std::move(sprite));
			}

			tileElement = tileElement->NextSiblingElement("tile");

			++x;
			if (x >= _width)
			{
				x = 0;
				++y;
				if (y >= _height)
				{
					y = 0;
				}
			}
		}

		_layers.push_back(std::move(layer));

		layerElement = layerElement->NextSiblingElement("layer");
	}



	// ���� ���� ���� ��������
	if (map->FirstChildElement("objectgroup") != nullptr)
	{
		// ������ � ���������
		TiXmlElement *objectGroupElement = map->FirstChildElement("objectgroup");
		while (objectGroupElement)
		{
			// ��������� <object>
			TiXmlElement *objectElement = objectGroupElement->FirstChildElement("object");

			while (objectElement)
			{
				// �������� ��� ������ - ���, ���, �������, � ��
				std::string objectType;
				if (objectElement->Attribute("type") != nullptr)
				{
					objectType = objectElement->Attribute("type");
				}
				std::string objectName;
				if (objectElement->Attribute("name") != nullptr)
				{
					objectName = objectElement->Attribute("name");
				}
				const int x = atoi(objectElement->Attribute("x"));
				const int y = atoi(objectElement->Attribute("y"));

				int width, height;

				sf::Sprite sprite;
				sprite.setTexture(_tilesetImage);
				sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
				sprite.setPosition(static_cast<float>(x), static_cast<float>(y));

				if (objectElement->Attribute("width") != nullptr)
				{
					width	= atoi(objectElement->Attribute("width"));
					height	= atoi(objectElement->Attribute("height"));
				}
				else
				{
					width	= subRects.at(atoi(objectElement->Attribute("gid")) 
										  - _firstTileID).width;
					height	= subRects.at(atoi(objectElement->Attribute("gid")) 
										  - _firstTileID).height;
					sprite.setTextureRect(subRects.at(atoi(objectElement->Attribute("gid")) 
										  - _firstTileID));
				}

				// ��������� ������� [
				Object object;
				object.mName	= objectName;
				object.mType	= objectType;
				object.mSprite	= sprite;

				sf::FloatRect objectRect;
				objectRect.top		= static_cast<float>(y);
				objectRect.left		= static_cast<float>(x);
				objectRect.height	= static_cast<float>(height);
				objectRect.width	= static_cast<float>(width);
				object.mRect		= objectRect;

				// "����������" ������� [
				TiXmlElement *properties = objectElement->FirstChildElement("properties");
				if (properties != nullptr)
				{
					TiXmlElement *prop = properties->FirstChildElement("property");
					if (prop != nullptr)
					{
						while (prop)
						{
							const std::string propertyName		= prop->Attribute("name");
							const std::string propertyValue		= prop->Attribute("value");

							object.mProperties.at(propertyName) = propertyValue;

							prop = prop->NextSiblingElement("property");
						}
					}
				}


				mObjects.push_back(std::move(object));

				objectElement = objectElement->NextSiblingElement("object");
			}
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else
	{
		std::cout << "No object layers found...\n";
	}

	return true;
}

Object Level::getObject(const std::string_view name) const
{
	Object result;
	// ������ ������ ������ � �������� ������
	for (const auto& object : mObjects)
	{
		if (object.mName == name)
		{
			result = object;
			return result;
		}
	}

	std::cout << "Error! Couldn't find object with this name!\n";
	return result;
}

std::vector<Object> Level::getObjects(const std::string_view name) const
{
	// ��� ������� � �������� ������
	std::vector<Object> result;
	for (const auto& object : mObjects)
	{
		if (object.mName == name)
		{
			result.push_back(object);
		}
	}

	return result;
}


std::vector<Object> Level::getAllObjects() const
{
	return mObjects;
}


sf::Vector2i Level::getTileSize() const
{
	return sf::Vector2i(_tileWidth, _tileHeight);
}

void Level::drawAll(sf::RenderWindow& window) const
{
	// ������ ��� ����� (������� �� ������!)
	for (const auto& layer : _layers)
	{
		for (const auto& mTile : layer.mTiles)
		{
			window.draw(mTile);
		}
	}
}

void Level::draw(sf::RenderWindow& window)
{
	const auto center	= window.getView().getCenter();
	const auto size		= window.getView().getSize();

	_drawingBounds = sf::FloatRect(center.x - (size.x / 2.f) - 25.f,
								   center.y - (size.y / 2.f) - 25.f, size.x + 25.f, size.y + 25.f);

	for (const auto& layer : _layers)
	{
		for (const auto& mTile : layer.mTiles)
		{
			if (_drawingBounds.contains(mTile.getPosition()))
			{
				window.draw(mTile);
			}
		}
	}
}
