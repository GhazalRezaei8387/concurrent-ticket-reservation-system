cmake_minimum_required(VERSION 3.20)

project(ConcurrentTicketReservationSystem
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include project headers
include_directories(include)

# Main application
add_executable(ticket_reservation
    src/main.cpp
)
