#include <gtest/gtest.h>

#include "services/EventService.hpp"

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class FakeEventRepository : public IEventRepository {
public:
    Event create(
        const std::string& title,
        const std::string& description,
        const std::string& location,
        std::chrono::system_clock::time_point eventDate
    ) override {
        Event event(
            nextId++,
            title,
            description,
            location,
            eventDate
        );

        events.push_back(event);
        return event;
    }

    std::optional<Event> findById(
        std::int64_t eventId
    ) override {
        for (const auto& event : events) {
            if (event.getEventId() == eventId) {
                return event;
            }
        }

        return std::nullopt;
    }

    std::vector<Event> findAll() override {
        return events;
    }

private:
    std::vector<Event> events;
    std::int64_t nextId = 1;
};

TEST(EventServiceTest, CreatesNewEvent) {
    FakeEventRepository repository;
    EventService service(repository);

    const auto eventDate =
        std::chrono::system_clock::now();

    Event event = service.createEvent(
        "Concert",
        "Live music event",
        "Tehran",
        eventDate
    );

    EXPECT_GT(event.getEventId(), 0);
    EXPECT_EQ(event.getTitle(), "Concert");
    EXPECT_EQ(event.getLocation(), "Tehran");
}

TEST(EventServiceTest, FindsEventById) {
    FakeEventRepository repository;
    EventService service(repository);

    Event createdEvent = service.createEvent(
        "Concert",
        "Live music event",
        "Tehran",
        std::chrono::system_clock::now()
    );

    const auto foundEvent =
        service.getEventById(createdEvent.getEventId());

    ASSERT_TRUE(foundEvent.has_value());
    EXPECT_EQ(foundEvent->getTitle(), "Concert");
}

TEST(EventServiceTest, ReturnsAllEvents) {
    FakeEventRepository repository;
    EventService service(repository);

    service.createEvent(
        "Concert",
        "Music event",
        "Tehran",
        std::chrono::system_clock::now()
    );

    service.createEvent(
        "Conference",
        "Tech event",
        "Berlin",
        std::chrono::system_clock::now()
    );

    const auto events = service.getAllEvents();

    ASSERT_EQ(events.size(), 2);
    EXPECT_EQ(events[0].getTitle(), "Concert");
    EXPECT_EQ(events[1].getTitle(), "Conference");
}