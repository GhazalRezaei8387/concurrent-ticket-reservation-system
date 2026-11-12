# Requirements

## 1. Project Overview

The Concurrent Ticket Reservation System is a command-line application for reserving individually numbered seats for scheduled events. It is designed to demonstrate safe booking under concurrent requests using C++17 and PostgreSQL.

## 2. Reservation Model

**Seat-based, direct booking** is the selected model.

- Each event has its own set of numbered seats.
- A user selects a specific seat for a specific event.
- A successful booking is confirmed immediately after it is committed to the database.
- There is no temporary hold, payment flow, or reservation-expiration process in the initial version.

## 3. Core Domain Entities

### User
Represents a person who can make reservations.

Initial attributes:
- `id`: unique identifier
- `name`: user's display name
- `email`: unique email address
- `created_at`: account creation timestamp

### Event
Represents an event for which seats can be reserved.

Initial attributes:
- `id`: unique identifier
- `name`: event name
- `venue`: venue or location
- `starts_at`: event date and time
- `created_at`: record creation timestamp

### Seat
Represents one individually identifiable seat belonging to one event.

Initial attributes:
- `id`: unique identifier
- `event_id`: the event this seat belongs to
- `row_label`: row identifier, such as A
- `seat_number`: seat number within the row

The combination of `event_id`, `row_label`, and `seat_number` must be unique.

### Reservation
Represents a user's booking of one seat.

Initial attributes:
- `id`: unique identifier
- `user_id`: the user who made the reservation
- `seat_id`: the reserved seat
- `status`: initially either `confirmed` or `cancelled`
- `created_at`: reservation creation timestamp
- `cancelled_at`: cancellation timestamp, nullable

A reservation record is retained after cancellation so that booking history is not lost.

## 4. Functional Requirements

1. The system can create and list users.
2. The system can create and list events.
3. Each event has a predefined set of seats.
4. Users can view the seats belonging to an event and identify which are available.
5. A user can reserve an available seat.
6. A successful reservation is confirmed immediately.
7. A user can cancel only their own confirmed reservation.
8. A cancelled seat becomes available for a new reservation.
9. A user may reserve multiple seats by creating separate reservations.
10. The system retains cancelled reservation records.

## 5. Concurrency and Data Integrity Requirements

1. Concurrent attempts to reserve the same seat must not create more than one active reservation.
2. The reservation operation must use a PostgreSQL transaction.
3. The selected seat row must be locked during the booking operation using row-level locking (for example, `SELECT ... FOR UPDATE`).
4. Database constraints must provide a final integrity guarantee against duplicate active reservations for the same seat.
5. If a booking fails, its transaction must not leave partially applied changes.
6. The system must behave correctly when booking requests come from multiple threads or application instances. An in-process mutex alone is not sufficient for this guarantee.

## 6. Initial Acceptance Scenario

Given one event with one available seat, when 100 concurrent booking requests attempt to reserve that same seat, exactly one request must succeed and the remaining requests must fail cleanly. The database must contain exactly one active reservation for that seat.

## 7. Initial Scope Exclusions

The first version does not include:
- Online payments or refunds
- Temporary seat holds or reservation expiration
- A graphical user interface
- Distributed services or microservice architecture
- Advanced user authentication and password management

These may be considered later only if they add clear value without distracting from the core concurrency goals.

## 8. Technical Constraints

- Language: C++17
- Build system: CMake
- Database: PostgreSQL
- PostgreSQL C++ client: libpqxx
- Concurrency: C++ standard threading facilities
- Testing: Google Test
- Interface: command-line application
