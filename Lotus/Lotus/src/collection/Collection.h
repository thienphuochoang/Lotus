#pragma once
#include <vector>
// A collection is just a vector (contiguous data) of objects of type T
class ICollection
{
public:
    virtual ~ICollection() {};
};
template <typename T>
class Collection : public ICollection
{
private:
    std::vector<T> data;
public:
    Collection(int size = 100);
    virtual ~Collection() = default;
    bool IsEmpty() const;
    int GetSize() const;
    void Resize(int n);
    void Clear();
    void Add(T object);
    void Set(int index, T object);
    T& Get(int index);
    T& operator [](unsigned int index);
};