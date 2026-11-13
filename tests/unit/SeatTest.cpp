#include <gtest/gtest.h>

#include "domain/Seat.hpp"

#include <cstdint>
#include <stdexcept>
#include <string>

TEST(SeatTest, CreatesSeatWithValidData) {
    Seat seat(
        1,
        10,
        "A12",
        "VIP"
    );

    EXPECT_EQ(seat.getSeatId(), 1);
    EXPECT_EQ(seat.getEventId(), 10);
    EXPECT_EQ(seat.getSeatNumber(), "A12");
    EXPECT_EQ(seat.getSeatType(), "VIP");
}

TEST(SeatTest, RejectsZeroSeatId) {
    EXPECT_THROW(
        Seat(0, 10, "A12", "VIP"),
        std::invalid_argument
    );
}

TEST(SeatTest, RejectsNegativeSeatId) {
    EXPECT_THROW(
        Seat(-1, 10, "A12", "VIP"),
        std::invalid_argument
    );
}

TEST(SeatTest, RejectsZeroEventId) {
    EXPECT_THROW(
        Seat(1, 0, "A12", "VIP"),
        std::invalid_argument
    );
}

TEST(SeatTest, RejectsNegativeEventId) {
    EXPECT_THROW(
        Seat(1, -1, "A12", "VIP"),
        std::invalid_argument
    );
}

TEST(SeatTest, RejectsEmptySeatNumber) {
    EXPECT_THROW(
        Seat(1, 10, "", "VIP"),
        std::invalid_argument
    );
}

TEST(SeatTest, RejectsWhitespaceOnlySeatNumber) {
    EXPECT_THROW(
        Seat(1, 10, "   ", "VIP"),
        std::invalid_argument
    );
}

TEST(SeatTest, RejectsEmptySeatType) {
    EXPECT_THROW(
        Seat(1, 10, "A12", ""),
        std::invalid_argument
    );
}

TEST(SeatTest, RejectsWhitespaceOnlySeatType) {
    EXPECT_THROW(
        Seat(1, 10, "A12", "   "),
        std::invalid_argument
    );
}
