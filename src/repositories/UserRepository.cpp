#include "repositories/UserRepository.hpp"

#include <chrono>

UserRepository::UserRepository(DatabaseConnection& database)
    : database_(database) {
}

User UserRepository::create(
    const std::string& fullName,
    const std::string& email
) {
    pqxx::work transaction(database_.getConnection());

    pqxx::result result = transaction.exec_params(
        "INSERT INTO users (full_name, email) "
        "VALUES ($1, $2) "
        "RETURNING user_id, full_name, email, "
        "EXTRACT(EPOCH FROM created_at)::double precision AS created_at_epoch;",
        fullName,
        email
    );

    const auto userId = result[0]["user_id"].as<std::int64_t>();

    const auto createdAtSeconds =
        result[0]["created_at_epoch"].as<double>();

    const auto createdAt =
        std::chrono::system_clock::time_point(
            std::chrono::duration_cast<std::chrono::system_clock::duration>(
                std::chrono::duration<double>(createdAtSeconds)
            )
        );

    User user(
        userId,
        result[0]["full_name"].as<std::string>(),
        result[0]["email"].as<std::string>(),
        createdAt
    );

    transaction.commit();

    return user;
}

std::optional<User> UserRepository::findById(std::int64_t userId) {
    pqxx::read_transaction transaction(database_.getConnection());

    pqxx::result result = transaction.exec_params(
        "SELECT user_id, full_name, email, "
        "EXTRACT(EPOCH FROM created_at)::double precision AS created_at_epoch "
        "FROM users "
        "WHERE user_id = $1;",
        userId
    );

    if (result.empty()) {
        transaction.commit();
        return std::nullopt;
    }

    const auto createdAtSeconds =
        result[0]["created_at_epoch"].as<double>();

    const auto createdAt =
        std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::duration<double>(createdAtSeconds)
        );

    User user(
        result[0]["user_id"].as<std::int64_t>(),
        result[0]["full_name"].as<std::string>(),
        result[0]["email"].as<std::string>(),
        std::chrono::system_clock::time_point(createdAt)
    );

    transaction.commit();

    return user;
}

std::optional<User> UserRepository::findByEmail(
    const std::string& email
) {
    pqxx::read_transaction transaction(database_.getConnection());

    pqxx::result result = transaction.exec_params(
        "SELECT user_id, full_name, email, "
        "EXTRACT(EPOCH FROM created_at)::double precision AS created_at_epoch "
        "FROM users "
        "WHERE email = $1;",
        email
    );

    if (result.empty()) {
        transaction.commit();
        return std::nullopt;
    }

    const auto createdAtSeconds =
        result[0]["created_at_epoch"].as<double>();

    const auto createdAt =
        std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::duration<double>(createdAtSeconds)
        );

    User user(
        result[0]["user_id"].as<std::int64_t>(),
        result[0]["full_name"].as<std::string>(),
        result[0]["email"].as<std::string>(),
        std::chrono::system_clock::time_point(createdAt)
    );

    transaction.commit();

    return user;
}
