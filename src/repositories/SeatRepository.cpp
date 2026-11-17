#include "repositories/SeatRepository.hpp"

SeatRepository::SeatRepository(DatabaseConnection& database)
    : database_(database) {
}

Seat SeatRepository::create(
    std::int64_t eventId,
    const std::string& seatNumber,
    const std::string& seatType
) {
    pqxx::work transaction(database_.getConnection());

    pqxx::result result = transaction.exec_params(
        "INSERT INTO seats (event_id, seat_number, seat_type) "
        "VALUES ($1, $2, $3) "
        "RETURNING seat_id, event_id, seat_number, seat_type;",
        eventId,
        seatNumber,
        seatType
    );

    Seat seat(
        result[0]["seat_id"].as<std::int64_t>(),
        result[0]["event_id"].as<std::int64_t>(),
        result[0]["seat_number"].as<std::string>(),
        result[0]["seat_type"].as<std::string>()
    );

    transaction.commit();

    return seat;
}

std::optional<Seat> SeatRepository::findById(std::int64_t seatId) {
    pqxx::read_transaction transaction(database_.getConnection());

    pqxx::result result = transaction.exec_params(
        "SELECT seat_id, event_id, seat_number, seat_type "
        "FROM seats "
        "WHERE seat_id = $1;",
        seatId
    );

    if (result.empty()) {
        transaction.commit();
        return std::nullopt;
    }

    Seat seat(
        result[0]["seat_id"].as<std::int64_t>(),
        result[0]["event_id"].as<std::int64_t>(),
        result[0]["seat_number"].as<std::string>(),
        result[0]["seat_type"].as<std::string>()
    );

    transaction.commit();

    return seat;
}

std::vector<Seat> SeatRepository::findByEventId(std::int64_t eventId) {
    pqxx::read_transaction transaction(database_.getConnection());

    pqxx::result result = transaction.exec_params(
        "SELECT seat_id, event_id, seat_number, seat_type "
        "FROM seats "
        "WHERE event_id = $1 "
        "ORDER BY seat_number ASC;",
        eventId
    );

    std::vector<Seat> seats;
    seats.reserve(result.size());

    for (const auto& row : result) {
        seats.emplace_back(
            row["seat_id"].as<std::int64_t>(),
            row["event_id"].as<std::int64_t>(),
            row["seat_number"].as<std::string>(),
            row["seat_type"].as<std::string>()
        );
    }

    transaction.commit();

    return seats;
}