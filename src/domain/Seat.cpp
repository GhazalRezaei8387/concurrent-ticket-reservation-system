#include "domain/Seat.hpp"

#include <stdexcept>

Seat::Seat(
    std::int64_t seatId,
    std::int64_t eventId,
    const std::string& seatNumber,
    const std::string& seatType
)
    : seatId_(seatId),
      eventId_(eventId),
      seatNumber_(seatNumber),
      seatType_(seatType) {

    if (seatId_ <= 0) {
        throw std::invalid_argument(
            "Seat ID must be positive."
        );
    }

    if (eventId_ <= 0) {
        throw std::invalid_argument(
            "Event ID must be positive."
        );
    }

    if (seatNumber_.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument(
            "Seat number cannot be empty."
        );
    }

    if (seatType_.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument(
            "Seat type cannot be empty."
        );
    }
}

std::int64_t Seat::getSeatId() const {
    return seatId_;
}

std::int64_t Seat::getEventId() const {
    return eventId_;
}

const std::string& Seat::getSeatNumber() const {
    return seatNumber_;
}

const std::string& Seat::getSeatType() const {
    return seatType_;
}
