#pragma once

#include <string>
#include "Repository/IRepository.h"
#include "Model/Item.h"

class JsonFileRepository : public IRepository<Item>
{
public:
    explicit JsonFileRepository(std::string filePath);

    void Save(const Item& item) override;
    std::vector<Item> FindAll() const override;
    std::optional<Item> FindById(int id) const override;
    bool Update(const Item& item) override;
    bool Remove(int id) override;

private:
    std::vector<Item> Load() const;
    void Flush(const std::vector<Item>& items) const;

    std::string filePath;
};
