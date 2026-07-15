#include "ItemSerialization.h"

void to_json(nlohmann::json& j, const Item& item)
{
    j = nlohmann::json{
        {"id", item.id},
        {"name", item.name},
        {"quantity", item.quantity}
    };
}

void from_json(const nlohmann::json& j, Item& item)
{
    j.at("id").get_to(item.id);
    j.at("name").get_to(item.name);
    j.at("quantity").get_to(item.quantity);
}
