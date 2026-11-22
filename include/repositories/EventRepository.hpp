#ifndef EVENT_REPOSITORY_HPP
#define EVENT_REPOSITORY_HPP

#include "database/DatabaseConnection.hpp"
#include "repositories/IEventRepository.hpp"

class EventRepository : public IEventRepository {
public:
    explicit EventRepository(DatabaseConnection& database);

    Event create(
        const std::string& title,
        const std::string& description,
        const std::string& location,
        std::chrono::system_clock::time_point eventDate
    ) override;

    std::optional<Event> findById(
        std::int64_t eventId
    ) override;

    std::vector<Event> findAll() override;

private:
    DatabaseConnection& database_;
};

#endif