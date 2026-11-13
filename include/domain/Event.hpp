#ifndef EVENT_HPP
#define EVENT_HPP

#include <chrono>
#include <cstdint>
#include <string>

class Event {
public:
    Event(
        std::int64_t eventId,
        const std::string& title,
        const std::string& description,
        const std::string& location,
        std::chrono::system_clock::time_point eventDate
    );

    std::int64_t getEventId() const;
    const std::string& getTitle() const;
    const std::string& getDescription() const;
    const std::string& getLocation() const;

    std::chrono::system_clock::time_point getEventDate() const;

private:
    std::int64_t eventId_;
    std::string title_;
    std::string description_;
    std::string location_;
    std::chrono::system_clock::time_point eventDate_;
};

#endif
