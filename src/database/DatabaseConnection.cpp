#include "database/DatabaseConnection.hpp"

#include <stdexcept>
#include <utility>

DatabaseConnection::DatabaseConnection(
    const std::string& connectionString
)
    : connectionString_(connectionString),
      connection_(nullptr) {
}

void DatabaseConnection::connect() {
    if (isConnected()) {
        return;
    }

    connection_ = std::make_unique<pqxx::connection>(
        connectionString_
    );

    if (!connection_->is_open()) {
        connection_.reset();
        throw std::runtime_error(
            "Failed to connect to the database."
        );
    }
}

void DatabaseConnection::disconnect() {
    connection_.reset();
}

bool DatabaseConnection::isConnected() const {
    return connection_ && connection_->is_open();
}

pqxx::connection& DatabaseConnection::getConnection() {
    if (!isConnected()) {
        throw std::runtime_error(
            "Database connection is not open."
        );
    }

    return *connection_;
}
