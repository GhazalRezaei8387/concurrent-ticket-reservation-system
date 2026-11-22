#include "services/SeatService.hpp"

SeatService::SeatService(ISeatRepository& seatRepository)
    : seatRepository_(seatRepository) {
}

Seat SeatService::createSeat(
    std::int64_t eventId,
    const std::string& seatNumber,
    const std::string& seatType
) {
    return seatRepository_.create(
        eventId,
        seatNumber,
        seatType
    );
}

std::optional<Seat> SeatService::getSeatById(
    std::int64_t seatId
) {
    return seatRepository_.findById(seatId);
}

std::vector<Seat> SeatService::getSeatsByEventId(
    std::int64_t eventId
) {
    return seatRepository_.findByEventId(eventId);
}