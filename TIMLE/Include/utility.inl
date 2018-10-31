#ifndef UTILITY_INL
#define UTILITY_INL

namespace utils
{

template <class T>
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

template <class T>
typename std::vector<T>::iterator append(std::vector<T>& dest, const std::vector<T>& src)
{
    typename std::vector<T>::iterator result;

    if (dest.empty())
    {
        dest = src;
        result = std::begin(dest);
    }
    else
    {
        result = dest.insert(std::end(dest), std::begin(src), std::end(src));
    }

    return result;
}

template <class T>
typename std::vector<T>::iterator append(std::vector<T>& dest, std::vector<T>&& src)
{
    typename std::vector<T>::iterator result;

    if (dest.empty())
    {
        dest = std::move(src);
        result = std::begin(dest);
    }
    else
    {
        result = dest.insert(std::end(dest),
                             std::make_move_iterator(std::begin(src)),
                             std::make_move_iterator(std::end(src)));
    }

    src.clear();
    src.shrink_to_fit();

    return result;
}

} // namespace utils

#endif // UTILITY_INL
