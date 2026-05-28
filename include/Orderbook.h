#pragma once
#include <map>
#include <unordered_map>
#include <list>
#include <iostream>
#include "Order.h"

class Orderbook {
private:
    using OrderList = std::list<Order>;
    std::map<double,OrderList,std::greater<double>> bids; 
    std::map<double,OrderList>asks;
    std::unordered_map<uint64_t,OrderList::iterator>OrderTracker;
public:
    void addorder(const Order& order);
    void cancelorder(uint64_t orderId);
    void printBook() const;

private:
    void match();

};