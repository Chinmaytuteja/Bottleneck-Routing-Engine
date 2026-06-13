# High-Performance Bottleneck Routing Engine

A C++ routing engine designed to find the maximum bandwidth path (widest path) between any two nodes in a massive, sparse network topology. 

## Technical Architecture
* **Algorithm:** Modified Dijkstra's Algorithm for the Widest Path problem.
* **Data Structures:** Custom array-based Max-Heap (priority queue) with dynamic `heapifyUp` and `heapifyDown` operations. Graph represented via adjacency lists dynamically mapped to integer IDs using `std::unordered_map` for O(1) lookups.
* **Complexity:** Safely processes 10,000+ node graphs in O((V + E) log V) time with early-exit optimization.
* **Benchmarking:** Integrated `<chrono>` high-resolution timer to measure microsecond-level path execution.

## Data Ingestion Pipeline
Rather than hosting massive static datasets, this repository includes a custom C++ data ingestion pipeline.

* `format_real_data.cpp`: Parses the real-world **Stanford Autonomous Systems (AS-733)** global ISP graph, sanitizes the inputs, and generates procedural edge capacities to format the data for the routing engine.

## How to Run

**1. Compile the pipeline and process the Stanford data:**
```bash
g++ -std=c++17 format_real_data.cpp -o format_real_data
./format_real_data
```

**2. Compile and run the interactive engine:**
```bash
g++ -std=c++17 Bottleneck_engine.cpp -o Bottleneck_engine
./Bottleneck_engine
```
