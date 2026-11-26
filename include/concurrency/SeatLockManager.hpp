#ifndef SEAT_LOCK_MANAGER_HPP
#define SEAT_LOCK_MANAGER_HPP

#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>

class SeatLockManager {
public:
    std::shared_ptr<std::mutex> getLockForSeat(
        std::int64_t seatId
    );

private:
    std::mutex mapMutex_;

    std::unordered_map<
        std::int64_t,
        std::shared_ptr<std::mutex>
    > seatLocks_;
};

#endif
