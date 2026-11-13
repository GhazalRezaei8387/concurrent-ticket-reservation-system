#ifndef DATABASE_CONNECTION_HPP
#define DATABASE_CONNECTION_HPP

#include <pqxx/pqxx>
#include <memory>
#include <string>

class DatabaseConnection {
public:
    explicit DatabaseConnection(const std::string& connectionString);

    void connect();
    void disconnect();

    bool isConnected() const;

    pqxx::connection& getConnection();

private:
    std::string connectionString_;
    std::unique_ptr<pqxx::connection> connection_;
};

#endif
