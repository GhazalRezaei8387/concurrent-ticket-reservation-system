#ifndef USER_HPP
#define USER_HPP

#include <chrono>
#include <cstdint>
#include <string>

class User {
public:
    User(
        std::int64_t userId,
        const std::string& fullName,
        const std::string& email,
        std::chrono::system_clock::time_point createdAt
    );

    std::int64_t getUserId() const;
    const std::string& getFullName() const;
    const std::string& getEmail() const;

    std::chrono::system_clock::time_point getCreatedAt() const;

private:
    std::int64_t userId_;
    std::string fullName_;
    std::string email_;
    std::chrono::system_clock::time_point createdAt_;
};

#endif
