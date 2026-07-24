#pragma once

#include <map>
#include <tuple>
#include <iostream>
#include <functional>
#include <unordered_map>

#include "Order.h"
#include "PriceLevel.h"

class OrderBook {
private:
    std::map<Price, PriceLevel, std::greater<Price>> bids;
    std::map<Price, PriceLevel> asks;

    std::unordered_map<
        OrderId,
        std::tuple<OrderIterator, Price, Side>
    > orderIdMap;

    template<typename Map>
    void printSide(const Map& priceLevels) const;

public:
    void addOrder(const Order& order);

    bool modifyOrder(OrderId orderId, Price newPrice, Quantity newQuantity);

    bool cancelOrder(OrderId orderId);

    void printOrders() const;

    auto& getBids();
    auto& getAsks();

    const auto& getBids() const;
    const auto& getAsks() const;
};

template<typename Map>
void OrderBook::printSide(const Map& priceLevels) const
{
    for (const auto& [price, level] : priceLevels)
    {
        std::cout << "Price: " << price << '\n';
    }
}