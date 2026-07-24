#pragma once

#include "Types.h"

class Order {
private:
    OrderId id;
    Quantity quantity;
    Price price;
    Side side;
    OrderType type;

public:
    Order() = default;

    Order(OrderId id, Quantity quantity, Price price, Side side, OrderType type)
        : id(id),
          quantity(quantity),
          price(price),
          side(side),
          type(type)
    {
    }

    [[nodiscard]] OrderId getID() const noexcept {
        return id;
    }

    [[nodiscard]] Quantity getQuantity() const noexcept {
        return quantity;
    }

    [[nodiscard]] Price getPrice() const noexcept {
        return price;
    }

    [[nodiscard]] Side getSide() const noexcept {
        return side;
    }
    [[nodiscard]] OrderType getType() const noexcept {
        return type;
    }

    void setQuantity(Quantity newQuantity) noexcept {
        quantity = newQuantity;
    }

    [[nodiscard]] bool isFilled() const noexcept {
        return quantity == 0;
    }
};