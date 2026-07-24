#include "MatchingEngine.h"
#include "OrderBook.h"

void MatchingEngine::processOrder(Order incomingOrder) {
    auto& asks = orderBook.getAsks();
    auto& bids = orderBook.getBids();
    bool isMarketOrder = incomingOrder.getType() == OrderType::Market;
    bool isImmediateOrCancel = incomingOrder.getType() == OrderType::ImmediateOrCancel;

    if (incomingOrder.getSide() == Side::Buy) {
        auto it = asks.begin();
        while (incomingOrder.getQuantity() > 0 && it != asks.end()) {
            Price bestPrice = it->first;
            if (bestPrice > incomingOrder.getPrice() && !isMarketOrder) {
                break;
            }
            auto& ordersAtPrice = it->second;
            while (!ordersAtPrice.empty() && incomingOrder.getQuantity() > 0) {
                Order& existingOrder = ordersAtPrice.front();
                Quantity matchQuantity = std::min(incomingOrder.getQuantity(), existingOrder.getQuantity());
                Trade trade(incomingOrder.getID(), existingOrder.getID(), bestPrice, matchQuantity);
                trades.push_back(trade);

                incomingOrder.setQuantity(incomingOrder.getQuantity() - matchQuantity);
                existingOrder.setQuantity(existingOrder.getQuantity() - matchQuantity);

                if (existingOrder.isFilled()) {
                    cancelOrder(existingOrder.getID());
                    //ordersAtPrice.popFront();
                }
            }

            if (ordersAtPrice.empty()) {
                it = asks.erase(it);
            } else {
                ++it;
            }
        }
    } else {
        auto it = bids.begin();
        while (incomingOrder.getQuantity() > 0 && it != bids.end()) {
            Price bestPrice = it->first;
            if (bestPrice < incomingOrder.getPrice() && !isMarketOrder) {
                break;
            }

            auto& ordersAtPrice = it->second;
            while (!ordersAtPrice.empty() && incomingOrder.getQuantity() > 0) {
                Order& existingOrder = ordersAtPrice.front();
                Quantity matchQuantity = std::min(incomingOrder.getQuantity(), existingOrder.getQuantity());
                Trade trade(existingOrder.getID(), incomingOrder.getID(), bestPrice, matchQuantity);
                trades.push_back(trade);

                incomingOrder.setQuantity(incomingOrder.getQuantity() - matchQuantity);
                existingOrder.setQuantity(existingOrder.getQuantity() - matchQuantity);

                if (existingOrder.isFilled()) {
                    cancelOrder(existingOrder.getID());
                    //ordersAtPrice.popFront();
                }
            }

            if (ordersAtPrice.empty()) {
                it = bids.erase(it);
            } else {
                ++it;
            }
        }
    }

    if (incomingOrder.getQuantity() > 0 && !isMarketOrder && !isImmediateOrCancel) {
        orderBook.addOrder(incomingOrder);
    }
}

bool MatchingEngine::cancelOrder(OrderId orderId) {
    return orderBook.cancelOrder(orderId);
}

void MatchingEngine::printOrderBook() const {
    orderBook.printOrders();
}
