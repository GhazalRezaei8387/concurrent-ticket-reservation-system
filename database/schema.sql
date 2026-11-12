CREATE TABLE users (
    user_id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    full_name VARCHAR(100) NOT NULL,
    email VARCHAR(255) NOT NULL UNIQUE,
    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE events (
    event_id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    title VARCHAR(150) NOT NULL,
    description TEXT,
    location VARCHAR(255) NOT NULL,
    event_date TIMESTAMPTZ NOT NULL
);

CREATE TABLE seats (
    seat_id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    event_id BIGINT NOT NULL,
    seat_number VARCHAR(10) NOT NULL,
    seat_type VARCHAR(30) NOT NULL,

    CONSTRAINT fk_seat_event
        FOREIGN KEY (event_id)
        REFERENCES events(event_id),

    CONSTRAINT unique_seat_per_event
        UNIQUE (event_id, seat_number)
);

CREATE TABLE reservations (
    reservation_id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    user_id BIGINT NOT NULL,
    seat_id BIGINT NOT NULL,
    status VARCHAR(20) NOT NULL DEFAULT 'CONFIRMED',
    reserved_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    cancelled_at TIMESTAMPTZ,

    CONSTRAINT fk_reservation_user
        FOREIGN KEY (user_id)
        REFERENCES users(user_id),

    CONSTRAINT fk_reservation_seat
        FOREIGN KEY (seat_id)
        REFERENCES seats(seat_id),

    CONSTRAINT check_reservation_status
        CHECK (status IN ('CONFIRMED', 'CANCELLED')),

    CONSTRAINT check_cancellation_time
        CHECK (
            (status = 'CONFIRMED' AND cancelled_at IS NULL)
            OR
            (status = 'CANCELLED' AND cancelled_at IS NOT NULL)
        )
);

CREATE UNIQUE INDEX unique_active_reservation_per_seat
ON reservations (seat_id)
WHERE status = 'CONFIRMED';