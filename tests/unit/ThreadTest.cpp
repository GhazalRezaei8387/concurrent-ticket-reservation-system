
#include <gtest/gtest.h>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

TEST(ThreadTest, RunsFunctionInSeparateThread) {
    bool executed = false;

    std::thread worker([&executed]() {
        executed = true;
    });

    worker.join();

    EXPECT_TRUE(executed);
}

TEST(ThreadTest, RunsMultipleThreadsConcurrently) {
    const int threadCount = 5;

    std::atomic<int> completedCount{0};

    std::vector<std::thread> workers;

    for (int i = 0; i < threadCount; ++i) {
        workers.emplace_back([&completedCount]() {
            completedCount++;
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }

    EXPECT_EQ(completedCount.load(), threadCount);
}

TEST(ThreadTest, StartsAllThreadsTogether) {
    const int threadCount = 5;

    int readyCount = 0;
    bool start = false;

    std::mutex mutex;
    std::condition_variable condition;

    std::atomic<int> completedCount{0};

    std::vector<std::thread> workers;

    for (int i = 0; i < threadCount; ++i) {
        workers.emplace_back([&]() {
            std::unique_lock<std::mutex> lock(mutex);

            readyCount++;
            condition.notify_all();

            condition.wait(lock, [&start]() {
                return start;
            });

            lock.unlock();

            completedCount++;
        });
    }

    {
        std::unique_lock<std::mutex> lock(mutex);

        condition.wait(lock, [&readyCount, threadCount]() {
            return readyCount == threadCount;
        });

        start = true;
    }

    condition.notify_all();

    for (auto& worker : workers) {
        worker.join();
    }

    EXPECT_EQ(completedCount.load(), threadCount);
}
