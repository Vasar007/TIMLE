#include "object.hpp"


int object::get_property_int(const std::string& name)
{
    // Возвращаем номер свойства в списке
    return std::stoi(properties.at(name).c_str());
}

float object::get_property_float(const std::string& name)
{
    return static_cast<float>(strtod(properties.at(name).c_str(), nullptr));
}

std::string object::get_property_string(const std::string& name)
{
    // Получаем имя в виде строки
    return properties.at(name);
}
