#pragma once

#include "Types.h"

class Trade {
private:
    OrderId buyerOrderId;
    OrderId sellerOrderId;
    Price price;
    Quantity quantity;

public:
    Trade(
        OrderId buyerOrderId,
        OrderId sellerOrderId,
        Price price,
        Quantity quantity)
        : buyerOrderId(buyerOrderId),
          sellerOrderId(sellerOrderId),
          price(price),
          quantity(quantity)
    {
    }

    [[nodiscard]] OrderId getBuyerOrderId() const noexcept {
        return buyerOrderId;
    }

    [[nodiscard]] OrderId getSellerOrderId() const noexcept {
        return sellerOrderId;
    }

    [[nodiscard]] Price getPrice() const noexcept {
        return price;
    }

    [[nodiscard]] Quantity getQuantity() const noexcept {
        return quantity;
    }
};