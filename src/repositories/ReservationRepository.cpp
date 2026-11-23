
#include "repositories/ReservationRepository.hpp"

#include <stdexcept>
#include <string>

ReservationRepository::ReservationRepository(DatabaseConnection& database)
    : database_(database) {
}

Reservation ReservationRepository::create(
    std::int64_t userId,
    std::int64_t seatId
) {
    pqxx::work transaction(database_.getConnection());

    auto result = transaction.exec_params(
        "INSERT INTO reservations (user_id, seat_id) "
        "VALUES ($1, $2) "
        "RETURNING reservation_id, user_id, seat_id, status, "
        "EXTRACT(EPOCH FROM reserved_at)::double precision AS reserved_at_epoch, "
        "EXTRACT(EPOCH FROM cancelled_at)::double precision AS cancelled_at_epoch",
        userId,
        seatId
    );

    const auto& row = result[0];

    const auto reservedAt =
        std::chrono::system_clock::time_point{
            std::chrono::duration_cast<std::chrono::system_clock::duration>(
                std::chrono::duration<double>(
                    row["reserved_at_epoch"].as<double>()
                )
            )
        };

    std::optional<std::chrono::system_clock::time_point> cancelledAt;

    if (!row["cancelled_at_epoch"].is_null()) {
        cancelledAt = std::chrono::system_clock::time_point{
            std::chrono::duration_cast<std::chrono::system_clock::duration>(
                std::chrono::duration<double>(
                    row["cancelled_at_epoch"].as<double>()
                )
            )
        };
    }

    Reservation reservation(
        row["reservation_id"].as<std::int64_t>(),
        row["user_id"].as<std::int64_t>(),
        row["seat_id"].as<std::int64_t>(),
        ReservationStatus::Confirmed,
        reservedAt,
        cancelledAt
    );

    transaction.commit();

    return reservation;
}

std::optional<Reservation> ReservationRepository::findById(
    std::int64_t reservationId
) {
    pqxx::read_transaction transaction(database_.getConnection());

    auto result = transaction.exec_params(
        "SELECT reservation_id, user_id, seat_id, status, "
        "EXTRACT(EPOCH FROM reserved_at)::double precision AS reserved_at_epoch, "
        "EXTRACT(EPOCH FROM cancelled_at)::double precision AS cancelled_at_epoch "
        "FROM reservations "
        "WHERE reservation_id = $1",
        reservationId
    );

    if (result.empty()) {
        return std::nullopt;
    }

    const auto& row = result[0];

    const auto reservedAt =
        std::chrono::system_clock::time_point{
            std::chrono::duration_cast<std::chrono::system_clock::duration>(
                std::chrono::duration<double>(
                    row["reserved_at_epoch"].as<double>()
                )
            )
        };

    std::optional<std::chrono::system_clock::time_point> cancelledAt;

    if (!row["cancelled_at_epoch"].is_null()) {
        cancelledAt = std::chrono::system_clock::time_point{
            std::chrono::duration_cast<std::chrono::system_clock::duration>(
                std::chrono::duration<double>(
                    row["cancelled_at_epoch"].as<double>()
                )
            )
        };
    }

    const std::string status = row["status"].as<std::string>();

    const ReservationStatus reservationStatus =
        status == "CONFIRMED"
            ? ReservationStatus::Confirmed
            : ReservationStatus::Cancelled;

    return Reservation(
        row["reservation_id"].as<std::int64_t>(),
        row["user_id"].as<std::int64_t>(),
        row["seat_id"].as<std::int64_t>(),
        reservationStatus,
        reservedAt,
        cancelledAt
    );
}

std::vector<Reservation> ReservationRepository::findByUserId(
    std::int64_t userId
) {
    pqxx::read_transaction transaction(database_.getConnection());

    auto result = transaction.exec_params(
        "SELECT reservation_id, user_id, seat_id, status, "
        "EXTRACT(EPOCH FROM reserved_at)::double precision AS reserved_at_epoch, "
        "EXTRACT(EPOCH FROM cancelled_at)::double precision AS cancelled_at_epoch "
        "FROM reservations "
        "WHERE user_id = $1 "
        "ORDER BY reserved_at DESC, reservation_id DESC",
        userId
    );

    std::vector<Reservation> reservations;

    for (const auto& row : result) {
        const auto reservedAt =
            std::chrono::system_clock::time_point{
                std::chrono::duration_cast<std::chrono::system_clock::duration>(
                    std::chrono::duration<double>(
                        row["reserved_at_epoch"].as<double>()
                    )
                )
            };

        std::optional<std::chrono::system_clock::time_point> cancelledAt;

        if (!row["cancelled_at_epoch"].is_null()) {
            cancelledAt = std::chrono::system_clock::time_point{
                std::chrono::duration_cast<std::chrono::system_clock::duration>(
                    std::chrono::duration<double>(
                        row["cancelled_at_epoch"].as<double>()
                    )
                )
            };
        }

        const std::string status = row["status"].as<std::string>();

        const ReservationStatus reservationStatus =
            status == "CONFIRMED"
                ? ReservationStatus::Confirmed
                : ReservationStatus::Cancelled;

        reservations.emplace_back(
            row["reservation_id"].as<std::int64_t>(),
            row["user_id"].as<std::int64_t>(),
            row["seat_id"].as<std::int64_t>(),
            reservationStatus,
            reservedAt,
            cancelledAt
        );
    }

    return reservations;
}

std::optional<Reservation> ReservationRepository::findActiveBySeatId(
    std::int64_t seatId
) {
    pqxx::read_transaction transaction(database_.getConnection());

    auto result = transaction.exec_params(
        "SELECT reservation_id, user_id, seat_id, status, "
        "EXTRACT(EPOCH FROM reserved_at)::double precision AS reserved_at_epoch, "
        "EXTRACT(EPOCH FROM cancelled_at)::double precision AS cancelled_at_epoch "
        "FROM reservations "
        "WHERE seat_id = $1 AND status = 'CONFIRMED'",
        seatId
    );

    if (result.empty()) {
        return std::nullopt;
    }

    const auto& row = result[0];

    const auto reservedAt =
        std::chrono::system_clock::time_point{
            std::chrono::duration_cast<std::chrono::system_clock::duration>(
                std::chrono::duration<double>(
                    row["reserved_at_epoch"].as<double>()
                )
            )
        };

    std::optional<std::chrono::system_clock::time_point> cancelledAt;

    if (!row["cancelled_at_epoch"].is_null()) {
        cancelledAt = std::chrono::system_clock::time_point{
            std::chrono::duration_cast<std::chrono::system_clock::duration>(
                std::chrono::duration<double>(
                    row["cancelled_at_epoch"].as<double>()
                )
            )
        };
    }

    return Reservation(
        row["reservation_id"].as<std::int64_t>(),
        row["user_id"].as<std::int64_t>(),
        row["seat_id"].as<std::int64_t>(),
        ReservationStatus::Confirmed,
        reservedAt,
        cancelledAt
    );
}

void ReservationRepository::cancel(
    std::int64_t reservationId,
    std::chrono::system_clock::time_point cancellationTime
) {
    pqxx::work transaction(database_.getConnection());

    const auto cancellationTimeEpoch =
        std::chrono::duration<double>(
            cancellationTime.time_since_epoch()
        ).count();

    auto result = transaction.exec_params(
        "UPDATE reservations "
        "SET status = 'CANCELLED', "
        "cancelled_at = TO_TIMESTAMP($2) "
        "WHERE reservation_id = $1 "
        "AND status = 'CONFIRMED' "
        "RETURNING reservation_id",
        reservationId,
        cancellationTimeEpoch
    );

    if (result.empty()) {
        throw std::runtime_error(
            "Reservation not found or already cancelled."
        );
    }

    transaction.commit();
}
