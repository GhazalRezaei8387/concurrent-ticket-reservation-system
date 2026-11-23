#ifndef IRESERVATION_REPOSITORY_HPP
#define IRESERVATION_REPOSITORY_HPP

#include "domain/Reservation.hpp"

#include <cstdint>
#include <optional>
#include <vector>

class IReservationRepository {
public:
    virtual ~IReservationRepository() = default;

    virtual Reservation create(
        std::int64_t userId,
        std::int64_t seatId
    ) = 0;

    virtual std::optional<Reservation> findById(
        std::int64_t reservationId
    ) = 0;

    virtual std::vector<Reservation> findByUserId(
        std::int64_t userId
    ) = 0;

    virtual std::optional<Reservation> findActiveBySeatId(
        std::int64_t seatId
    ) = 0;

    virtual void cancel(
        std::int64_t reservationId,
        std::chrono::system_clock::time_point cancellationTime
    ) = 0;
};

#endif
