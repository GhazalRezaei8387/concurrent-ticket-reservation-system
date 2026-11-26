#include "concurrency/SeatLockManager.hpp"

#include <stdexcept>

std::shared_ptr<std::mutex> SeatLockManager::getLockForSeat(
    std::int64_t seatId
) {
    if (seatId <= 0) {
        throw std::invalid_argument(
            "Seat ID must be positive."
        );
    }

    std::lock_guard<std::mutex> guard(mapMutex_);

    auto it = seatLocks_.find(seatId);

    if (it != seatLocks_.end()) {
        return it->second;
    }

    auto seatMutex = std::make_shared<std::mutex>();

    seatLocks_[seatId] = seatMutex;

    return seatMutex;
}
