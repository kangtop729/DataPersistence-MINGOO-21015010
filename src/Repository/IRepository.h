#pragma once

#include <optional>
#include <vector>

template <typename T>
class IRepository
{
public:
    virtual ~IRepository() = default;

    virtual void Save(const T& entity) = 0;
    virtual std::vector<T> FindAll() const = 0;
    virtual std::optional<T> FindById(int id) const = 0;
    virtual bool Update(const T& entity) = 0;
    virtual bool Remove(int id) = 0;
};
