
#pragma once
#include <vector>
#include <unordered_map>

/**
 * @brief A class to store the neighbors of each particle in a membrane
 */
class MembraneMap {
public:
    /**
     * @brief Add a neighbor relationship
     * @param particleIndex The index of the particle
     * @param neighborIndex The index of the neighboring particle
     */
    void addNeighbor(size_t particleIndex, size_t neighborIndex);

    /**
     * @brief Get the neighbors of a particle
     * @param particleIndex The index of the particle
     * @return A vector of indices of neighboring particles
     */
    const std::vector<size_t>& getNeighbors(size_t particleIndex) const;

private:
    /**
     * @brief Map to store neighboring particles in a membrane
     */
    std::unordered_map<size_t, std::vector<size_t>> neighborMap;
};