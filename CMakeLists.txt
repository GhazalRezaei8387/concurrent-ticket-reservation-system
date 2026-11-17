
cmake_minimum_required(VERSION 3.20)

project(ConcurrentTicketReservationSystem
    VERSION 1.0
    LANGUAGES CXX
)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)
find_package(PostgreSQL REQUIRED)

# Include project headers
include_directories(include)

# Main application
add_executable(ticket_reservation
    src/main.cpp
    src/database/DatabaseConnection.cpp
    src/domain/User.cpp
    src/domain/Event.cpp
    src/domain/Seat.cpp
    src/repositories/UserRepository.cpp
    src/repositories/EventRepository.cpp
    src/repositories/SeatRepository.cpp
)

target_link_libraries(ticket_reservation
    PRIVATE
        PostgreSQL::PostgreSQL
        PkgConfig::PQXX
)

# Tests
if(BUILD_TESTING)

    find_package(GTest REQUIRED)
    include(GoogleTest)

    # Unit Tests
    add_executable(user_tests
        tests/unit/UserTest.cpp
        src/domain/User.cpp

        tests/unit/EventTest.cpp
        src/domain/Event.cpp

        tests/unit/SeatTest.cpp
        src/domain/Seat.cpp

        tests/unit/ReservationTest.cpp
        src/domain/Reservation.cpp
    )

    target_link_libraries(user_tests
        PRIVATE
            GTest::gtest_main
    )

    gtest_discover_tests(user_tests)

    # Integration Tests
    add_executable(user_repository_tests
        tests/integration/UserRepositoryTest.cpp
        tests/integration/EventRepositoryTest.cpp

        src/database/DatabaseConnection.cpp

        src/domain/User.cpp
        src/domain/Event.cpp

        src/repositories/UserRepository.cpp
        src/repositories/EventRepository.cpp

        tests/integration/SeatRepositoryTest.cpp
        src/domain/Seat.cpp
        src/repositories/SeatRepository.cpp
    )

    target_link_libraries(user_repository_tests
        PRIVATE
            GTest::gtest_main
            PostgreSQL::PostgreSQL
            PkgConfig::PQXX
    )

    gtest_discover_tests(user_repository_tests)

endif()
