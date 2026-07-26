#include "OrderBook.h"

void OrderBook::addOrder(const Order& order)
{
    if (order.getSide() == Side::Buy)
    {
        auto& level = bids[order.getPrice()];

        level.addOrder(order);

        orderIdMap[order.getID()] = {
            level.lastIterator(),
            order.getPrice(),
            order.getSide()
        };
    }
    else
    {
        auto& level = asks[order.getPrice()];

        level.addOrder(order);

        orderIdMap[order.getID()] = {
            level.lastIterator(),
            order.getPrice(),
            order.getSide()
        };
    }
}

bool OrderBook::cancelOrder(OrderId orderId)
{
    auto it = orderIdMap.find(orderId);

    if (it == orderIdMap.end())
    {
        return false;
    }

    auto [orderIterator, price, side] = it->second;

    if (side == Side::Buy)
    {
        bids[price].eraseOrder(orderIterator);

    }
    else
    {
        asks[price].eraseOrder(orderIterator);
    }

    orderIdMap.erase(it);

    return true;
}

bool OrderBook::modifyOrder(OrderId orderId, Price newPrice, Quantity newQuantity)
{
    auto it = orderIdMap.find(orderId);

    if (it == orderIdMap.end())
    {
        return false;
    }

    Side side = std::get<2>(it->second);
    if (!cancelOrder(orderId)) {
        return false;
    }
    Order modifiedOrder(orderId, newQuantity, newPrice, side, OrderType::Limit);
    addOrder(modifiedOrder);

    return true;
}

void OrderBook::printOrders() const
{
    printSide(bids);
    printSide(asks);
}

std::map<Price, PriceLevel, std::greater<Price>>& OrderBook::getBids()
{
    return bids;
}

std::map<Price, PriceLevel>& OrderBook::getAsks()
{
    return asks;
}

const std::map<Price, PriceLevel, std::greater<Price>>& OrderBook::getBids() const
{
    return bids;
}

const std::map<Price, PriceLevel>& OrderBook::getAsks() const
{
    return asks;
}