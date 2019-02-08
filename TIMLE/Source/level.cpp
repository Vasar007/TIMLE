#include <iostream>

#include "utility.hpp"

#include "level.hpp"


level::level()
: _width(0)
, _height(0)
, _tile_width(0)
, _tile_height(0)
, _first_tile_id(0)
, level_number(0)
{
}

bool level::load_from_file(const std::string& filename, const std::string& tile_sheet_path)
{
    //Загружаем файл в TiXmlDocument
    TiXmlDocument level_file(filename.c_str());

    if (!level_file.LoadFile())
    {
        // Если не удалось загрузить карту, выдаем ошибку
        std::cerr << "Loading level \"" << filename << "\" failed.\n";
        return false;
    }

    const TiXmlElement* const map = level_file.FirstChildElement("map");

    // Пример карты: <map version="1.0" orientation="orthogonal"
    // width="10" height="10" tilewidth="34" tileheight="34">

    // Извлекаем из нашей карты ее свойства, которые задавали при работе в Тайлмап редакторе
    _width = utils::stou(map->Attribute("width"));
    _height = utils::stou(map->Attribute("height"));
    _tile_width = utils::stou(map->Attribute("tilewidth"));
    _tile_height = utils::stou(map->Attribute("tileheight"));

    // Берем описание тайлсета и идентификатор первого тайла
    const TiXmlElement* const tileset_element = map->FirstChildElement("tileset");
    _first_tile_id = std::stoi(tileset_element->Attribute("firstgid"));

    // source - путь до картинки в контейнере image
    const TiXmlElement* const image = tileset_element->FirstChildElement("image");

    const std::string imagepath = tile_sheet_path + image->Attribute("source");

    // Пытаемся загрузить тайлсет
    sf::Image img;

    if (!img.loadFromFile(imagepath))
    {
        // Если не удалось загрузить тайлсет-выводим ошибку в консоль
        std::cerr << "Failed to load tile sheet.\n";
        return false;
    }

    // Для маски цвета
    img.createMaskFromColor(sf::Color(152, 172, 226));
    _tileset.loadFromImage(img);
    //Сглаживание
    _tileset.setSmooth(false);

    // Получаем количество столбцов и строк тайлсета
    const int columns = _tileset.getSize().x / _tile_width;
    const int rows = _tileset.getSize().y / _tile_height;

    // Вектор из прямоугольников изображений (TextureRect)
    std::vector<sf::IntRect> sub_rects;
    sub_rects.reserve(rows * columns);
    for (int j = 0; j < rows; ++j)
    {
        for (int i = 0; i < columns; ++i)
        {
            // Rect: left, top, width, height.
            sub_rects.emplace_back(i * _tile_width, j * _tile_height, _tile_width, _tile_height);
        }
    }

    const auto tile_size = get_tile_size();

    // Работа со слоями
    const TiXmlElement* layer_element = map->FirstChildElement("layer");
    while (layer_element) 
    {
        layer one_layer(_tileset, tile_size, _width, _height);

        // Если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
        if (layer_element->Attribute("opacity") != nullptr)
        {
            const auto opacity = static_cast<float>(strtod(layer_element->Attribute("opacity"),
                                                           nullptr));
            one_layer.opacity = static_cast<int>(255 * opacity);
        }
        else
        {
            one_layer.opacity = 255;
        }

        // Контейнер <data>
        const TiXmlElement* const layer_data_element = layer_element->FirstChildElement("data");

        if (layer_data_element == nullptr)
        {
            std::cerr << "Bad map. No layer information found.\n";
            return false;
        }

        // Контейнер <tile> - описание тайлов каждого слоя
        const TiXmlElement* tile_element = layer_data_element->FirstChildElement("tile");

        if (tile_element == nullptr)
        {
            std::cerr << "Bad map. No tile information found.\n";
            return false;
        }

        unsigned int x = 0;
        unsigned int y = 0;

        while (tile_element)
        {
            int tileGID = 0;
            if (const char* const attribute = tile_element->Attribute("gid"); attribute != nullptr)
            {
                tileGID = std::stoi(attribute);
            }
            
            // Устанавливаем TextureRect каждого тайла
            if (const int sub_rect_to_use = tileGID - _first_tile_id; sub_rect_to_use >= 0)
            {
                const sf::Color color(255, 255, 255, static_cast<sf::Uint8>(one_layer.opacity));
                // Закидываем в слой спрайты тайлов
                one_layer.tiles.add_tile(sub_rects.at(sub_rect_to_use), x, y, color);
            }

            tile_element = tile_element->NextSiblingElement("tile");

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

        _layers.push_back(std::move(one_layer));

        layer_element = layer_element->NextSiblingElement("layer");
    }


    // Если есть слои объектов
    if (map->FirstChildElement("objectgroup") != nullptr)
    {
        // Работа с объектами
        const TiXmlElement* object_group_element = map->FirstChildElement("objectgroup");
        while (object_group_element)
        {
            // Контейнер <object>
            const TiXmlElement* object_element = object_group_element->FirstChildElement("object");

            while (object_element)
            {
                // Получаем все данные - тип, имя, позиция, и тд
                std::string objectType;
                if (object_element->Attribute("type") != nullptr)
                {
                    objectType = object_element->Attribute("type");
                }
                std::string objectName;
                if (object_element->Attribute("name") != nullptr)
                {
                    objectName = object_element->Attribute("name");
                }
                const int object_id = std::stoi(object_element->Attribute("id"));
                const int x = std::stoi(object_element->Attribute("x"));
                const int y = std::stoi(object_element->Attribute("y"));

                sf::Sprite sprite; // TOOD: check if we need this sprite in object section.
                sprite.setTexture(_tileset);
                sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
                sprite.setPosition(static_cast<float>(x), static_cast<float>(y));

                int width;
                int height;
                if (object_element->Attribute("width") != nullptr)
                {
                    width = std::stoi(object_element->Attribute("width"));
                    height = std::stoi(object_element->Attribute("height"));
                }
                else
                {
                    width = sub_rects.at(std::stoi(object_element->Attribute("gid")) - _first_tile_id).width;
                    height = sub_rects.at(std::stoi(object_element->Attribute("gid")) - _first_tile_id).height;
                    sprite.setTextureRect(sub_rects.at(std::stoi(object_element->Attribute("gid")) - _first_tile_id));
                }

                // Экземпляр объекта [
                object object;
                object.id = object_id;
                object.name = objectName;
                object.type = objectType;
                object.sprite = sprite;

                object.rect = sf::FloatRect(static_cast<float>(x), static_cast<float>(y),
                                             static_cast<float>(width), static_cast<float>(height));

                // "Переменные" объекта [
                const TiXmlElement* const properties = object_element->FirstChildElement("properties");
                if (properties != nullptr)
                {
                    const TiXmlElement* prop = properties->FirstChildElement("property");
                    if (prop != nullptr)
                    {
                        while (prop)
                        {
                            const std::string property_name = prop->Attribute("name");
                            object.properties.at(property_name) = prop->Attribute("value");

                            prop = prop->NextSiblingElement("property");
                        }
                    }
                }

                objects.push_back(std::move(object));
                object_element = object_element->NextSiblingElement("object");
            }
            object_group_element = object_group_element->NextSiblingElement("objectgroup");
        }
    }
    else
    {
        std::cerr << "No object layers found...\n";
    }

    return true;
}

object level::get_object(const std::string_view name) const
{
    // Только первый объект с заданным именем
    for (const auto& object : objects)
    {
        if (object.name == name)
        {
            return object;
        }
    }

    std::cerr << "Error! Couldn't find object with '"<< name << "' name!\n";
    throw std::invalid_argument(name.data());
}

std::vector<object> level::get_objects(const std::string_view name) const
{
    // Все объекты с заданным именем
    std::vector<object> result;
    for (const auto& object : objects)
    {
        if (object.name == name)
        {
            result.push_back(object);
        }
    }

    return result;
}


std::vector<object> level::get_all_objects() const noexcept
{
    return objects;
}


sf::Vector2u level::get_tile_size() const noexcept
{
    return sf::Vector2u(_tile_width, _tile_height);
}

void level::draw_all(sf::RenderWindow& window) const
{
    // Рисуем все тайлы (объекты не рисуем!)
    for (const auto& one_layer : _layers)
    {
        window.draw(one_layer.tiles);
    }
}

void level::draw(sf::RenderWindow& window)
{
    const auto& center = window.getView().getCenter();
    const auto& size = window.getView().getSize();

    constexpr float offset = 25.f;
    _drawing_bounds = sf::FloatRect(
        center.x - (size.x / 2.f) - offset, center.y - (size.y / 2.f) - offset,
        size.x + offset, size.y + offset
    );

    for (const auto& one_layer : _layers)
    {
        one_layer.tiles.draw_rect(window, static_cast<sf::IntRect>(_drawing_bounds));
    }
}
