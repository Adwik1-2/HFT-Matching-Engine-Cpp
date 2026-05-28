#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Orderbook.h"

class ThreadSafeQueue {
private:
    std::queue<Order> q;
    std::mutex mtx;
    std::condition_variable cv;
    bool done = false;

public:
    void push(const Order& order) {
        std::lock_guard<std::mutex> lock(mtx);
        q.push(order);
        cv.notify_one();
    }

    bool pop(Order& order) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return !q.empty() || done; });
        
        if (q.empty() && done) {
            return false;
        }
        
        order = q.front();
        q.pop();
        return true;
    }

    void setDone() {
        std::lock_guard<std::mutex> lock(mtx);
        done = true;
        cv.notify_all();
    }
};