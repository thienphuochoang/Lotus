#include "Collection.h"
template <typename T>
Collection<T>::Collection(int size)
{
    data.resize(size);
}
template <typename T>
bool Collection<T>::IsEmpty() const
{
    return data.empty();
}

template<typename T>
int Collection<T>::GetSize() const
{
    return data.size();
}

template<typename T>
void Collection<T>::Resize(int n)
{
    data.resize(n);
}

template<typename T>
void Collection<T>::Clear()
{
    data.clear();
}

template<typename T>
void Collection<T>::Add(T object)
{
    data.push_back(object);
}

template<typename T>
void Collection<T>::Set(int index, T object)
{
    data[index] = object;
}

template<typename T>
T& Collection<T>::Get(int index)
{
    return static_cast<T>(data[index]);
}

template<typename T>
T& Collection<T>::operator [](unsigned int index)
{
    return data[index];
}
