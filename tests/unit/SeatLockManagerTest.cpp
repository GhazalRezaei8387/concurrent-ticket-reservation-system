#include <gtest/gtest.h>

#include "concurrency/SeatLockManager.hpp"

#include <stdexcept>

TEST(SeatLockManagerTest, ReturnsLockForValidSeat) {
    SeatLockManager manager;

    const auto seatLock = manager.getLockForSeat(101);

    EXPECT_NE(seatLock, nullptr);
}

TEST(SeatLockManagerTest, ReturnsSameLockForSameSeat) {
    SeatLockManager manager;

    const auto firstLock = manager.getLockForSeat(101);
    const auto secondLock = manager.getLockForSeat(101);

    EXPECT_EQ(firstLock, secondLock);
}

TEST(SeatLockManagerTest, ReturnsDifferentLocksForDifferentSeats) {
    SeatLockManager manager;

    const auto firstLock = manager.getLockForSeat(101);
    const auto secondLock = manager.getLockForSeat(102);

    EXPECT_NE(firstLock, secondLock);
}

TEST(SeatLockManagerTest, RejectsInvalidSeatId) {
    SeatLockManager manager;

    EXPECT_THROW(
        manager.getLockForSeat(0),
        std::invalid_argument
    );

    EXPECT_THROW(
        manager.getLockForSeat(-1),
        std::invalid_argument
    );
}
