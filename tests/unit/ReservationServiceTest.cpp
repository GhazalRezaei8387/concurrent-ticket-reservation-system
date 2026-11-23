#include <gtest/gtest.h>

#include "services/ReservationService.hpp"

#include <chrono>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <vector>

class FakeReservationRepository : public IReservationRepository {
public:
    Reservation create(
        std::int64_t userId,
        std::int64_t seatId
    ) override {
        const auto now = std::chrono::system_clock::now();

        Reservation reservation(
            nextId++,
            userId,
            seatId,
            ReservationStatus::Confirmed,
            now,
            std::nullopt
        );

        reservations.push_back(reservation);
        return reservation;
    }

    std::optional<Reservation> findById(
        std::int64_t reservationId
    ) override {
        for (const auto& reservation : reservations) {
            if (reservation.getReservationId() == reservationId) {
                return reservation;
            }
        }

        return std::nullopt;
    }

    std::vector<Reservation> findByUserId(
        std::int64_t userId
    ) override {
        std::vector<Reservation> result;

        for (const auto& reservation : reservations) {
            if (reservation.getUserId() == userId) {
                result.push_back(reservation);
            }
        }

        return result;
    }

    std::optional<Reservation> findActiveBySeatId(
        std::int64_t seatId
    ) override {
        for (const auto& reservation : reservations) {
            if (
                reservation.getSeatId() == seatId &&
                reservation.getStatus() == ReservationStatus::Confirmed
            ) {
                return reservation;
            }
        }

        return std::nullopt;
    }

    void cancel(
        std::int64_t reservationId,
        std::chrono::system_clock::time_point cancellationTime
    ) override {
        for (auto& reservation : reservations) {
            if (reservation.getReservationId() == reservationId) {
                reservation.cancel(cancellationTime);
                return;
            }
        }

        throw std::runtime_error("Reservation not found.");
    }

private:
    std::vector<Reservation> reservations;
    std::int64_t nextId = 1;
};

TEST(ReservationServiceTest, CreatesNewReservation) {
    FakeReservationRepository repository;
    ReservationService service(repository);

    const Reservation reservation =
        service.createReservation(10, 20);

    EXPECT_GT(reservation.getReservationId(), 0);
    EXPECT_EQ(reservation.getUserId(), 10);
    EXPECT_EQ(reservation.getSeatId(), 20);
    EXPECT_EQ(
        reservation.getStatus(),
        ReservationStatus::Confirmed
    );
}

TEST(ReservationServiceTest, RejectsInvalidUserId) {
    FakeReservationRepository repository;
    ReservationService service(repository);

    EXPECT_THROW(
        service.createReservation(0, 20),
        std::invalid_argument
    );
}

TEST(ReservationServiceTest, RejectsInvalidSeatId) {
    FakeReservationRepository repository;
    ReservationService service(repository);

    EXPECT_THROW(
        service.createReservation(10, 0),
        std::invalid_argument
    );
}

TEST(ReservationServiceTest, RejectsDuplicateActiveReservation) {
    FakeReservationRepository repository;
    ReservationService service(repository);

    service.createReservation(10, 20);

    EXPECT_THROW(
        service.createReservation(11, 20),
        std::runtime_error
    );
}

TEST(ReservationServiceTest, FindsReservationById) {
    FakeReservationRepository repository;
    ReservationService service(repository);

    const auto created =
        service.createReservation(10, 20);

    const auto found =
        service.getReservationById(created.getReservationId());

    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getUserId(), 10);
    EXPECT_EQ(found->getSeatId(), 20);
}

TEST(ReservationServiceTest, ReturnsReservationsByUserId) {
    FakeReservationRepository repository;
    ReservationService service(repository);

    service.createReservation(10, 20);
    service.createReservation(10, 21);
    service.createReservation(11, 22);

    const auto reservations =
        service.getReservationsByUserId(10);

    ASSERT_EQ(reservations.size(), 2);
    EXPECT_EQ(reservations[0].getSeatId(), 20);
    EXPECT_EQ(reservations[1].getSeatId(), 21);
}

TEST(ReservationServiceTest, FindsActiveReservationBySeatId) {
    FakeReservationRepository repository;
    ReservationService service(repository);

    const auto created =
        service.createReservation(10, 20);

    const auto found =
        service.getActiveReservationBySeatId(20);

    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(
        found->getReservationId(),
        created.getReservationId()
    );
}

TEST(ReservationServiceTest, ReturnsEmptyForUnreservedSeat) {
    FakeReservationRepository repository;
    ReservationService service(repository);

    const auto found =
        service.getActiveReservationBySeatId(20);

    EXPECT_FALSE(found.has_value());
}

TEST(ReservationServiceTest, CancelsReservation) {
    FakeReservationRepository repository;
    ReservationService service(repository);

    const auto created =
        service.createReservation(10, 20);

    service.cancelReservation(created.getReservationId());

    const auto cancelled =
        service.getReservationById(created.getReservationId());

    ASSERT_TRUE(cancelled.has_value());
    EXPECT_EQ(
        cancelled->getStatus(),
        ReservationStatus::Cancelled
    );
    EXPECT_TRUE(cancelled->getCancelledAt().has_value());
}

TEST(ReservationServiceTest, RejectsCancellingNonexistentReservation) {
    FakeReservationRepository repository;
    ReservationService service(repository);

    EXPECT_THROW(
        service.cancelReservation(999),
        std::runtime_error
    );
}

TEST(ReservationServiceTest, RejectsCancellingAlreadyCancelledReservation) {
    FakeReservationRepository repository;
    ReservationService service(repository);

    const auto created =
        service.createReservation(10, 20);

    service.cancelReservation(created.getReservationId());

    EXPECT_THROW(
        service.cancelReservation(created.getReservationId()),
        std::logic_error
    );
}
