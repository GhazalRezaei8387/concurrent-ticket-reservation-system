#ifndef USER_REPOSITORY_HPP
#define USER_REPOSITORY_HPP

#include "database/DatabaseConnection.hpp"
#include "domain/User.hpp"

#include <cstdint>
#include <optional>
#include <string>

class UserRepository {
public:
    explicit UserRepository(DatabaseConnection& database);

    User create(
        const std::string& fullName,
        const std::string& email
    );

    std::optional<User> findById(std::int64_t userId);

    std::optional<User> findByEmail(const std::string& email);

private:
    DatabaseConnection& database_;
};

#endif