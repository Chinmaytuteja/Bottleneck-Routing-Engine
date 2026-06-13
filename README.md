# Bottleneck Routing Engine

A high-performance C++ engine that calculates the maximum possible data throughput (the "widest path") between two nodes in a global ISP network topology.

## Overview
This tool models physical network infrastructures to identify bandwidth bottlenecks. Instead of finding the shortest physical path, the engine calculates the route that guarantees the highest continuous data transfer rate across a series of routers. 

## Technical Architecture
* **Algorithm:** Implements a modified Dijkstra's Algorithm (Widest-Path Problem) to maximize the minimum edge weight along a path.
* **Data Structures:** Utilizes a custom-built **Max-Heap** (Priority Queue) to efficiently fetch the next optimal node, running in $O((V + E) \log V)$ time.
* **Data Pipeline:** Includes a custom parsing layer that translates raw, real-world `.csv` string data (e.g., "Router_Tokyo") into a dynamically allocated adjacency list using `std::unordered_map`.

## Features
* **Interactive CLI:** Allows users to query the maximum bandwidth between any two global routers in real-time.
* **Dynamic Sizing:** Capable of processing massive, non-contiguous real-world datasets with thousands of nodes and edges instantly.
* **Memory Efficient:** Operates entirely by reference to prevent massive memory copies during graph traversal.
