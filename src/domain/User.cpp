#include "domain/User.hpp"

#include <stdexcept>

User::User(
    std::int64_t userId,
    const std::string& fullName,
    const std::string& email,
    std::chrono::system_clock::time_point createdAt
)
    : userId_(userId),
      fullName_(fullName),
      email_(email),
      createdAt_(createdAt) {

    if (userId_ <= 0) {
        throw std::invalid_argument(
            "User ID must be positive."
        );
    }

    if (fullName_.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument(
            "User full name cannot be empty."
        );
    }

    if (email_.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument(
            "User email cannot be empty."
        );
    }
}

std::int64_t User::getUserId() const {
    return userId_;
}

const std::string& User::getFullName() const {
    return fullName_;
}

const std::string& User::getEmail() const {
    return email_;
}

std::chrono::system_clock::time_point User::getCreatedAt() const {
    return createdAt_;
}
