
#include <gtest/gtest.h>

#include "database/DatabaseConnection.hpp"
#include "repositories/UserRepository.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>

std::string makeUniqueEmail(const std::string& prefix) {
    const auto timestamp =
        std::chrono::system_clock::now()
            .time_since_epoch()
            .count();

    return prefix + "_" + std::to_string(timestamp) + "@example.com";
}

class UserRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        const char* databaseUrl = std::getenv("DATABASE_URL");

        if (databaseUrl == nullptr) {
            GTEST_SKIP()
                << "DATABASE_URL environment variable is not set.";
        }

        database = std::make_unique<DatabaseConnection>(databaseUrl);
        database->connect();

        repository = std::make_unique<UserRepository>(*database);
    }

    void TearDown() override {
        repository.reset();

        if (database && database->isConnected()) {
            database->disconnect();
        }
    }

    std::unique_ptr<DatabaseConnection> database;
    std::unique_ptr<UserRepository> repository;
};

TEST_F(UserRepositoryTest, CreateUser) {
    const std::string email =
        makeUniqueEmail("repository_create_test");

    User user = repository->create("Repository Test User", email);

    EXPECT_GT(user.getUserId(), 0);
    EXPECT_EQ(user.getFullName(), "Repository Test User");
    EXPECT_EQ(user.getEmail(), email);
}

TEST_F(UserRepositoryTest, FindExistingUserById) {
    const std::string email =
        makeUniqueEmail("repository_find_id_test");

    User createdUser = repository->create("Find By ID User", email);

    auto foundUser = repository->findById(createdUser.getUserId());

    ASSERT_TRUE(foundUser.has_value());
    EXPECT_EQ(foundUser->getUserId(), createdUser.getUserId());
    EXPECT_EQ(foundUser->getEmail(), email);
}

TEST_F(UserRepositoryTest, FindExistingUserByEmail) {
    const std::string email =
        makeUniqueEmail("repository_find_email_test");

    User createdUser = repository->create("Find By Email User", email);

    auto foundUser = repository->findByEmail(email);

    ASSERT_TRUE(foundUser.has_value());
    EXPECT_EQ(foundUser->getUserId(), createdUser.getUserId());
    EXPECT_EQ(foundUser->getFullName(), "Find By Email User");
}

TEST_F(UserRepositoryTest, ReturnEmptyWhenUserDoesNotExist) {
    const std::string email =
        makeUniqueEmail("nonexistent_user");

    auto foundUser = repository->findByEmail(email);

    EXPECT_FALSE(foundUser.has_value());
}
