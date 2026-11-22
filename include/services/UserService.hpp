
#ifndef USER_SERVICE_HPP
#define USER_SERVICE_HPP

#include "repositories/IUserRepository.hpp"

#include <cstdint>
#include <optional>
#include <string>

class UserService {
public:
    explicit UserService(IUserRepository& userRepository);

    User registerUser(
        const std::string& fullName,
        const std::string& email
    );

    std::optional<User> getUserById(std::int64_t userId);

    std::optional<User> getUserByEmail(const std::string& email);

private:
    IUserRepository& userRepository_;
};

#endif
