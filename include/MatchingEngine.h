#pragma once

#include <vector>

#include "OrderBook.h"
#include "Trade.h"

class MatchingEngine {
private:
    OrderBook orderBook;
    std::vector<Trade> trades;

public:
    MatchingEngine() = default;

    void processOrder(Order order);

    bool cancelOrder(OrderId orderId);

    void printOrderBook() const;

    const std::vector<Trade>& getTrades() const
    {
        return trades;
    }
};