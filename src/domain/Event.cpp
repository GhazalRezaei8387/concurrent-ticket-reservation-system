#include "domain/Event.hpp"

#include <stdexcept>

Event::Event(
    std::int64_t eventId,
    const std::string& title,
    const std::string& description,
    const std::string& location,
    std::chrono::system_clock::time_point eventDate
)
    : eventId_(eventId),
      title_(title),
      description_(description),
      location_(location),
      eventDate_(eventDate) {

    if (eventId_ <= 0) {
        throw std::invalid_argument(
            "Event ID must be positive."
        );
    }

    if (title_.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument(
            "Event title cannot be empty."
        );
    }

    if (location_.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument(
            "Event location cannot be empty."
        );
    }
}

std::int64_t Event::getEventId() const {
    return eventId_;
}

const std::string& Event::getTitle() const {
    return title_;
}

const std::string& Event::getDescription() const {
    return description_;
}

const std::string& Event::getLocation() const {
    return location_;
}

std::chrono::system_clock::time_point Event::getEventDate() const {
    return eventDate_;
}
