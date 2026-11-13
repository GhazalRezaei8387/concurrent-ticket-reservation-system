#include <gtest/gtest.h>

#include "domain/Reservation.hpp"

#include <chrono>
#include <cstdint>
#include <optional>
#include <stdexcept>

using namespace std::chrono;

TEST(ReservationTest, CreatesConfirmedReservation) {
    const auto reservedAt = system_clock::now();

    Reservation reservation(
        1,
        10,
        20,
        ReservationStatus::Confirmed,
        reservedAt,
        std::nullopt
    );

    EXPECT_EQ(reservation.getReservationId(), 1);
    EXPECT_EQ(reservation.getUserId(), 10);
    EXPECT_EQ(reservation.getSeatId(), 20);
    EXPECT_EQ(reservation.getStatus(), ReservationStatus::Confirmed);
    EXPECT_EQ(reservation.getReservedAt(), reservedAt);
    EXPECT_FALSE(reservation.getCancelledAt().has_value());
}

TEST(ReservationTest, CreatesCancelledReservation) {
    const auto reservedAt = system_clock::now();
    const auto cancelledAt = reservedAt + minutes(10);

    Reservation reservation(
        1,
        10,
        20,
        ReservationStatus::Cancelled,
        reservedAt,
        cancelledAt
    );

    EXPECT_EQ(reservation.getStatus(), ReservationStatus::Cancelled);
    EXPECT_EQ(reservation.getCancelledAt(), cancelledAt);
}

TEST(ReservationTest, RejectsInvalidReservationId) {
    EXPECT_THROW(
        Reservation(
            0, 10, 20,
            ReservationStatus::Confirmed,
            system_clock::now(),
            std::nullopt
        ),
        std::invalid_argument
    );
}

TEST(ReservationTest, RejectsInvalidUserId) {
    EXPECT_THROW(
        Reservation(
            1, 0, 20,
            ReservationStatus::Confirmed,
            system_clock::now(),
            std::nullopt
        ),
        std::invalid_argument
    );
}

TEST(ReservationTest, RejectsInvalidSeatId) {
    EXPECT_THROW(
        Reservation(
            1, 10, 0,
            ReservationStatus::Confirmed,
            system_clock::now(),
            std::nullopt
        ),
        std::invalid_argument
    );
}

TEST(ReservationTest, RejectsConfirmedReservationWithCancellationTime) {
    EXPECT_THROW(
        Reservation(
            1, 10, 20,
            ReservationStatus::Confirmed,
            system_clock::now(),
            system_clock::now()
        ),
        std::invalid_argument
    );
}

TEST(ReservationTest, RejectsCancelledReservationWithoutCancellationTime) {
    EXPECT_THROW(
        Reservation(
            1, 10, 20,
            ReservationStatus::Cancelled,
            system_clock::now(),
            std::nullopt
        ),
        std::invalid_argument
    );
}

TEST(ReservationTest, CancelsConfirmedReservation) {
    const auto reservedAt = system_clock::now();
    const auto cancelledAt = reservedAt + minutes(5);

    Reservation reservation(
        1, 10, 20,
        ReservationStatus::Confirmed,
        reservedAt,
        std::nullopt
    );

    reservation.cancel(cancelledAt);

    EXPECT_EQ(reservation.getStatus(), ReservationStatus::Cancelled);
    EXPECT_EQ(reservation.getCancelledAt(), cancelledAt);
}

TEST(ReservationTest, RejectsCancellingAlreadyCancelledReservation) {
    const auto reservedAt = system_clock::now();
    const auto cancelledAt = reservedAt + minutes(5);

    Reservation reservation(
        1, 10, 20,
        ReservationStatus::Cancelled,
        reservedAt,
        cancelledAt
    );

    EXPECT_THROW(
        reservation.cancel(cancelledAt + minutes(5)),
        std::logic_error
    );
}

TEST(ReservationTest, RejectsCancellationBeforeReservationTime) {
    const auto reservedAt = system_clock::now();

    Reservation reservation(
        1, 10, 20,
        ReservationStatus::Confirmed,
        reservedAt,
        std::nullopt
    );

    EXPECT_THROW(
        reservation.cancel(reservedAt - minutes(1)),
        std::invalid_argument
    );
}
