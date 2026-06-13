#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <chrono>
using namespace std;

// Custom Max-Heap Implementation
class MaxHeap {
private:
    vector<pair<int, int>> heap; // pair<bandwidth, node_id>

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].first > heap[parent].first) {
                swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < size && heap[left].first > heap[largest].first) largest = left;
            if (right < size && heap[right].first > heap[largest].first) largest = right;

            if (largest != index) {
                swap(heap[index], heap[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }

public:
    void push(int bandwidth, int node) {
        heap.push_back({bandwidth, node});
        heapifyUp(heap.size() - 1);
    }

    pair<int, int> pop() {
        if (heap.empty()) return {-1, -1};
        pair<int, int> topElement = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return topElement;
    }

    bool isEmpty() { return heap.empty(); }
};

// Widest-Path Algorithm (Modified Dijkstra) returning the bottleneck bandwidth
int getBottleneckBandwidth(int nodes, int source, int dest, const vector<vector<pair<int, int>>>& adj) {
    vector<int> max_bandwidth(nodes, 0);
    MaxHeap maxHeap;

    max_bandwidth[source] = 1e9; // Infinity capacity for the source node itself
    maxHeap.push(1e9, source);

    while (!maxHeap.isEmpty()) {
        auto [current_bw, u] = maxHeap.pop();

        if (current_bw < max_bandwidth[u]) continue;

        // Early Exit Optimization: If we just processed the destination, we are done!
        if (u == dest) return max_bandwidth[dest];

        for (auto edge : adj[u]) {
            int v = edge.first;
            int capacity = edge.second;

            // Bottleneck logic: max(current_known, min(path_to_u, edge_capacity))
            int path_bandwidth = min(current_bw, capacity);

            if (path_bandwidth > max_bandwidth[v]) {
                max_bandwidth[v] = path_bandwidth;
                maxHeap.push(path_bandwidth, v);
            }
        }
    }
    return max_bandwidth[dest]; // Will return 0 if unreachable
}

int main() {
    // 1. The Translation Map
    unordered_map<string, int> node_to_id;
    int current_node_id = 0; // Starts at 0 and goes up

    // 2. The Adjacency List (starts empty, grows dynamically)
    vector<vector<pair<int, int>>> adj;

    // 3. Open the CSV file
    ifstream file("network_data.csv");
    if (!file.is_open()) {
        cerr << "Error: Could not open 'network_data.csv'. Ensure it is in the same directory!" << endl;
        return 1;
    }

    string line;
    // 4. Read the file line by line
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; 

        stringstream ss(line);
        string source_str, dest_str, capacity_str;

        // Parse the line broken by commas
        getline(ss, source_str, ',');
        getline(ss, dest_str, ',');
        getline(ss, capacity_str, ',');

        int capacity = stoi(capacity_str);

        // --- TRANSLATION LOGIC ---
        if (node_to_id.find(source_str) == node_to_id.end()) {
            node_to_id[source_str] = current_node_id++;
            adj.push_back({}); // Add an empty list for this new node
        }
        
        if (node_to_id.find(dest_str) == node_to_id.end()) {
            node_to_id[dest_str] = current_node_id++;
            adj.push_back({}); 
        }

        int u = node_to_id[source_str];
        int v = node_to_id[dest_str];

        // 5. Add the edge to the adjacency list (Undirected graph)
        adj[u].push_back({v, capacity});
        adj[v].push_back({u, capacity}); 
    }

    file.close();

    cout << "\n=========================================" << endl;
    cout << "      BOTTLENECK ENGINE INITIALIZED      " << endl;
    cout << "=========================================" << endl;
    cout << "Loaded " << current_node_id << " routers into the network topology." << endl;

    // 6. The Interactive CLI Loop
    string src_str, dest_str;
    while (true) {
        cout << "\nEnter Source Router (or type 'exit' to quit): ";
        
        if (!(cin >> src_str)) {
            cout << "\n[Error] Standard input stream failed. Exiting.\n";
            break;
        }

        if (src_str == "exit") break;

        cout << "Enter Destination Router: ";
        if (!(cin >> dest_str)) break; 

        if (node_to_id.find(src_str) == node_to_id.end() || node_to_id.find(dest_str) == node_to_id.end()) {
            cout << "[Error] One or both routers do not exist in the database. Try again.\n";
            continue;
        }

        int src_id = node_to_id[src_str];
        int dest_id = node_to_id[dest_str];

        // --- START TIMER ---
        auto start_time = chrono::high_resolution_clock::now();

        int optimal_bandwidth = getBottleneckBandwidth(current_node_id, src_id, dest_id, adj);

        // --- STOP TIMER ---
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

        cout << "\n-----------------------------------------";
        if (src_id == dest_id) {
            cout << "\nResult: Source and destination are the same node.\n";
        } else if (optimal_bandwidth == 0) {
            cout << "\nResult: No physical connection exists between " << src_str << " and " << dest_str << ".\n";
        } else {
            cout << "\nResult: Maximum possible bandwidth from " << src_str << " to " << dest_str 
                 << " is " << optimal_bandwidth << " Mbps.\n";
            cout << "[Benchmark] Path computed in: " << duration.count() << " microseconds.\n";
        }
        cout << "-----------------------------------------\n";
    }

    cout << "Engine shutting down...\n";
    return 0;
}