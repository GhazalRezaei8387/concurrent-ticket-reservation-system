#include <gtest/gtest.h>

#include "services/SeatService.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class FakeSeatRepository : public ISeatRepository {
public:
    Seat create(
        std::int64_t eventId,
        const std::string& seatNumber,
        const std::string& seatType
    ) override {
        Seat seat(
            nextId++,
            eventId,
            seatNumber,
            seatType
        );

        seats.push_back(seat);
        return seat;
    }

    std::optional<Seat> findById(
        std::int64_t seatId
    ) override {
        for (const auto& seat : seats) {
            if (seat.getSeatId() == seatId) {
                return seat;
            }
        }

        return std::nullopt;
    }

    std::vector<Seat> findByEventId(
        std::int64_t eventId
    ) override {
        std::vector<Seat> result;

        for (const auto& seat : seats) {
            if (seat.getEventId() == eventId) {
                result.push_back(seat);
            }
        }

        return result;
    }

private:
    std::vector<Seat> seats;
    std::int64_t nextId = 1;
};

TEST(SeatServiceTest, CreatesNewSeat) {
    FakeSeatRepository repository;
    SeatService service(repository);

    Seat seat = service.createSeat(
        1,
        "A1",
        "VIP"
    );

    EXPECT_GT(seat.getSeatId(), 0);
    EXPECT_EQ(seat.getEventId(), 1);
    EXPECT_EQ(seat.getSeatNumber(), "A1");
    EXPECT_EQ(seat.getSeatType(), "VIP");
}

TEST(SeatServiceTest, FindsSeatById) {
    FakeSeatRepository repository;
    SeatService service(repository);

    Seat createdSeat = service.createSeat(
        1,
        "A1",
        "VIP"
    );

    const auto foundSeat =
        service.getSeatById(createdSeat.getSeatId());

    ASSERT_TRUE(foundSeat.has_value());
    EXPECT_EQ(foundSeat->getSeatNumber(), "A1");
}

TEST(SeatServiceTest, ReturnsSeatsByEventId) {
    FakeSeatRepository repository;
    SeatService service(repository);

    service.createSeat(1, "A1", "VIP");
    service.createSeat(1, "A2", "VIP");
    service.createSeat(2, "B1", "Regular");

    const auto seats = service.getSeatsByEventId(1);

    ASSERT_EQ(seats.size(), 2);
    EXPECT_EQ(seats[0].getSeatNumber(), "A1");
    EXPECT_EQ(seats[1].getSeatNumber(), "A2");
}
