
#ifndef RESERVATION_SERVICE_HPP
#define RESERVATION_SERVICE_HPP

#include "repositories/IReservationRepository.hpp"

#include <cstdint>
#include <optional>
#include <vector>

class ReservationService {
public:
    explicit ReservationService(
        IReservationRepository& reservationRepository
    );

    Reservation createReservation(
        std::int64_t userId,
        std::int64_t seatId
    );

    std::optional<Reservation> getReservationById(
        std::int64_t reservationId
    );

    std::vector<Reservation> getReservationsByUserId(
        std::int64_t userId
    );

    std::optional<Reservation> getActiveReservationBySeatId(
        std::int64_t seatId
    );

    void cancelReservation(std::int64_t reservationId);

private:
    IReservationRepository& reservationRepository_;
};

#endif
