#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "Model/Item.h"
#include "Serialization/ItemSerialization.h"

TEST(ItemSerializationTest, ToJsonProducesExpectedFields)
{
    Item item{ 1, "Widget", 10 };

    nlohmann::json j = item;

    EXPECT_EQ(j.at("id").get<int>(), 1);
    EXPECT_EQ(j.at("name").get<std::string>(), "Widget");
    EXPECT_EQ(j.at("quantity").get<int>(), 10);
}

TEST(ItemSerializationTest, FromJsonReconstructsItem)
{
    nlohmann::json j = { {"id", 2}, {"name", "Gadget"}, {"quantity", 5} };

    Item item = j.get<Item>();

    EXPECT_EQ(item, (Item{ 2, "Gadget", 5 }));
}
