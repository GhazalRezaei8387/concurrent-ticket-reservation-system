#include "database/DatabaseConnection.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    try {
        const char* connectionString = std::getenv("DATABASE_URL");

        if (connectionString == nullptr) {
            throw std::runtime_error(
                "DATABASE_URL environment variable is not set."
            );
        }

        DatabaseConnection database(connectionString);

        database.connect();

        std::cout << "Successfully connected to PostgreSQL!"
                  << std::endl;

        {
            pqxx::work transaction(database.getConnection());

            pqxx::result result = transaction.exec(
                "SELECT current_user, current_database();"
            );

            std::cout << "Database user: "
                      << result[0]["current_user"].c_str()
                      << std::endl;

            std::cout << "Database name: "
                      << result[0]["current_database"].c_str()
                      << std::endl;

            transaction.commit();
        } // Transaction is destroyed here.

        database.disconnect();

        std::cout << "Database connection closed successfully."
                  << std::endl;

    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}
