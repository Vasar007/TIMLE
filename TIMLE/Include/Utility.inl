#ifndef UTILITY_INL
#define UTILITY_INL

template <typename T>
std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

#endif // UTILITY_INL
