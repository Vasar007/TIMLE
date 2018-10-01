#ifndef UTILITY_INL
#define UTILITY_INL

template <typename T>
std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

template <class T>
std::unique_ptr<T> copyUnique(const std::unique_ptr<T>& source)
{
    return source ? std::make_unique<T>(*source) : nullptr;
}

#endif // UTILITY_INL
