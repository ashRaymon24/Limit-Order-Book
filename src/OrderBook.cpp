#include "OrderBook.h"
#include <iostream>
#include <algorithm>


void OrderBook::addOrder(const Order& order) {
    if (order.getSide() == Side::Buy) {
        bids[order.getPrice()].emplace_back(order);
    } else {
        asks[order.getPrice()].emplace_back(order);
    }
}
       
void OrderBook::matchOrder(Order& incomingOrder){
    if (incomingOrder.getSide() == Side::Buy) {
        auto it = asks.begin();
        while (incomingOrder.getQuantity() > 0 && it != asks.end()) {
            Price bestPrice = it->first;
            if (bestPrice > incomingOrder.getPrice()){
                break; // No more matching possible
            }
            auto& ordersAtPrice = it->second;
            while (!ordersAtPrice.empty() && incomingOrder.getQuantity() > 0) {
                Order& existingOrder = ordersAtPrice.front();
                Quantity matchQuantity = std::min(incomingOrder.getQuantity(), existingOrder.getQuantity());
                incomingOrder.setQuantity(incomingOrder.getQuantity() - matchQuantity);
                existingOrder.setQuantity(existingOrder.getQuantity() - matchQuantity);
                if (existingOrder.getQuantity() == 0) {
                    ordersAtPrice.pop_front(); // Remove fully matched order
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
            if (bestPrice < incomingOrder.getPrice()) {
                break; // No more matching possible
            }
            auto& ordersAtPrice = it->second;
            while (!ordersAtPrice.empty() && incomingOrder.getQuantity() > 0) {
                Order& existingOrder = ordersAtPrice.front();
                Quantity matchQuantity = std::min(incomingOrder.getQuantity(), existingOrder.getQuantity());
                incomingOrder.setQuantity(incomingOrder.getQuantity() - matchQuantity);
                existingOrder.setQuantity(existingOrder.getQuantity() - matchQuantity);
                if (existingOrder.getQuantity() == 0) {
                    ordersAtPrice.pop_front(); // Remove fully matched order
                }
            }
            if (ordersAtPrice.empty()) {
                it = bids.erase(it);
            } else {
                ++it;
            }
        };
    };
}
void OrderBook::processOrder(Order order) {
    matchOrder(order);
    if (order.getQuantity() > 0) {
        addOrder(order);
    }
}
        
void OrderBook::printOrders() const {
    printSide(bids);
    printSide(asks);
}
