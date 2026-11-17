#include <gtest/gtest.h>

#include "database/DatabaseConnection.hpp"
#include "repositories/EventRepository.hpp"
#include "repositories/SeatRepository.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>

std::string makeUniqueSeatTestEventTitle(const std::string& prefix) {
    const auto timestamp =
        std::chrono::system_clock::now()
            .time_since_epoch()
            .count();

    return prefix + "_" + std::to_string(timestamp);
}

class SeatRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        const char* databaseUrl = std::getenv("DATABASE_URL");

        if (databaseUrl == nullptr) {
            GTEST_SKIP()
                << "DATABASE_URL environment variable is not set.";
        }

        database = std::make_unique<DatabaseConnection>(databaseUrl);
        database->connect();

        eventRepository = std::make_unique<EventRepository>(*database);
        seatRepository = std::make_unique<SeatRepository>(*database);
    }

    void TearDown() override {
        seatRepository.reset();
        eventRepository.reset();

        if (database && database->isConnected()) {
            database->disconnect();
        }
    }

    std::int64_t createTestEvent() {
        Event event = eventRepository->create(
            makeUniqueSeatTestEventTitle("seat_repository_test"),
            "Test event",
            "Test location",
            std::chrono::system_clock::now() + std::chrono::hours(24)
        );

        return event.getEventId();
    }

    std::unique_ptr<DatabaseConnection> database;
    std::unique_ptr<EventRepository> eventRepository;
    std::unique_ptr<SeatRepository> seatRepository;
};

TEST_F(SeatRepositoryTest, CreateSeat) {
    const std::int64_t eventId = createTestEvent();

    Seat seat = seatRepository->create(
        eventId,
        "A1",
        "VIP"
    );

    EXPECT_GT(seat.getSeatId(), 0);
    EXPECT_EQ(seat.getEventId(), eventId);
    EXPECT_EQ(seat.getSeatNumber(), "A1");
    EXPECT_EQ(seat.getSeatType(), "VIP");
}

TEST_F(SeatRepositoryTest, FindExistingSeatById) {
    const std::int64_t eventId = createTestEvent();

    Seat createdSeat = seatRepository->create(
        eventId,
        "B1",
        "Regular"
    );

    auto foundSeat =
        seatRepository->findById(createdSeat.getSeatId());

    ASSERT_TRUE(foundSeat.has_value());

    EXPECT_EQ(foundSeat->getSeatId(), createdSeat.getSeatId());
    EXPECT_EQ(foundSeat->getEventId(), eventId);
    EXPECT_EQ(foundSeat->getSeatNumber(), "B1");
    EXPECT_EQ(foundSeat->getSeatType(), "Regular");
}

TEST_F(SeatRepositoryTest, ReturnEmptyWhenSeatDoesNotExist) {
    auto foundSeat = seatRepository->findById(-1);

    EXPECT_FALSE(foundSeat.has_value());
}

TEST_F(SeatRepositoryTest, FindAllSeatsByEventId) {
    const std::int64_t eventId = createTestEvent();

    seatRepository->create(eventId, "A3", "Regular");
    seatRepository->create(eventId, "A1", "VIP");
    seatRepository->create(eventId, "A2", "Regular");

    const auto seats = seatRepository->findByEventId(eventId);

    ASSERT_EQ(seats.size(), 3);

    EXPECT_EQ(seats[0].getSeatNumber(), "A1");
    EXPECT_EQ(seats[1].getSeatNumber(), "A2");
    EXPECT_EQ(seats[2].getSeatNumber(), "A3");

    for (const auto& seat : seats) {
        EXPECT_EQ(seat.getEventId(), eventId);
    }
}