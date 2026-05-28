#pragma once
#include <cstdint>
#include <string>

enum class OrderSide{
    BUY,SELL
};

struct Order{
    uint64_t orderId;       
    std::string ticker;     
    OrderSide side;         
    double price;           
    uint32_t quantity;      
    uint64_t timestamp;
    Order(uint64_t id, std::string t, OrderSide s, double p, uint32_t q, uint64_t ts)
        : orderId(id), ticker(t), side(s), price(p), quantity(q), timestamp(ts) {}
};
