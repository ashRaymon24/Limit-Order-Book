#include <iostream>
#include <cstdint>
#include <map>
#include <deque>

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
    public:
        void addOrder(OrderId id, Quantity quantity, Price price, Side side) {
            if (side == Side::Buy) {
                bids[price].emplace_back(id, quantity, price, side);
            } else {
                asks[price].emplace_back(id, quantity, price, side);
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
