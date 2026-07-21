#pragma once

#include <map>
#include <deque>
#include <iostream>
#include <functional>

#include "Order.h"

class OrderBook
{
private:
    std::map<Price, std::deque<Order>, std::greater<Price>> bids;
    std::map<Price, std::deque<Order>> asks;

    void addOrder(const Order& order);
    void matchOrder(Order& incomingOrder);

    template<typename Map>
    void printSide(const Map& priceLevels) const;

public:
    void processOrder(Order order);
    void printOrders() const;
};

template<typename Map>
void OrderBook::printSide(const Map& priceLevels) const
{
    for (const auto& [price, orders] : priceLevels)
    {
        std::cout << "Price: " << price << '\n';

        for (const auto& order : orders)
        {
            std::cout
                << "Order ID: " << order.getID()
                << ", Quantity: " << order.getQuantity()
                << ", Price: " << order.getPrice()
                << ", Side: "
                << (order.getSide() == Side::Buy ? "Buy" : "Sell")
                << '\n';
        }
    }
}