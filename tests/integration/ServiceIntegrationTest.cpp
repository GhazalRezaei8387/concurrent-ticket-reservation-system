#include <gtest/gtest.h>

#include "database/DatabaseConnection.hpp"

#include "repositories/UserRepository.hpp"
#include "repositories/EventRepository.hpp"
#include "repositories/SeatRepository.hpp"
#include "repositories/ReservationRepository.hpp"

#include "services/UserService.hpp"
#include "services/EventService.hpp"
#include "services/SeatService.hpp"
#include "services/ReservationService.hpp"

#include <chrono>
#include <cstdlib>
#include <cstdint>
#include <memory>
#include <string>

class ServiceIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        const char* databaseUrl = std::getenv("DATABASE_URL");

        if (databaseUrl == nullptr) {
            GTEST_SKIP()
                << "DATABASE_URL environment variable is not set.";
        }

        database = std::make_unique<DatabaseConnection>(databaseUrl);
        database->connect();

        userRepository =
            std::make_unique<UserRepository>(*database);

        eventRepository =
            std::make_unique<EventRepository>(*database);

        seatRepository =
            std::make_unique<SeatRepository>(*database);

        reservationRepository =
            std::make_unique<ReservationRepository>(*database);

        userService =
            std::make_unique<UserService>(*userRepository);

        eventService =
            std::make_unique<EventService>(*eventRepository);

        seatService =
            std::make_unique<SeatService>(*seatRepository);

        reservationService =
            std::make_unique<ReservationService>(*reservationRepository);
    }

    void TearDown() override {
        reservationService.reset();
        seatService.reset();
        eventService.reset();
        userService.reset();

        reservationRepository.reset();
        seatRepository.reset();
        eventRepository.reset();
        userRepository.reset();

        if (database && database->isConnected()) {
            database->disconnect();
        }
    }

    std::unique_ptr<DatabaseConnection> database;

    std::unique_ptr<UserRepository> userRepository;
    std::unique_ptr<EventRepository> eventRepository;
    std::unique_ptr<SeatRepository> seatRepository;
    std::unique_ptr<ReservationRepository> reservationRepository;

    std::unique_ptr<UserService> userService;
    std::unique_ptr<EventService> eventService;
    std::unique_ptr<SeatService> seatService;
    std::unique_ptr<ReservationService> reservationService;
};

TEST_F(
    ServiceIntegrationTest,
    CompletesFullReservationWorkflow
) {
    const auto timestamp =
        std::chrono::system_clock::now()
            .time_since_epoch()
            .count();

    // 1. Register a user.
    const std::string email =
        "service_integration_" +
        std::to_string(timestamp) +
        "@example.com";

    User user = userService->registerUser(
        "Integration Test User",
        email
    );

    ASSERT_GT(user.getUserId(), 0);

    // 2. Create an event.
    Event event = eventService->createEvent(
        "Integration Test Event",
        "Event created during service integration testing",
        "Test Location",
        std::chrono::system_clock::now() +
            std::chrono::hours(24)
    );

    ASSERT_GT(event.getEventId(), 0);

    // 3. Create a seat for the event.
    Seat seat = seatService->createSeat(
        event.getEventId(),
        "A1",
        "Regular"
    );

    ASSERT_GT(seat.getSeatId(), 0);

    // 4. Reserve the seat.
    Reservation reservation =
        reservationService->createReservation(
            user.getUserId(),
            seat.getSeatId()
        );

    ASSERT_GT(reservation.getReservationId(), 0);

    EXPECT_EQ(
        reservation.getStatus(),
        ReservationStatus::Confirmed
    );

    // 5. Verify the active reservation.
    const auto activeReservation =
        reservationService->getActiveReservationBySeatId(
            seat.getSeatId()
        );

    ASSERT_TRUE(activeReservation.has_value());

    EXPECT_EQ(
        activeReservation->getReservationId(),
        reservation.getReservationId()
    );

    // 6. Cancel the reservation.
    reservationService->cancelReservation(
        reservation.getReservationId()
    );

    // 7. Verify that the reservation is cancelled.
    const auto cancelledReservation =
        reservationService->getReservationById(
            reservation.getReservationId()
        );

    ASSERT_TRUE(cancelledReservation.has_value());

    EXPECT_EQ(
        cancelledReservation->getStatus(),
        ReservationStatus::Cancelled
    );

    EXPECT_TRUE(
        cancelledReservation->getCancelledAt().has_value()
    );

    // 8. Verify that the reservation remains in user history.
    const auto history =
        reservationService->getReservationsByUserId(
            user.getUserId()
        );

    ASSERT_EQ(history.size(), 1);

    EXPECT_EQ(
        history[0].getReservationId(),
        reservation.getReservationId()
    );

    EXPECT_EQ(
        history[0].getStatus(),
        ReservationStatus::Cancelled
    );

    // 9. Verify that the seat is available again.
    const auto activeReservationAfterCancellation =
        reservationService->getActiveReservationBySeatId(
            seat.getSeatId()
        );

    EXPECT_FALSE(
        activeReservationAfterCancellation.has_value()
    );
}