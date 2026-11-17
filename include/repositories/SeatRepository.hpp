#ifndef SEAT_REPOSITORY_HPP
#define SEAT_REPOSITORY_HPP

#include "database/DatabaseConnection.hpp"
#include "domain/Seat.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class SeatRepository {
public:
    explicit SeatRepository(DatabaseConnection& database);

    Seat create(
        std::int64_t eventId,
        const std::string& seatNumber,
        const std::string& seatType
    );

    std::optional<Seat> findById(std::int64_t seatId);

    std::vector<Seat> findByEventId(std::int64_t eventId);

private:
    DatabaseConnection& database_;
};

#endif