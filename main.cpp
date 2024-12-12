#include "network.hpp"
#include <iostream>
#include <algorithm>

int main() {
    Network network;
    network.readInput();
    
    // Calculate and print the total energy capacity
    std::cout << network.getTotalEnergyCapacity() << std::endl;
    
    // Calculate and print the unmet energy
    std::cout << network.getUnmetEnergy() << std::endl;
    
    // Calculate and print the lost energy
    std::cout << network.getLostEnergy() << std::endl;
    
    // Get and print critical connections
    std::vector<Edge> criticalEdges = network.getCriticalConnections();
    std::cout << criticalEdges.size() << std::endl;
    // ordene criticalEdges pela capacidade
    std::sort(criticalEdges.begin(), criticalEdges.end(), [](const Edge& a, const Edge& b) {
        return a.capacity > b.capacity;
    });
    for (const Edge& edge : criticalEdges) {
        std::cout << (edge.from + 1) << " " << (edge.to + 1) << " " << edge.capacity << std::endl;
    }
    
    return 0;
}