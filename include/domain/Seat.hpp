#ifndef SEAT_HPP
#define SEAT_HPP

#include <cstdint>
#include <string>

class Seat {
public:
    Seat(
        std::int64_t seatId,
        std::int64_t eventId,
        const std::string& seatNumber,
        const std::string& seatType
    );

    std::int64_t getSeatId() const;
    std::int64_t getEventId() const;
    const std::string& getSeatNumber() const;
    const std::string& getSeatType() const;

private:
    std::int64_t seatId_;
    std::int64_t eventId_;
    std::string seatNumber_;
    std::string seatType_;
};

#endif
