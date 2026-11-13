#include <gtest/gtest.h>

#include "domain/User.hpp"

#include <chrono>
#include <cstdint>
#include <stdexcept>
#include <string>

using namespace std::chrono;

TEST(UserTest, CreatesUserWithValidData) {
    const auto now = system_clock::now();

    User user(
        1,
        "Ghazal Rezaei",
        "ghazal@example.com",
        now
    );

    EXPECT_EQ(user.getUserId(), 1);
    EXPECT_EQ(user.getFullName(), "Ghazal Rezaei");
    EXPECT_EQ(user.getEmail(), "ghazal@example.com");
    EXPECT_EQ(user.getCreatedAt(), now);
}

TEST(UserTest, RejectsZeroUserId) {
    EXPECT_THROW(
        User(
            0,
            "Test User",
            "test@example.com",
            system_clock::now()
        ),
        std::invalid_argument
    );
}

TEST(UserTest, RejectsNegativeUserId) {
    EXPECT_THROW(
        User(
            -1,
            "Test User",
            "test@example.com",
            system_clock::now()
        ),
        std::invalid_argument
    );
}

TEST(UserTest, RejectsEmptyFullName) {
    EXPECT_THROW(
        User(
            1,
            "",
            "test@example.com",
            system_clock::now()
        ),
        std::invalid_argument
    );
}

TEST(UserTest, RejectsWhitespaceOnlyFullName) {
    EXPECT_THROW(
        User(
            1,
            "   ",
            "test@example.com",
            system_clock::now()
        ),
        std::invalid_argument
    );
}

TEST(UserTest, RejectsEmptyEmail) {
    EXPECT_THROW(
        User(
            1,
            "Test User",
            "",
            system_clock::now()
        ),
        std::invalid_argument
    );
}
