#ifndef RESERVATION_REPOSITORY_HPP
#define RESERVATION_REPOSITORY_HPP

#include "database/DatabaseConnection.hpp"
#include "repositories/IReservationRepository.hpp"

#include <cstdint>
#include <optional>
#include <vector>

class ReservationRepository : public IReservationRepository {
public:
    explicit ReservationRepository(DatabaseConnection& database);

    Reservation create(
        std::int64_t userId,
        std::int64_t seatId
    ) override;

    std::optional<Reservation> findById(
        std::int64_t reservationId
    ) override;

    std::vector<Reservation> findByUserId(
        std::int64_t userId
    ) override;

    std::optional<Reservation> findActiveBySeatId(
        std::int64_t seatId
    ) override;

    void cancel(
        std::int64_t reservationId,
        std::chrono::system_clock::time_point cancellationTime
    ) override;

private:
    DatabaseConnection& database_;
};

#endif
