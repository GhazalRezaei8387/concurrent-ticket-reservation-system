#ifndef EVENT_REPOSITORY_HPP
#define EVENT_REPOSITORY_HPP

#include "database/DatabaseConnection.hpp"
#include "domain/Event.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class EventRepository {
public:
    explicit EventRepository(DatabaseConnection& database);

    Event create(
        const std::string& title,
        const std::string& description,
        const std::string& location,
        std::chrono::system_clock::time_point eventDate
    );

    std::optional<Event> findById(std::int64_t eventId);

    std::vector<Event> findAll();

private:
    DatabaseConnection& database_;
};

#endif