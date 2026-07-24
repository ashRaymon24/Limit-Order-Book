#pragma once

#include <deque>
#include <iterator>

#include "Order.h"

using OrderIterator = std::deque<Order>::iterator;

class PriceLevel {
private:
    std::deque<Order> orders;

public:
    void addOrder(const Order& order);

    void eraseOrder(OrderIterator orderIterator);

    Order& front();

    OrderIterator lastIterator();

    void popFront();

    bool empty() const noexcept;
};