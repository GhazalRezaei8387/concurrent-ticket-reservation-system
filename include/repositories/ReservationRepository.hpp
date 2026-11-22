#ifndef RESERVATION_REPOSITORY_HPP
#define RESERVATION_REPOSITORY_HPP

#include "database/DatabaseConnection.hpp"
#include "domain/Reservation.hpp"

#include <cstdint>
#include <optional>
#include <vector>

class ReservationRepository {
public:
    explicit ReservationRepository(DatabaseConnection& database);

    Reservation create(
        std::int64_t userId,
        std::int64_t seatId
    );

    std::optional<Reservation> findById(
        std::int64_t reservationId
    );

    std::vector<Reservation> findByUserId(
        std::int64_t userId
    );

    std::optional<Reservation> findActiveBySeatId(
        std::int64_t seatId
    );

private:
    DatabaseConnection& database_;
};

#endif
