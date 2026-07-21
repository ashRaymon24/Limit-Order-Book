#pragma once 

#include <cstdint>
#include "Types.h"

enum class Side : std::uint8_t {
    Buy,
    Sell
};


class Order {
private:
    OrderId id;
    Quantity quantity;
    Price price;
    Side side;

public:
    Order() = default;

    Order(OrderId id, Quantity quantity, Price price, Side side)
        : id(id), quantity(quantity), price(price), side(side) {}

    [[nodiscard]] OrderId getID() const noexcept { return id; }
    [[nodiscard]] Quantity getQuantity() const noexcept { return quantity; }
    [[nodiscard]] Price getPrice() const noexcept { return price; }
    [[nodiscard]] Side getSide() const noexcept { return side; }

    void setQuantity(Quantity newQuantity) noexcept { quantity = newQuantity; }
    [[nodiscard]] bool isFilled() const noexcept { return quantity == 0; }
};