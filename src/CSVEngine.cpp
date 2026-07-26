#include "CSVEngine.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


CSVEngine::CSVEngine(MatchingEngine& engine) : matchingEngine(engine) {}

void CSVEngine::replay(const std::string& filename) {
    std::size_t addCount = 0;
    std::size_t modifyCount = 0;
    std::size_t cancelCount = 0;
    // Implementation for replaying orders from a CSV file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::vector<std::string> row;
    std::string line;
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        row.clear();

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        if (row.empty()) {
            continue; // Skip empty lines
        }
        // Assuming the CSV has the following columns: OrderId, Side, Price, Quantity, OrderType
        std::string command = row[0];
        try {
                if (command == "ADD" && row.size() == 6) {
                OrderId orderId = static_cast<OrderId>(std::stoull(row[1]));
                Side side;
                if (row[2] == "BUY") {
                    side = Side::Buy;
                } else if (row[2] == "SELL") {
                    side = Side::Sell;
                } else {
                    std::cerr << "Unknown side: " << row[2] << std::endl;
                    continue;
                }
                Price price = std::stoi(row[3]);
                Quantity quantity = std::stoul(row[4]);
                OrderType orderType;
                if (row[5] == "MARKET") {
                    orderType = OrderType::Market;
                } else if (row[5] == "IOC") {
                    orderType = OrderType::ImmediateOrCancel;
                } else if (row[5] == "GTC") {
                    orderType = OrderType::GoodTillCancel;
                } else if (row[5] == "FAK") {
                    orderType = OrderType::FillAndKill;
                }
                else{
                    std::cerr << "Unknown order type: " << row[5] << std::endl;
                    continue;
                }
                Order order(orderId, quantity, price, side, orderType);
                matchingEngine.processOrder(order);
                addCount++;
            }
            else if (command == "MODIFY" && row.size() == 4) {
                OrderId orderId = static_cast<OrderId>(std::stoull(row[1]));
                Price newPrice = std::stoi(row[2]);
                Quantity newQuantity = std::stoul(row[3]);
                matchingEngine.modifyOrder(orderId, newPrice, newQuantity);
                modifyCount++;
            }
            else if (command == "CANCEL" && row.size() == 2) {
                OrderId orderId = static_cast<OrderId>(std::stoull(row[1]));
                matchingEngine.cancelOrder(orderId);
                cancelCount++;
            }
            else {
                std::cerr << "Unknown command or incorrect number of columns: " << line << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error processing row: " << line << ". Exception: " << e.what() << std::endl;
        }
    }
    std::cout
        << "Processed:\n"
        << "Adds: " << addCount << '\n'
        << "Modifies: " << modifyCount << '\n'
        << "Cancels: " << cancelCount << '\n';

}