#pragma once

#include <nlohmann/json.hpp>
#include "Model/Item.h"

void to_json(nlohmann::json& j, const Item& item);
void from_json(const nlohmann::json& j, Item& item);
