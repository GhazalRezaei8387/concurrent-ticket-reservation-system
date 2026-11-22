#ifndef SEAT_SERVICE_HPP
#define SEAT_SERVICE_HPP

#include "repositories/ISeatRepository.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class SeatService {
public:
    explicit SeatService(ISeatRepository& seatRepository);

    Seat createSeat(
        std::int64_t eventId,
        const std::string& seatNumber,
        const std::string& seatType
    );

    std::optional<Seat> getSeatById(std::int64_t seatId);

    std::vector<Seat> getSeatsByEventId(std::int64_t eventId);

private:
    ISeatRepository& seatRepository_;
};

#endif