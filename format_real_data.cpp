#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
using namespace std;

int main() {
    // 1. Downloaded file
    ifstream infile("stanford_graph.txt"); 
    ofstream outfile("network_data.csv");

    if (!infile.is_open() || !outfile.is_open()) {
        cerr << "Error: Could not open the text files. Check the names!" << endl;
        return 1;
    }

    // 2. Set up the random bandwidth generator (100 to 10,000 Mbps)
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> capacity_dist(1, 100);

    string line;
    int edge_count = 0;

    cout << "Reading Stanford topology and generating bandwidths..." << endl;

    // 3. Parse the real data
    while (getline(infile, line)) {
        // Skip comment lines in the Stanford file
        if (line.empty() || line[0] == '#') continue; 

        stringstream ss(line);
        string u, v;
        
        // Stanford files separate nodes by spaces or tabs
        if (ss >> u >> v) {
            int capacity = capacity_dist(gen) * 100;
            
            // Format it exactly how the engine expects it
            outfile << "Router_" << u << ",Router_" << v << "," << capacity << "\n";
            edge_count++;
        }
    }

    infile.close();
    outfile.close();

    cout << "Success! Processed " << edge_count << " real-world connections into network_data.csv.\n";
    return 0;
}
