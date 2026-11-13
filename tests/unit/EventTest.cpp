#include <gtest/gtest.h>

#include "domain/Event.hpp"

#include <chrono>
#include <cstdint>
#include <stdexcept>
#include <string>

using namespace std::chrono;

TEST(EventTest, CreatesEventWithValidData) {
    const auto eventDate = system_clock::now();

    Event event(
        1,
        "Rock Concert",
        "Live music event",
        "Berlin Arena",
        eventDate
    );

    EXPECT_EQ(event.getEventId(), 1);
    EXPECT_EQ(event.getTitle(), "Rock Concert");
    EXPECT_EQ(event.getDescription(), "Live music event");
    EXPECT_EQ(event.getLocation(), "Berlin Arena");
    EXPECT_EQ(event.getEventDate(), eventDate);
}

TEST(EventTest, AcceptsEmptyDescription) {
    Event event(
        1,
        "Rock Concert",
        "",
        "Berlin Arena",
        system_clock::now()
    );

    EXPECT_TRUE(event.getDescription().empty());
}

TEST(EventTest, RejectsZeroEventId) {
    EXPECT_THROW(
        Event(
            0,
            "Rock Concert",
            "",
            "Berlin Arena",
            system_clock::now()
        ),
        std::invalid_argument
    );
}

TEST(EventTest, RejectsNegativeEventId) {
    EXPECT_THROW(
        Event(
            -1,
            "Rock Concert",
            "",
            "Berlin Arena",
            system_clock::now()
        ),
        std::invalid_argument
    );
}

TEST(EventTest, RejectsEmptyTitle) {
    EXPECT_THROW(
        Event(
            1,
            "",
            "",
            "Berlin Arena",
            system_clock::now()
        ),
        std::invalid_argument
    );
}

TEST(EventTest, RejectsWhitespaceOnlyTitle) {
    EXPECT_THROW(
        Event(
            1,
            "   ",
            "",
            "Berlin Arena",
            system_clock::now()
        ),
        std::invalid_argument
    );
}

TEST(EventTest, RejectsEmptyLocation) {
    EXPECT_THROW(
        Event(
            1,
            "Rock Concert",
            "",
            "",
            system_clock::now()
        ),
        std::invalid_argument
    );
}

TEST(EventTest, RejectsWhitespaceOnlyLocation) {
    EXPECT_THROW(
        Event(
            1,
            "Rock Concert",
            "",
            "   ",
            system_clock::now()
        ),
        std::invalid_argument
    );
}
