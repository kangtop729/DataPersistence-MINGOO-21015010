#ifdef _DEBUG
#include <gtest/gtest.h>
#else
#include <iostream>
#include <nlohmann/json.hpp>
#endif

int main(int argc, char** argv)
{
#ifdef _DEBUG
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#else
    nlohmann::json sample = { {"name", "DataPersistence PoC"} };
    std::cout << sample.dump() << std::endl;
    return 0;
#endif
}
