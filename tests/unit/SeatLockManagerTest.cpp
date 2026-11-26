#include <gtest/gtest.h>

#include "concurrency/SeatLockManager.hpp"

#include <stdexcept>

#include <atomic>
#include <thread>
#include <vector>

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

TEST(SeatLockManagerTest, PreventsConcurrentAccessToSameSeat) {
    SeatLockManager manager;

    const int threadCount = 5;

    auto seatLock = manager.getLockForSeat(101);

    int activeThreads = 0;
    std::atomic<bool> concurrentAccessDetected{false};

    std::vector<std::thread> workers;

    for (int i = 0; i < threadCount; ++i) {
        workers.emplace_back([&]() {
            std::lock_guard<std::mutex> lock(*seatLock);

            activeThreads++;

            if (activeThreads > 1) {
                concurrentAccessDetected = true;
            }

            activeThreads--;
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }

    EXPECT_FALSE(concurrentAccessDetected.load());
}

TEST(SeatLockManagerTest, UniqueLockCanUnlockAndLockAgain) {
    std::mutex mutex;

    std::unique_lock<std::mutex> lock(mutex);

    EXPECT_TRUE(lock.owns_lock());

    lock.unlock();

    EXPECT_FALSE(lock.owns_lock());

    lock.lock();

    EXPECT_TRUE(lock.owns_lock());
}

TEST(SeatLockManagerTest, AllowsConcurrentAccessToDifferentSeats) {
    SeatLockManager manager;

    auto firstSeatLock = manager.getLockForSeat(101);
    auto secondSeatLock = manager.getLockForSeat(102);

    std::atomic<int> completedCount{0};

    std::thread firstThread([&]() {
        std::lock_guard<std::mutex> lock(*firstSeatLock);
        completedCount++;
    });

    std::thread secondThread([&]() {
        std::lock_guard<std::mutex> lock(*secondSeatLock);
        completedCount++;
    });

    firstThread.join();
    secondThread.join();

    EXPECT_EQ(completedCount.load(), 2);
}
