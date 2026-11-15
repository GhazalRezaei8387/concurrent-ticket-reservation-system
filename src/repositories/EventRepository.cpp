#include "repositories/EventRepository.hpp"

#include <chrono>

EventRepository::EventRepository(DatabaseConnection& database)
    : database_(database) {
}

Event EventRepository::create(
    const std::string& title,
    const std::string& description,
    const std::string& location,
    std::chrono::system_clock::time_point eventDate
) {
    pqxx::work transaction(database_.getConnection());

    const auto eventDateSeconds =
        std::chrono::duration<double>(
            eventDate.time_since_epoch()
        ).count();

    pqxx::result result = transaction.exec_params(
        "INSERT INTO events (title, description, location, event_date) "
        "VALUES ($1, $2, $3, to_timestamp($4)) "
        "RETURNING event_id, title, description, location, "
        "EXTRACT(EPOCH FROM event_date)::double precision AS event_date_epoch;",
        title,
        description,
        location,
        eventDateSeconds
    );

    const auto storedEventDateSeconds =
        result[0]["event_date_epoch"].as<double>();

    const auto storedEventDate =
        std::chrono::system_clock::time_point(
            std::chrono::duration_cast<std::chrono::system_clock::duration>(
                std::chrono::duration<double>(storedEventDateSeconds)
            )
        );

    Event event(
        result[0]["event_id"].as<std::int64_t>(),
        result[0]["title"].as<std::string>(),
        result[0]["description"].as<std::string>(),
        result[0]["location"].as<std::string>(),
        storedEventDate
    );

    transaction.commit();

    return event;
}

std::optional<Event> EventRepository::findById(std::int64_t eventId) {
    pqxx::read_transaction transaction(database_.getConnection());

    pqxx::result result = transaction.exec_params(
        "SELECT event_id, title, description, location, "
        "EXTRACT(EPOCH FROM event_date)::double precision AS event_date_epoch "
        "FROM events "
        "WHERE event_id = $1;",
        eventId
    );

    if (result.empty()) {
        transaction.commit();
        return std::nullopt;
    }

    const auto eventDateSeconds =
        result[0]["event_date_epoch"].as<double>();

    const auto eventDate =
        std::chrono::system_clock::time_point(
            std::chrono::duration_cast<std::chrono::system_clock::duration>(
                std::chrono::duration<double>(eventDateSeconds)
            )
        );

    Event event(
        result[0]["event_id"].as<std::int64_t>(),
        result[0]["title"].as<std::string>(),
        result[0]["description"].as<std::string>(),
        result[0]["location"].as<std::string>(),
        eventDate
    );

    transaction.commit();

    return event;
}

std::vector<Event> EventRepository::findAll() {
    pqxx::read_transaction transaction(database_.getConnection());

    pqxx::result result = transaction.exec(
        "SELECT event_id, title, description, location, "
        "EXTRACT(EPOCH FROM event_date)::double precision AS event_date_epoch "
        "FROM events "
        "ORDER BY event_date ASC;"
    );

    std::vector<Event> events;
    events.reserve(result.size());

    for (const auto& row : result) {
        const auto eventDateSeconds =
            row["event_date_epoch"].as<double>();

        const auto eventDate =
            std::chrono::system_clock::time_point(
                std::chrono::duration_cast<std::chrono::system_clock::duration>(
                    std::chrono::duration<double>(eventDateSeconds)
                )
            );

        events.emplace_back(
            row["event_id"].as<std::int64_t>(),
            row["title"].as<std::string>(),
            row["description"].as<std::string>(),
            row["location"].as<std::string>(),
            eventDate
        );
    }

    transaction.commit();

    return events;
}