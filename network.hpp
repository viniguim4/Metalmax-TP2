#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

const int INF = 2147483647;

struct Edge {
    int from, to, capacity, flow;
};

struct Node {
    int id, demand;
};

class Network {
private:
    int numNodes, numEdges;
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::vector<std::vector<int>> adj;
    
    bool bfs(int s, int t, std::vector<int>& parent);
    int fordFulkerson(int s, int t);
    void addEdge(int from, int to, int capacity);

public:
    Network();
    void readInput();
    bool hasCirculation();
};

#endif
