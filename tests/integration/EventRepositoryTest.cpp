
#include <gtest/gtest.h>

#include "database/DatabaseConnection.hpp"
#include "repositories/EventRepository.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>

std::string makeUniqueEventTitle(const std::string& prefix) {
    const auto timestamp =
        std::chrono::system_clock::now()
            .time_since_epoch()
            .count();

    return prefix + "_" + std::to_string(timestamp);
}

class EventRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        const char* databaseUrl = std::getenv("DATABASE_URL");

        if (databaseUrl == nullptr) {
            GTEST_SKIP()
                << "DATABASE_URL environment variable is not set.";
        }

        database = std::make_unique<DatabaseConnection>(databaseUrl);
        database->connect();

        repository = std::make_unique<EventRepository>(*database);
    }

    void TearDown() override {
        repository.reset();

        if (database && database->isConnected()) {
            database->disconnect();
        }
    }

    std::unique_ptr<DatabaseConnection> database;
    std::unique_ptr<EventRepository> repository;
};

TEST_F(EventRepositoryTest, CreateEvent) {
    const std::string title =
        makeUniqueEventTitle("repository_create_test");

    const auto eventDate =
        std::chrono::system_clock::now() + std::chrono::hours(24);

    Event event = repository->create(
        title,
        "Integration test event",
        "Test Location",
        eventDate
    );

    EXPECT_GT(event.getEventId(), 0);
    EXPECT_EQ(event.getTitle(), title);
    EXPECT_EQ(event.getDescription(), "Integration test event");
    EXPECT_EQ(event.getLocation(), "Test Location");
}

TEST_F(EventRepositoryTest, FindExistingEventById) {
    const std::string title =
        makeUniqueEventTitle("repository_find_id_test");

    const auto eventDate =
        std::chrono::system_clock::now() + std::chrono::hours(24);

    Event createdEvent = repository->create(
        title,
        "Find by ID test",
        "Test Location",
        eventDate
    );

    auto foundEvent =
        repository->findById(createdEvent.getEventId());

    ASSERT_TRUE(foundEvent.has_value());

    EXPECT_EQ(foundEvent->getEventId(), createdEvent.getEventId());
    EXPECT_EQ(foundEvent->getTitle(), title);
    EXPECT_EQ(foundEvent->getDescription(), "Find by ID test");
    EXPECT_EQ(foundEvent->getLocation(), "Test Location");
}

TEST_F(EventRepositoryTest, ReturnEmptyWhenEventDoesNotExist) {
    auto foundEvent = repository->findById(-1);

    EXPECT_FALSE(foundEvent.has_value());
}

TEST_F(EventRepositoryTest, FindAllEventsSortedByDate) {
    const auto baseDate =
        std::chrono::system_clock::now();

    Event thirdEvent = repository->create(
        makeUniqueEventTitle("repository_third_event"),
        "Third event",
        "Location C",
        baseDate + std::chrono::hours(72)
    );

    Event firstEvent = repository->create(
        makeUniqueEventTitle("repository_first_event"),
        "First event",
        "Location A",
        baseDate + std::chrono::hours(24)
    );

    Event secondEvent = repository->create(
        makeUniqueEventTitle("repository_second_event"),
        "Second event",
        "Location B",
        baseDate + std::chrono::hours(48)
    );

    const auto events = repository->findAll();

    const auto firstPosition = std::find_if(
        events.begin(),
        events.end(),
        [&](const Event& event) {
            return event.getEventId() == firstEvent.getEventId();
        }
    );

    const auto secondPosition = std::find_if(
        events.begin(),
        events.end(),
        [&](const Event& event) {
            return event.getEventId() == secondEvent.getEventId();
        }
    );

    const auto thirdPosition = std::find_if(
        events.begin(),
        events.end(),
        [&](const Event& event) {
            return event.getEventId() == thirdEvent.getEventId();
        }
    );

    ASSERT_NE(firstPosition, events.end());
    ASSERT_NE(secondPosition, events.end());
    ASSERT_NE(thirdPosition, events.end());

    EXPECT_LT(firstPosition, secondPosition);
    EXPECT_LT(secondPosition, thirdPosition);
}
