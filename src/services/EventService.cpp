#include "services/EventService.hpp"

EventService::EventService(IEventRepository& eventRepository)
    : eventRepository_(eventRepository) {
}

Event EventService::createEvent(
    const std::string& title,
    const std::string& description,
    const std::string& location,
    std::chrono::system_clock::time_point eventDate
) {
    return eventRepository_.create(
        title,
        description,
        location,
        eventDate
    );
}

std::optional<Event> EventService::getEventById(
    std::int64_t eventId
) {
    return eventRepository_.findById(eventId);
}

std::vector<Event> EventService::getAllEvents() {
    return eventRepository_.findAll();
}