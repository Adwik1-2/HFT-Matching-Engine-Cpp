# 🚀 Low-Latency HFT Limit Order Book (LOB) Engine

A high-performance, multi-threaded Limit Order Book matching engine implemented in Modern C++ (C++17). This engine simulates the core backend of an exchange (like NSE or Binance), matching buy and sell orders based on strict **Price-Time Priority** in sub-millisecond latency.

## ⚡ Core Features
* **O(log N) Price Matching:** Utilizes Red-Black Trees to constantly maintain the best Bid and Ask prices.
* **O(1) Order Execution & Tie-Breaking:** Employs Doubly Linked Lists to naturally resolve price ties using FIFO (Time Priority) without sorting overhead.
* **O(1) Order Cancellations:** Hash map integration allows instant memory lookup and deletion of cancelled orders.
* **Multi-Threaded Architecture:** Implements a Producer-Consumer pattern using `std::thread` and `std::condition_variable` for parallel order ingestion and matching.

## 🛠️ Tech Stack & Tools
* **Language:** C++17
* **Concurrency:** `std::thread`, `std::mutex`, `std::condition_variable`
* **Compiler:** GCC (MSYS2 UCRT64 - POSIX thread model)
* **Optimization:** `-O3` compiler flags

## 🧠 Data Structure Architecture (The Secret Sauce)
1. **Price Priority (`std::map`):** Keeps prices strictly sorted. Buyers get the highest available price first, and sellers get the lowest.
2. **Time Priority (`std::list`):** Orders at the same price are pushed into a linked list. `push_back()` ensures natural time ordering (first come, first served).
3. **Order Tracking (`std::unordered_map`):** Maps unique Order IDs directly to their memory iterators, bypassing the need to traverse the tree during cancellations.

## 📊 Performance Benchmarks
Tested on a standard consumer-grade CPU using a 1,000,000 (1 Million) order injection load test:
* **Total Orders Processed:** 1,000,000
* **Execution Time:** ~614 milliseconds
* **Throughput:** ~1.6 Million orders/second
* **Average Latency:** < 650 nanoseconds per order

## ⚙️ How to Build and Run
Ensure you have a POSIX-compliant C++ compiler (like MSYS2 for Windows or GCC for Linux) installed.

**1. Compile with max optimizations:**
```bash
# Standard command (if GCC is in your PATH)
g++ -O3 src/main.cpp src/OrderBook.cpp -o hft_benchmark

# Example command using explicit MSYS2 UCRT64 path on Windows
C:\msys64\ucrt64\bin\g++.exe -O3 src/main.cpp src/OrderBook.cpp -o hft_benchmark