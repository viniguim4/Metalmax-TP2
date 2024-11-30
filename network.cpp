#include "network.hpp"
#include <iostream>
#include <vector>

Network::Network() : numNodes(0), numEdges(0) {}

void Network::readInput() {
    std::cin >> numNodes >> numEdges;
    nodes.resize(numNodes);
    adj.resize(numNodes);
    
    for (int i = 0; i < numNodes; ++i) {
        int id, type;
        std::cin >> id >> type;
        nodes[i].id = id - 1;  // Convert to 0-based indexing
        nodes[i].type = type;
    }
    
    for (int i = 0; i < numEdges; ++i) {
        Edge edge;
        std::cin >> edge.from >> edge.to >> edge.capacity;
        --edge.from; // Convert to 0-based indexing
        --edge.to;
        edge.flow = 0;
        edges.push_back(edge);
        adj[edge.from].push_back(static_cast<int>(edges.size() - 1));  // Store edge index in adjacency list
    }
}
