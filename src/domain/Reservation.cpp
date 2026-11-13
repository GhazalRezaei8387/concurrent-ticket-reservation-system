#include "domain/Reservation.hpp"

#include <stdexcept>

Reservation::Reservation(
    std::int64_t reservationId,
    std::int64_t userId,
    std::int64_t seatId,
    ReservationStatus status,
    std::chrono::system_clock::time_point reservedAt,
    std::optional<std::chrono::system_clock::time_point> cancelledAt
)
    : reservationId_(reservationId),
      userId_(userId),
      seatId_(seatId),
      status_(status),
      reservedAt_(reservedAt),
      cancelledAt_(cancelledAt) {

    if (reservationId_ <= 0) {
        throw std::invalid_argument(
            "Reservation ID must be positive."
        );
    }

    if (userId_ <= 0) {
        throw std::invalid_argument(
            "User ID must be positive."
        );
    }

    if (seatId_ <= 0) {
        throw std::invalid_argument(
            "Seat ID must be positive."
        );
    }

    if (
        (status_ == ReservationStatus::Confirmed && cancelledAt_.has_value()) ||
        (status_ == ReservationStatus::Cancelled && !cancelledAt_.has_value())
    ) {
        throw std::invalid_argument(
            "Reservation status and cancellation time are inconsistent."
        );
    }
}

std::int64_t Reservation::getReservationId() const {
    return reservationId_;
}

std::int64_t Reservation::getUserId() const {
    return userId_;
}

std::int64_t Reservation::getSeatId() const {
    return seatId_;
}

ReservationStatus Reservation::getStatus() const {
    return status_;
}

std::chrono::system_clock::time_point Reservation::getReservedAt() const {
    return reservedAt_;
}

std::optional<std::chrono::system_clock::time_point>
Reservation::getCancelledAt() const {
    return cancelledAt_;
}

void Reservation::cancel(
    std::chrono::system_clock::time_point cancellationTime
) {
    if (status_ == ReservationStatus::Cancelled) {
        throw std::logic_error(
            "Reservation is already cancelled."
        );
    }

    if (cancellationTime < reservedAt_) {
        throw std::invalid_argument(
            "Cancellation time cannot be earlier than reservation time."
        );
    }

    status_ = ReservationStatus::Cancelled;
    cancelledAt_ = cancellationTime;
}
