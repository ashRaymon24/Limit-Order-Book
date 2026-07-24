#include "PriceLevel.h"

void PriceLevel::addOrder(const Order& order)
{
    orders.push_back(order);
}

Order& PriceLevel::front()
{
    return orders.front();
}

OrderIterator PriceLevel::lastIterator()
{
    return std::prev(orders.end());
}

void PriceLevel::popFront()
{
    orders.pop_front();
}

void PriceLevel::eraseOrder(OrderIterator orderIterator)
{
    orders.erase(orderIterator);
}

bool PriceLevel::empty() const noexcept
{
    return orders.empty();
}