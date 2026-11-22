#ifndef IUSER_REPOSITORY_HPP
#define IUSER_REPOSITORY_HPP

#include "domain/User.hpp"

#include <cstdint>
#include <optional>
#include <string>

class IUserRepository {
public:
    virtual ~IUserRepository() = default;

    virtual User create(
        const std::string& fullName,
        const std::string& email
    ) = 0;

    virtual std::optional<User> findById(
        std::int64_t userId
    ) = 0;

    virtual std::optional<User> findByEmail(
        const std::string& email
    ) = 0;
};

#endif
