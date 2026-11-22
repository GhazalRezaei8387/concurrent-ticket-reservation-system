#include <gtest/gtest.h>

#include <stdexcept>

#include "services/UserService.hpp"

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class FakeUserRepository : public IUserRepository {
public:
    User create(
        const std::string& fullName,
        const std::string& email
    ) override {
        User user(
            nextId++,
            fullName,
            email,
            std::chrono::system_clock::now()
        );

        users.push_back(user);
        return user;
    }

    std::optional<User> findById(
        std::int64_t userId
    ) override {
        for (const auto& user : users) {
            if (user.getUserId() == userId) {
                return user;
            }
        }

        return std::nullopt;
    }

    std::optional<User> findByEmail(
        const std::string& email
    ) override {
        for (const auto& user : users) {
            if (user.getEmail() == email) {
                return user;
            }
        }

        return std::nullopt;
    }

private:
    std::vector<User> users;
    std::int64_t nextId = 1;
};


TEST(UserServiceTest, RegistersNewUser) {
    FakeUserRepository repository;
    UserService service(repository);

    User user = service.registerUser(
        "Ghazal Rezaei",
        "ghazal@example.com"
    );

    EXPECT_GT(user.getUserId(), 0);
    EXPECT_EQ(user.getFullName(), "Ghazal Rezaei");
    EXPECT_EQ(user.getEmail(), "ghazal@example.com");
}

TEST(UserServiceTest, RejectsDuplicateEmail) {
    FakeUserRepository repository;
    UserService service(repository);

    service.registerUser(
        "First User",
        "test@example.com"
    );

    EXPECT_THROW(
        service.registerUser(
            "Second User",
            "test@example.com"
        ),
        std::runtime_error
    );
}

TEST(UserServiceTest, FindsUserById) {
    FakeUserRepository repository;
    UserService service(repository);

    User createdUser = service.registerUser(
        "Test User",
        "test@example.com"
    );

    const auto foundUser =
        service.getUserById(createdUser.getUserId());

    ASSERT_TRUE(foundUser.has_value());
    EXPECT_EQ(foundUser->getEmail(), "test@example.com");
}

TEST(UserServiceTest, FindsUserByEmail) {
    FakeUserRepository repository;
    UserService service(repository);

    service.registerUser(
        "Test User",
        "test@example.com"
    );

    const auto foundUser =
        service.getUserByEmail("test@example.com");

    ASSERT_TRUE(foundUser.has_value());
    EXPECT_EQ(foundUser->getFullName(), "Test User");
}
