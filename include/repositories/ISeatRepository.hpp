#ifndef ISEAT_REPOSITORY_HPP
#define ISEAT_REPOSITORY_HPP

#include "domain/Seat.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class ISeatRepository {
public:
    virtual ~ISeatRepository() = default;

    virtual Seat create(
        std::int64_t eventId,
        const std::string& seatNumber,
        const std::string& seatType
    ) = 0;

    virtual std::optional<Seat> findById(
        std::int64_t seatId
    ) = 0;

    virtual std::vector<Seat> findByEventId(
        std::int64_t eventId
    ) = 0;
};

#endif