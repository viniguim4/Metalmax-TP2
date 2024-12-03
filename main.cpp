#include "network.hpp"
#include <iostream>

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
    for (const Edge& edge : criticalEdges) {
        std::cout << (edge.from + 1) << " " << (edge.to + 1) << " " << edge.capacity << std::endl;
    }
    
    return 0;
}
