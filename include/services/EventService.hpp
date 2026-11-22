#ifndef EVENT_SERVICE_HPP
#define EVENT_SERVICE_HPP

#include "repositories/IEventRepository.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class EventService {
public:
    explicit EventService(IEventRepository& eventRepository);

    Event createEvent(
        const std::string& title,
        const std::string& description,
        const std::string& location,
        std::chrono::system_clock::time_point eventDate
    );

    std::optional<Event> getEventById(std::int64_t eventId);

    std::vector<Event> getAllEvents();

private:
    IEventRepository& eventRepository_;
};

#endif