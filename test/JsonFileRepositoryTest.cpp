#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "Repository/JsonFileRepository.h"

namespace
{
    std::filesystem::path MakeTempFilePath()
    {
        return std::filesystem::temp_directory_path() / "DataPersistenceTest_items.json";
    }
}

class JsonFileRepositoryTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        filePath = MakeTempFilePath().string();
        std::filesystem::remove(filePath);
    }

    void TearDown() override
    {
        std::filesystem::remove(filePath);
    }

    std::string filePath;
};

TEST_F(JsonFileRepositoryTest, FindAllOnMissingFileReturnsEmptyList)
{
    JsonFileRepository repo(filePath);

    EXPECT_TRUE(repo.FindAll().empty());
}

TEST_F(JsonFileRepositoryTest, SaveThenFindAllReturnsSavedItem)
{
    JsonFileRepository repo(filePath);

    repo.Save(Item{ 1, "Widget", 10 });

    auto items = repo.FindAll();
    ASSERT_EQ(items.size(), 1u);
    EXPECT_EQ(items[0], (Item{ 1, "Widget", 10 }));
}

TEST_F(JsonFileRepositoryTest, SavedDataPersistsAcrossRepositoryInstances)
{
    {
        JsonFileRepository repo(filePath);
        repo.Save(Item{ 1, "Widget", 10 });
    }

    JsonFileRepository reloaded(filePath);
    auto items = reloaded.FindAll();

    ASSERT_EQ(items.size(), 1u);
    EXPECT_EQ(items[0], (Item{ 1, "Widget", 10 }));
}

TEST_F(JsonFileRepositoryTest, FindByIdReturnsItemWhenFound)
{
    JsonFileRepository repo(filePath);
    repo.Save(Item{ 1, "Widget", 10 });

    auto found = repo.FindById(1);

    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(*found, (Item{ 1, "Widget", 10 }));
}

TEST_F(JsonFileRepositoryTest, FindByIdReturnsNulloptWhenNotFound)
{
    JsonFileRepository repo(filePath);
    repo.Save(Item{ 1, "Widget", 10 });

    EXPECT_FALSE(repo.FindById(999).has_value());
}

TEST_F(JsonFileRepositoryTest, UpdateExistingItemChangesStoredData)
{
    JsonFileRepository repo(filePath);
    repo.Save(Item{ 1, "Widget", 10 });

    const bool updated = repo.Update(Item{ 1, "Widget", 99 });

    EXPECT_TRUE(updated);
    ASSERT_TRUE(repo.FindById(1).has_value());
    EXPECT_EQ(repo.FindById(1)->quantity, 99);
}

TEST_F(JsonFileRepositoryTest, UpdateReturnsFalseWhenIdNotFound)
{
    JsonFileRepository repo(filePath);

    EXPECT_FALSE(repo.Update(Item{ 999, "Ghost", 1 }));
}

TEST_F(JsonFileRepositoryTest, RemoveExistingItemDeletesData)
{
    JsonFileRepository repo(filePath);
    repo.Save(Item{ 1, "Widget", 10 });

    const bool removed = repo.Remove(1);

    EXPECT_TRUE(removed);
    EXPECT_TRUE(repo.FindAll().empty());
}

TEST_F(JsonFileRepositoryTest, RemoveReturnsFalseWhenIdNotFound)
{
    JsonFileRepository repo(filePath);

    EXPECT_FALSE(repo.Remove(999));
}

TEST_F(JsonFileRepositoryTest, LoadingCorruptedFileThrows)
{
    {
        std::ofstream file(filePath);
        file << "{ not valid json ";
    }

    JsonFileRepository repo(filePath);

    EXPECT_THROW(repo.FindAll(), std::runtime_error);
}
