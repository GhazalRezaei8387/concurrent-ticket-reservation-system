
#include "services/ReservationService.hpp"

#include <chrono>
#include <stdexcept>

ReservationService::ReservationService(
    IReservationRepository& reservationRepository
)
    : reservationRepository_(reservationRepository) {
}

Reservation ReservationService::createReservation(
    std::int64_t userId,
    std::int64_t seatId
) {
    if (userId <= 0 || seatId <= 0) {
        throw std::invalid_argument(
            "User ID and Seat ID must be positive."
        );
    }

    const auto activeReservation =
        reservationRepository_.findActiveBySeatId(seatId);

    if (activeReservation.has_value()) {
        throw std::runtime_error(
            "This seat already has an active reservation."
        );
    }

    return reservationRepository_.create(userId, seatId);
}

std::optional<Reservation> ReservationService::getReservationById(
    std::int64_t reservationId
) {
    return reservationRepository_.findById(reservationId);
}

std::vector<Reservation> ReservationService::getReservationsByUserId(
    std::int64_t userId
) {
    return reservationRepository_.findByUserId(userId);
}

std::optional<Reservation>
ReservationService::getActiveReservationBySeatId(
    std::int64_t seatId
) {
    return reservationRepository_.findActiveBySeatId(seatId);
}

void ReservationService::cancelReservation(
    std::int64_t reservationId
) {
    const auto reservation =
        reservationRepository_.findById(reservationId);

    if (!reservation.has_value()) {
        throw std::runtime_error("Reservation not found.");
    }

    const auto cancellationTime =
        std::chrono::system_clock::now();

    Reservation reservationToCancel = reservation.value();

    reservationToCancel.cancel(cancellationTime);

    reservationRepository_.cancel(
        reservationId,
        cancellationTime
    );
}
