#ifndef RESERVATION_HPP
#define RESERVATION_HPP

#include <chrono>
#include <cstdint>
#include <optional>

enum class ReservationStatus {
    Confirmed,
    Cancelled
};

class Reservation {
public:
    Reservation(
        std::int64_t reservationId,
        std::int64_t userId,
        std::int64_t seatId,
        ReservationStatus status,
        std::chrono::system_clock::time_point reservedAt,
        std::optional<std::chrono::system_clock::time_point> cancelledAt
    );

    std::int64_t getReservationId() const;
    std::int64_t getUserId() const;
    std::int64_t getSeatId() const;

    ReservationStatus getStatus() const;

    std::chrono::system_clock::time_point getReservedAt() const;

    std::optional<std::chrono::system_clock::time_point>
    getCancelledAt() const;

    void cancel(
        std::chrono::system_clock::time_point cancellationTime
    );

private:
    std::int64_t reservationId_;
    std::int64_t userId_;
    std::int64_t seatId_;

    ReservationStatus status_;

    std::chrono::system_clock::time_point reservedAt_;

    std::optional<std::chrono::system_clock::time_point> cancelledAt_;
};

#endif
