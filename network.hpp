#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

const int INF = 2147483647;

struct Edge {
    int from, to, capacity, flow;
};

struct Node {
    int id, type;
};

class Network {
private:
    int numNodes, numEdges;
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::vector<std::vector<int>> adj;  // Adjacency list    

public:
    Network();
    void readInput();
};

#endif
