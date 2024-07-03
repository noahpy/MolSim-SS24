
#include "MembraneMap.h"

void MembraneMap::addNeighbor(size_t particleIndex, size_t neighborIndex) {
    neighborMap[particleIndex].push_back(neighborIndex);
}

const std::vector<size_t>& MembraneMap::getNeighbors(size_t particleIndex) const {
    static const std::vector<size_t> empty;
    auto it = neighborMap.find(particleIndex);
    if (it != neighborMap.end()) {
        return it->second;
    }
    return empty;
}