
#include <gtest/gtest.h>

#include "database/DatabaseConnection.hpp"
#include "repositories/UserRepository.hpp"
#include "repositories/EventRepository.hpp"
#include "repositories/SeatRepository.hpp"
#include "repositories/ReservationRepository.hpp"

#include <chrono>
#include <cstdlib>
#include <cstdint>
#include <memory>
#include <string>

std::string makeUniqueReservationTestEmail() {
    const auto timestamp =
        std::chrono::system_clock::now()
            .time_since_epoch()
            .count();

    return "reservation_test_" +
           std::to_string(timestamp) +
           "@example.com";
}

std::string makeUniqueReservationTestEventTitle() {
    const auto timestamp =
        std::chrono::system_clock::now()
            .time_since_epoch()
            .count();

    return "reservation_event_test_" +
           std::to_string(timestamp);
}

class ReservationRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        const char* databaseUrl = std::getenv("DATABASE_URL");

        if (databaseUrl == nullptr) {
            GTEST_SKIP()
                << "DATABASE_URL environment variable is not set.";
        }

        database = std::make_unique<DatabaseConnection>(databaseUrl);
        database->connect();

        userRepository = std::make_unique<UserRepository>(*database);
        eventRepository = std::make_unique<EventRepository>(*database);
        seatRepository = std::make_unique<SeatRepository>(*database);

        reservationRepository =
            std::make_unique<ReservationRepository>(*database);
    }

    void TearDown() override {
        reservationRepository.reset();
        seatRepository.reset();
        eventRepository.reset();
        userRepository.reset();

        if (database && database->isConnected()) {
            database->disconnect();
        }
    }

    std::int64_t createTestUser() {
        User user = userRepository->create(
            "Reservation Test User",
            makeUniqueReservationTestEmail()
        );

        return user.getUserId();
    }

    std::int64_t createTestSeat() {
        Event event = eventRepository->create(
            makeUniqueReservationTestEventTitle(),
            "Reservation test event",
            "Test location",
            std::chrono::system_clock::now() + std::chrono::hours(24)
        );

        Seat seat = seatRepository->create(
            event.getEventId(),
            "A1",
            "Regular"
        );

        return seat.getSeatId();
    }

    std::unique_ptr<DatabaseConnection> database;
    std::unique_ptr<UserRepository> userRepository;
    std::unique_ptr<EventRepository> eventRepository;
    std::unique_ptr<SeatRepository> seatRepository;
    std::unique_ptr<ReservationRepository> reservationRepository;
};

// Test 1: Create Reservation
TEST_F(ReservationRepositoryTest, CreateReservation) {
    const std::int64_t userId = createTestUser();
    const std::int64_t seatId = createTestSeat();

    Reservation reservation =
        reservationRepository->create(userId, seatId);

    EXPECT_GT(reservation.getReservationId(), 0);
    EXPECT_EQ(reservation.getUserId(), userId);
    EXPECT_EQ(reservation.getSeatId(), seatId);

    EXPECT_EQ(
        reservation.getStatus(),
        ReservationStatus::Confirmed
    );

    EXPECT_TRUE(
        reservation.getReservedAt() <=
        std::chrono::system_clock::now()
    );

    EXPECT_FALSE(reservation.getCancelledAt().has_value());
}

// Test 2: Find Reservation By ID
TEST_F(ReservationRepositoryTest, FindReservationById) {
    const std::int64_t userId = createTestUser();
    const std::int64_t seatId = createTestSeat();

    Reservation createdReservation =
        reservationRepository->create(userId, seatId);

    const auto foundReservation =
        reservationRepository->findById(
            createdReservation.getReservationId()
        );

    ASSERT_TRUE(foundReservation.has_value());

    EXPECT_EQ(
        foundReservation->getReservationId(),
        createdReservation.getReservationId()
    );

    EXPECT_EQ(foundReservation->getUserId(), userId);
    EXPECT_EQ(foundReservation->getSeatId(), seatId);

    EXPECT_EQ(
        foundReservation->getStatus(),
        ReservationStatus::Confirmed
    );

    EXPECT_FALSE(foundReservation->getCancelledAt().has_value());
}

// Test 3: Find Reservation By ID Not Found
TEST_F(ReservationRepositoryTest, FindReservationByIdNotFound) {
    const std::int64_t nonExistingReservationId = 999999999;

    const auto foundReservation =
        reservationRepository->findById(nonExistingReservationId);

    EXPECT_FALSE(foundReservation.has_value());
}

// Test 4: Find Reservations By User ID
TEST_F(ReservationRepositoryTest, FindReservationsByUserId) {
    const std::int64_t userId = createTestUser();
    const std::int64_t anotherUserId = createTestUser();

    const std::int64_t firstSeatId = createTestSeat();
    const std::int64_t secondSeatId = createTestSeat();
    const std::int64_t thirdSeatId = createTestSeat();

    Reservation firstReservation =
        reservationRepository->create(userId, firstSeatId);

    Reservation secondReservation =
        reservationRepository->create(userId, secondSeatId);

    reservationRepository->create(anotherUserId, thirdSeatId);

    const auto reservations =
        reservationRepository->findByUserId(userId);

    ASSERT_EQ(reservations.size(), 2);

    EXPECT_EQ(
        reservations[0].getReservationId(),
        secondReservation.getReservationId()
    );

    EXPECT_EQ(
        reservations[1].getReservationId(),
        firstReservation.getReservationId()
    );

    EXPECT_EQ(reservations[0].getUserId(), userId);
    EXPECT_EQ(reservations[1].getUserId(), userId);
}

// Test 5: Find Active Reservation By Seat ID
TEST_F(ReservationRepositoryTest, FindActiveReservationBySeatId) {
    const std::int64_t userId = createTestUser();
    const std::int64_t seatId = createTestSeat();

    Reservation createdReservation =
        reservationRepository->create(userId, seatId);

    const auto activeReservation =
        reservationRepository->findActiveBySeatId(seatId);

    ASSERT_TRUE(activeReservation.has_value());

    EXPECT_EQ(
        activeReservation->getReservationId(),
        createdReservation.getReservationId()
    );

    EXPECT_EQ(activeReservation->getUserId(), userId);
    EXPECT_EQ(activeReservation->getSeatId(), seatId);

    EXPECT_EQ(
        activeReservation->getStatus(),
        ReservationStatus::Confirmed
    );

    EXPECT_FALSE(activeReservation->getCancelledAt().has_value());
}