#include <iostream>
#include <cstdint>
#include <map>
#include <deque>
#include <algorithm>

enum class OrderType {
    GoodTillCancel,
    FillAndKill
};
enum class Side {
    Buy,
    Sell
};

using Price = std::int32_t;
using Quantity = std::uint32_t;
using OrderId = std::uint64_t;

class Order {
    private:
        OrderId id;
        Quantity quantity;
        Price price;
        Side side;
    public:
        Order(OrderId id, Quantity quantity, Price price, Side side)
            : id(id), quantity(quantity), price(price), side(side) {}

        OrderId getID() const { return id; }
        Quantity getQuantity() const { return quantity; }
        Price getPrice() const { return price; }
        Side getSide() const { return side; }

        void setQuantity(Quantity quantity) { this->quantity = quantity; }
};

class OrderBook {
    private:
        std::map<Price, std::deque<Order>, std::greater<Price>> bids;
        std::map<Price, std::deque<Order>> asks;

        void addOrder(const Order& order) {
            if (order.getSide() == Side::Buy) {
                bids[order.getPrice()].emplace_back(order.getID(), order.getQuantity(), order.getPrice(), order.getSide());
            } else {
                asks[order.getPrice()].emplace_back(order.getID(), order.getQuantity(), order.getPrice(), order.getSide());
            }
        }
        void matchOrder(Order& incomingOrder){
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
    public:
        void processOrder(Order order) {
            matchOrder(order);
            if (order.getQuantity() > 0) {
                addOrder(order);
            }
        }
        
        void printOrders() const {
            printSide(bids);
            printSide(asks);
        }
        template<typename Map>
        void printSide(const Map& priceLevels) const {
            for (const auto& [price, orders] : priceLevels) {
                std::cout << "Price: " << price << std::endl;
                for (const auto& order : orders) {
                    std::cout << "Order ID: " << order.getID()
                              << ", Quantity: " << order.getQuantity()
                              << ", Price: " << order.getPrice()
                              << ", Side: " << (order.getSide() == Side::Buy ? "Buy" : "Sell")
                              << std::endl;
                }
            }
        }

};
