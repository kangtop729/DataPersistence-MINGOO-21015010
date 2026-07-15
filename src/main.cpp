#ifdef _DEBUG
#include <gtest/gtest.h>
#else
#include <iostream>
#include "Model/Item.h"
#include "Repository/JsonFileRepository.h"

namespace
{
    void PrintAll(const JsonFileRepository& repo)
    {
        for (const auto& item : repo.FindAll())
        {
            std::cout << "  [" << item.id << "] " << item.name << " x" << item.quantity << "\n";
        }
    }

    int NextId(const std::vector<Item>& items)
    {
        int maxId = 0;
        for (const auto& item : items)
        {
            maxId = std::max(maxId, item.id);
        }
        return maxId + 1;
    }
}
#endif

int main(int argc, char** argv)
{
#ifdef _DEBUG
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#else
    JsonFileRepository repo("items.json");

    std::cout << "Before:\n";
    PrintAll(repo);

    const int widgetId = NextId(repo.FindAll());
    const int gadgetId = widgetId + 1;

    repo.Save(Item{ widgetId, "Widget", 10 });
    repo.Save(Item{ gadgetId, "Gadget", 5 });
    repo.Update(Item{ widgetId, "Widget", 20 });
    repo.Remove(gadgetId);

    std::cout << "After Create/Update/Delete:\n";
    PrintAll(repo);

    return 0;
#endif
}
