#include "JsonFileRepository.h"

#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "Serialization/ItemSerialization.h"

JsonFileRepository::JsonFileRepository(std::string filePath)
    : filePath(std::move(filePath))
{
}

std::vector<Item> JsonFileRepository::Load() const
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        return {};
    }

    nlohmann::json j;
    try
    {
        file >> j;
    }
    catch (const nlohmann::json::parse_error&)
    {
        throw std::runtime_error("Failed to parse JSON file: " + filePath);
    }

    return j.get<std::vector<Item>>();
}

void JsonFileRepository::Flush(const std::vector<Item>& items) const
{
    const nlohmann::json j = items;
    std::ofstream file(filePath);
    file << j.dump(2);
}

void JsonFileRepository::Save(const Item& item)
{
    auto items = Load();
    items.push_back(item);
    Flush(items);
}

std::vector<Item> JsonFileRepository::FindAll() const
{
    return Load();
}

std::optional<Item> JsonFileRepository::FindById(int id) const
{
    auto items = Load();
    const auto it = std::find_if(items.begin(), items.end(),
        [id](const Item& item) { return item.id == id; });

    if (it == items.end())
    {
        return std::nullopt;
    }
    return *it;
}

bool JsonFileRepository::Update(const Item& item)
{
    auto items = Load();
    const auto it = std::find_if(items.begin(), items.end(),
        [&item](const Item& existing) { return existing.id == item.id; });

    if (it == items.end())
    {
        return false;
    }

    *it = item;
    Flush(items);
    return true;
}

bool JsonFileRepository::Remove(int id)
{
    auto items = Load();
    const auto originalSize = items.size();

    items.erase(std::remove_if(items.begin(), items.end(),
        [id](const Item& item) { return item.id == id; }), items.end());

    if (items.size() == originalSize)
    {
        return false;
    }

    Flush(items);
    return true;
}
