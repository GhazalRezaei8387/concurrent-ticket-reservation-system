#ifndef IEVENT_REPOSITORY_HPP
#define IEVENT_REPOSITORY_HPP

#include "domain/Event.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class IEventRepository {
public:
    virtual ~IEventRepository() = default;

    virtual Event create(
        const std::string& title,
        const std::string& description,
        const std::string& location,
        std::chrono::system_clock::time_point eventDate
    ) = 0;

    virtual std::optional<Event> findById(
        std::int64_t eventId
    ) = 0;

    virtual std::vector<Event> findAll() = 0;
};

#endif