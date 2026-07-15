#pragma once

#include <string>

struct Item
{
    int id = 0;
    std::string name;
    int quantity = 0;

    bool operator==(const Item& other) const
    {
        return id == other.id && name == other.name && quantity == other.quantity;
    }
};
