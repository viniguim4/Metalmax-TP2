#include "network.hpp"
#include <iostream>
#include <vector>

Network::Network() : numNodes(0), numEdges(0) {}

void Network::readInput() {
    std::cin >> numNodes >> numEdges;
    nodes.resize(numNodes);
    adj.resize(numNodes);
    
    // Lê as informações dos nós primeiro
    for (int i = 0; i < numNodes; ++i) {
        int id, demand;
        std::cin >> id >> demand;
        nodes[i].id = id - 1;  // Converte para indexação baseada em 0
        nodes[i].demand = demand;
    }
    
    // Lê e armazena as arestas
    for (int i = 0; i < numEdges; ++i) {
        Edge edge;
        std::cin >> edge.from >> edge.to >> edge.capacity;
        --edge.from; // Converte para indexação baseada em 0
        --edge.to;
        edge.flow = 0;
        edges.push_back(edge);
        adj[edge.from].push_back(static_cast<int>(edges.size() - 1));
    }
    
    // Calcula e define demandas negativas para vértices com demanda = 0
    for (int i = 0; i < numNodes; ++i) {
        if (nodes[i].demand == 0) {
            int outgoingSum = 0;
            for (int edgeIdx : adj[i]) {
                outgoingSum += edges[edgeIdx].capacity;
            }
            nodes[i].demand = -outgoingSum;  // Define demanda como negativo da soma das capacidades de saída
        }
    }
}

void Network::addEdge(int from, int to, int capacity) {
    Edge edge;
    edge.from = from;
    edge.to = to;
    edge.capacity = capacity;
    edge.flow = 0;
    edges.push_back(edge);
    adj[from].push_back(static_cast<int>(edges.size() - 1));
}

bool Network::bfs(int s, int t, std::vector<int>& parent) {
    std::vector<bool> visited(numNodes + 2, false);
    std::vector<int> queue;
    
    queue.push_back(s);
    visited[s] = true;
    parent[s] = -1;
    
    while (!queue.empty()) {
        int u = queue.front();
        queue.erase(queue.begin());
        
        for (int edgeIdx : adj[u]) {
            Edge& edge = edges[edgeIdx];
            int v = edge.to;
            
            if (!visited[v] && edge.capacity > edge.flow) {
                queue.push_back(v);
                parent[v] = edgeIdx;
                visited[v] = true;
                
                if (v == t)
                    return true;
            }
        }
    }
    return false;
}

int Network::fordFulkerson(int s, int t) {
    std::vector<int> parent(numNodes + 2);
    int maxFlow = 0;
    
    while (bfs(s, t, parent)) {
        int pathFlow = INF;
        
        // Encontra o fluxo mínimo no caminho
        for (int v = t; v != s; v = edges[parent[v]].from) {
            Edge& edge = edges[parent[v]];
            pathFlow = std::min(pathFlow, edge.capacity - edge.flow);
        }
        
        // Atualiza o fluxo
        for (int v = t; v != s; v = edges[parent[v]].from) {
            Edge& edge = edges[parent[v]];
            edge.flow += pathFlow;
        }
        
        maxFlow += pathFlow;
    }
    
    return maxFlow;
}

bool Network::hasCirculation() {
    // Adiciona source (s) e sink (t)
    int s = numNodes;
    int t = numNodes + 1;
    
    // Redimensiona o grafo para incluir s e t
    adj.resize(numNodes + 2);
    
    // Adiciona os nós source e sink
    Node source, sink;
    source.id = s;
    source.demand = 0;
    sink.id = t;
    sink.demand = 0;
    nodes.push_back(source);
    nodes.push_back(sink);
    
    int totalSupply = 0;
    
    // Conecta source aos vértices de suprimento (d(v) < 0)
    // e vértices de demanda ao sink (d(v) > 0)
    for (int i = 0; i < numNodes; i++) {
        if (nodes[i].demand < 0) {  // Supply vertex
            addEdge(s, i, -nodes[i].demand);
            totalSupply += -nodes[i].demand;
        }
        else if (nodes[i].demand > 0) {  // Demand vertex
            addEdge(i, t, nodes[i].demand);
        }
    }
    
    // Executa Ford-Fulkerson
    int maxFlow = fordFulkerson(s, t);
    
    // Remove os nós source e sink
    nodes.pop_back();
    nodes.pop_back();
    
    // Verifica se existe circulação
    return maxFlow == totalSupply;
}

int Network::maxCapacity() {
    int maxCapacity = 0;
    // soma de toda energia que há nas fontes
    for (int i = 0; i < numNodes; i++) {
        if (nodes[i].demand < 0) {
            maxCapacity += -nodes[i].demand;
        }
    }
    return maxCapacity;
}

int Network::getTotalEnergyCapacity() {
    // Create a copy of edges to preserve original capacities
    std::vector<Edge> originalEdges = edges;
    
    // Add source and sink
    int s = numNodes;
    int t = numNodes + 1;
    adj.resize(numNodes + 2);
    
    // Connect generators to source and consumers to sink
    for (int i = 0; i < numNodes; i++) {
        if (nodes[i].demand < 0) {  // Generator
            addEdge(s, i, INF);  // Unlimited capacity from source to generators
        }
        else if (nodes[i].demand > 0) {  // Consumer
            addEdge(i, t, nodes[i].demand);
        }
    }
    
    // Calculate maximum flow
    int maxFlow = fordFulkerson(s, t);
    
    // Restore original edges
    edges = originalEdges;
    adj.resize(numNodes);
    
    return maxFlow;
}

int Network::getUnmetEnergy() {
    int totalDemand = 0;
    int actualFlow = getTotalEnergyCapacity();
    
    // Calculate total demand from consumers
    for (const Node& node : nodes) {
        if (node.demand > 0) {
            totalDemand += node.demand;
        }
    }
    
    return totalDemand - actualFlow;
}

int Network::getLostEnergy() {
    int drainFromSource = 0;
    int maxSupply = maxCapacity();    

    // Create temporary graph with source and sink
    std::vector<Edge> originalEdges = edges;
    int s = numNodes;
    int t = numNodes + 1;
    adj.resize(numNodes + 2);
    
    // Connect generators to source and consumers to sink
    for (int i = 0; i < numNodes; i++) {
        if (nodes[i].demand < 0) {
            addEdge(s, i, INF);
        }
        else if (nodes[i].demand > 0) {
            addEdge(i, t, nodes[i].demand);
        }
    }
    
    // Run max flow to get the actual flow distribution
    fordFulkerson(s, t);
    
    // Calculate how much is being drained from source by summing flows from source
    for (const Edge& edge : edges) {
        if (edge.from == s) {
            drainFromSource += edge.flow;
        }
    }
    
    // Restore original graph
    edges = originalEdges;
    adj.resize(numNodes);
    
    return maxSupply - drainFromSource;
}

std::vector<Edge> Network::getCriticalConnections() {
    std::vector<Edge> criticalEdges;
    
    // Run max flow first
    int s = numNodes;
    int t = numNodes + 1;
    adj.resize(numNodes + 2);
    
    // Connect generators to source and consumers to sink
    for (int i = 0; i < numNodes; i++) {
        if (nodes[i].demand < 0) {
            addEdge(s, i, INF);
        }
        else if (nodes[i].demand > 0) {
            addEdge(i, t, nodes[i].demand);
        }
    }
    
    fordFulkerson(s, t);
    
    // Check which edges are at capacity
    for (const Edge& edge : edges) {
        if (edge.from != s && edge.to != t && edge.flow == edge.capacity) {
            criticalEdges.push_back(edge);
        }
    }
    
    // Restore original graph
    adj.resize(numNodes);
    
    return criticalEdges;
}
