#include "../include/Orderbook.h"
#include <iostream>
#include <algorithm>

void Orderbook::addorder(const Order& order) {
    if (order.side == OrderSide::BUY) {
        bids[order.price].push_back(order);
        auto it = --bids[order.price].end();
        OrderTracker[order.orderId] = it;
    } else {
        asks[order.price].push_back(order);
        auto it = --asks[order.price].end();
        OrderTracker[order.orderId] = it;
    }
    
    match();
}

void Orderbook::match() {
    while (!bids.empty() && !asks.empty()) {
        auto bestBid = bids.begin();
        auto bestAsk = asks.begin();

        double highestBuyPrice = bestBid->first;
        double lowestSellPrice = bestAsk->first;

        if (highestBuyPrice >= lowestSellPrice) {
            Order& buyer = bestBid->second.front();
            Order& seller = bestAsk->second.front();

            uint32_t tradeQuantity = std::min(buyer.quantity, seller.quantity);


            buyer.quantity -= tradeQuantity;
            seller.quantity -= tradeQuantity;

            if (buyer.quantity == 0) {
                OrderTracker.erase(buyer.orderId);
                bestBid->second.pop_front();
            }

            if (seller.quantity == 0) {
                OrderTracker.erase(seller.orderId);
                bestAsk->second.pop_front();
            }

            if (bestBid->second.empty()) {
                bids.erase(bestBid);
            }
            if (bestAsk->second.empty()) {
                asks.erase(bestAsk);
            }
        } else {
            break;
        }
    }
}

void Orderbook::cancelorder(uint64_t orderId) {
    auto it = OrderTracker.find(orderId);
    
    if (it == OrderTracker.end()) {
        return; // Silent failure for benchmarking
    }

    OrderList::iterator orderLocation = it->second;
    double orderPrice = orderLocation->price;
    OrderSide side = orderLocation->side;

    if (side == OrderSide::BUY) {
        bids[orderPrice].erase(orderLocation);
        if (bids[orderPrice].empty()) {
            bids.erase(orderPrice);
        }
    } else {
        asks[orderPrice].erase(orderLocation);
        if (asks[orderPrice].empty()) {
            asks.erase(orderPrice);
        }
    }

    OrderTracker.erase(orderId);
}

void Orderbook::printBook() const {
    std::cout << "\n========== CURRENT ORDER BOOK ==========\n";
    std::cout << "--- ASKS (SELLERS) ---\n";
    for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
        uint32_t totalQuantity = 0;
        for (const auto& order : it->second) totalQuantity += order.quantity;
        std::cout << "Rs " << it->first << " | Qty: " << totalQuantity << "\n";
    }

    std::cout << "--- BIDS (BUYERS) ---\n";
    for (auto it = bids.begin(); it != bids.end(); ++it) {
        uint32_t totalQuantity = 0;
        for (const auto& order : it->second) totalQuantity += order.quantity;
        std::cout << "Rs " << it->first << " | Qty: " << totalQuantity << "\n";
    }
    std::cout << "========================================\n\n";
}
