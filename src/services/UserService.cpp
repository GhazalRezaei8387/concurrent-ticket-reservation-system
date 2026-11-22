
#include "services/UserService.hpp"

#include <stdexcept>

UserService::UserService(IUserRepository& userRepository)
    : userRepository_(userRepository) {
}

User UserService::registerUser(
    const std::string& fullName,
    const std::string& email
) {
    if (userRepository_.findByEmail(email).has_value()) {
        throw std::runtime_error("A user with this email already exists.");
    }

    return userRepository_.create(fullName, email);
}

std::optional<User> UserService::getUserById(std::int64_t userId) {
    return userRepository_.findById(userId);
}

std::optional<User> UserService::getUserByEmail(const std::string& email) {
    return userRepository_.findByEmail(email);
}
