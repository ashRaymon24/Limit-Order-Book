# Limit Order Book

A C++ implementation of a limit order book built to learn how exchange matching engines work.

The project supports price-time priority matching, multiple order types, order modification, cancellation, trade generation, CSV replay, and basic performance benchmarking.



## Features

### Matching Engine

- Price-time priority matching
- Partial fills
- Trade generation
- Separate bid and ask books

### Order Types

- Limit Orders
- Market Orders
- Good Till Cancel (GTC)
- Immediate Or Cancel (IOC)

### Order Management

- Order submission
- Order cancellation
- Order modification
- O(1) order lookup and cancellation using an `unordered_map`

### Replay & Benchmarking

- Replay order events from a CSV file
- Generate synthetic workloads
- Measure replay throughput

---

## Project Structure

```text
include/
├── CSVEngine.h
├── MatchingEngine.h
├── Order.h
├── OrderBook.h
├── PriceLevel.h
├── Trade.h
└── Types.h

src/
├── Benchmark.cpp
├── CSVEngine.cpp
├── MatchingEngine.cpp
├── OrderBook.cpp
└── PriceLevel.cpp
```

---

## Design

The matching engine sits on top of the order book and is responsible for matching incoming orders and generating trades.

```text
MatchingEngine
│
├── OrderBook
│   ├── Bids
│   └── Asks
│
├── Trade History
│
└── CSV Replay Engine
```

Orders are grouped by price level and processed using FIFO ordering within each level.

### Order Book

Bid and ask levels are stored using `std::map`:

```cpp
std::map<Price, PriceLevel, std::greater<Price>> bids;
std::map<Price, PriceLevel> asks;
```

This provides quick access to the best bid and best ask.

### Cancellation

Orders are indexed by ID:

```cpp
std::unordered_map<
    OrderId,
    std::tuple<OrderIterator, Price, Side>
>
```

allowing orders to be cancelled without searching through the entire book.

---

## CSV Replay

The replay engine processes order events from a CSV file.

Supported commands:

```text
ADD
MODIFY
CANCEL
```

### Example

```csv
ADD,1,BUY,100,100,GTC
ADD,2,SELL,101,50,GTC
ADD,3,SELL,100,40,GTC
MODIFY,1,102,60
CANCEL,2
ADD,4,BUY,0,80,MARKET
```

This makes it possible to replay large event streams and benchmark the engine using realistic workloads.

---

## Benchmark Results

Replay workload:

- 70% ADD
- 20% MODIFY
- 10% CANCEL

### 100,000 Events

```text
462,963 events/sec
```

### 500,000 Events

```text
656,168 events/sec
```

### 1,000,000 Events

```text
630,517 events/sec
```

The engine sustained roughly **630k events/sec** when replaying a 1 million event workload.

---

## Building

Compile with:

```bash
g++ -std=c++17 src/*.cpp -Iinclude -o OrderBook.exe
``*

Run:

```bash
./OrderBook.exe
``*

---

## What I Learned

Some of *he main concepts explored in this *roject were:

- Price-time priorit* matching
- Exchange order book de*ign
- STL containers and iterators*- Order lifecycle management
- CSV*parsing and replay systems
- Perfo*mance benchmarking
- Modern C++ pr*ject structure

---

## Future Wor*

Potential extensions include:

- Unit tests
- Additional order types
- Multithreading
- Market data publishing
- Networked order entry

---

## Author

Asher Raymon

Built as a personal project to learn more about matching engines, market microstructure, and performance-focused C++ development.