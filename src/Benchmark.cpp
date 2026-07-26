#include "MatchingEngine.h"
#include "CSVEngine.h"

#include <fstream>
#include <random>
#include <string>
#include <chrono>

void generateCSV(const std::string& filename, std::size_t numEvents)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> eventDist(0, 99);
    std::uniform_int_distribution<int> sideDist(0, 1);
    std::uniform_int_distribution<int> priceDist(95, 105);
    std::uniform_int_distribution<int> quantityDist(1, 100);
    std::uniform_int_distribution<int> typeDist(0, 2);

    std::vector<OrderId> activeOrders;

    OrderId nextOrderId = 1;

    for (std::size_t i = 0; i < numEvents; ++i)
    {
        int eventType = eventDist(gen);

        // ADD (70%)
        if (activeOrders.empty() || eventType < 70)
        {
            std::string side =
                sideDist(gen) == 0
                ? "BUY"
                : "SELL";

            int price = priceDist(gen);

            int quantity =
                quantityDist(gen);

            std::string orderType;

            switch (typeDist(gen))
            {
                case 0:
                    orderType = "GTC";
                    break;

                case 1:
                    orderType = "IOC";
                    break;

                case 2:
                    orderType = "MARKET";
                    price = 0;
                    break;
            }

            file
                << "ADD,"
                << nextOrderId << ','
                << side << ','
                << price << ','
                << quantity << ','
                << orderType
                << '\n';

            activeOrders.push_back(nextOrderId);

            ++nextOrderId;
        }

        // MODIFY (20%)
        else if (eventType < 90)
        {
            std::uniform_int_distribution<std::size_t>
                orderDist(0, activeOrders.size() - 1);

            OrderId orderId =
                activeOrders[orderDist(gen)];

            int newPrice =
                priceDist(gen);

            int newQuantity =
                quantityDist(gen);

            file
                << "MODIFY,"
                << orderId << ','
                << newPrice << ','
                << newQuantity
                << '\n';
        }

        // CANCEL (10%)
        else
        {
            std::uniform_int_distribution<std::size_t>
                orderDist(0, activeOrders.size() - 1);

            std::size_t index =
                orderDist(gen);

            OrderId orderId =
                activeOrders[index];

            file
                << "CANCEL,"
                << orderId
                << '\n';

            activeOrders.erase(
                activeOrders.begin() + index);
        }
    }
}

int main()
{
    std::size_t nums[] = {100000, 500000, 1000000};
    for (std::size_t numOrder : nums) {
        std::size_t NUM_ORDERS = numOrder;
        MatchingEngine engine;
        generateCSV("orders.csv", NUM_ORDERS);
        CSVEngine csvEngine(engine);
        auto start = std::chrono::high_resolution_clock::now();
        csvEngine.replay("orders.csv");
        auto end = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                end - start);
        double seconds =
            duration.count() / 1000.0;
        std::cout << "\nBenchmark Results\n";
        std::cout << "-----------------\n";
        std::cout << "Orders Processed: "
                << NUM_ORDERS
                << '\n';

        std::cout << "Time Taken: "
                << seconds
                << " seconds\n";

        std::cout << "Throughput: "
                << static_cast<double>(NUM_ORDERS) / seconds
                << " orders/sec\n";
    }
    return 0;
}