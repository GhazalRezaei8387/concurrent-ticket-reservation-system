
#ifndef USER_REPOSITORY_HPP
#define USER_REPOSITORY_HPP

#include "database/DatabaseConnection.hpp"
#include "repositories/IUserRepository.hpp"

class UserRepository : public IUserRepository {
public:
    explicit UserRepository(DatabaseConnection& database);

    User create(
        const std::string& fullName,
        const std::string& email
    ) override;

    std::optional<User> findById(
        std::int64_t userId
    ) override;

    std::optional<User> findByEmail(
        const std::string& email
    ) override;

private:
    DatabaseConnection& database_;
};

#endif
