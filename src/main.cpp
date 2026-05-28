#include "../include/Orderbook.h"
#include "../include/ThreadSafeQueue.h"
#include <iostream>
#include <chrono>
#include <random>
#include <thread>

void producer(ThreadSafeQueue& orderQueue, int numOrders) {
    std::mt19937 gen(42); 
    std::uniform_real_distribution<double> priceDist(490.0, 510.0);
    std::uniform_int_distribution<uint32_t> qtyDist(1, 100);
    std::uniform_int_distribution<int> sideDist(0, 1);

    for (int i = 0; i < numOrders; ++i) {
        OrderSide side = (sideDist(gen) == 0) ? OrderSide::BUY : OrderSide::SELL;
        double price = std::round(priceDist(gen) * 100.0) / 100.0;
        
        orderQueue.push(Order(i, "OSM", side, price, qtyDist(gen), i));
    }
    
    orderQueue.setDone();
}

void consumer(ThreadSafeQueue& orderQueue, Orderbook& ob) {
    Order order(0, "", OrderSide::BUY, 0.0, 0, 0);
    while (orderQueue.pop(order)) {
        ob.addorder(order);
    }
}

int main() {
    Orderbook ob;
    ThreadSafeQueue orderQueue;
    const int NUM_ORDERS = 1000000;

    std::cout << "--- HFT MULTI-THREADED BENCHMARK ---\n";
    std::cout << "Spinning up Producer and Consumer threads...\n";

    auto start = std::chrono::high_resolution_clock::now();

    std::thread prodThread(producer, std::ref(orderQueue), NUM_ORDERS);
    std::thread consThread(consumer, std::ref(orderQueue), std::ref(ob));

    prodThread.join();
    consThread.join();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "--- TEST COMPLETE ---\n";
    std::cout << "Processed " << NUM_ORDERS << " orders in " << duration.count() << " milliseconds!\n";

    return 0;
}