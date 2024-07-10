
#pragma once
#include <array>
#include <models/ParticleContainer.h>
#include <unordered_map>
#include <vector>
#include <algorithm>

/**
 * @brief A class to model a membrane of particles, managing molecular attributes and neighbors
 */
class Membrane {
public:
    /**
     * @brief Constructor for the Membrane class
     * @param origin The origin of the membrane
     * @param numParticlesWidth The number of particles in the width of the membrane (x)
     * @param numParticlesHeight The number of particles in the height of the membrane (y)
     */
    Membrane(std::array<double, 3> origin, int numParticlesWidth, int numParticlesHeight);

    /**
     * @brief Add a neighbor relationship
     * @param particleId The Id of the particle
     * @param neighborId The Id to the neighboring particle
     */
    void addNeighbor(int particleId, int neighborId);

    /**
     * @brief Get the membrane id's of neighbors of a particle
     * @param particle The reference to the particle
     * @return A vector of references to neighboring particles
     */
    [[nodiscard]] const std::vector<int>& getNeighbors(Particle& particle) const;

    /**
     * @brief Initialize the membrane by setting molecular attributes and filling the neighbor map for particles
     * @param container The container with particles
     */
    void initMembrane(ParticleContainer& container, double spacing);

    /**
     * @brief Checks if particle2 is a neighbor to particle1 in the membrane
     * @param particle1 Particle for which the neighbor is checked
     * @param particle2 Particle for neighbor check
     * @return True if particle2 is a neighbor of particle1, otherwise false
     */
    bool isNeighbor( Particle& particle1, Particle& particle2) const;
    /**
     * @brief Checks if particle2 is a diagonal neighbor to particle1 in the membrane
     * @param particle1 Particle for which the neighbor is checked
     * @param particle2 Particle for neighbor check
     * @return True if particle2 is a diagonal neighbor of particle1, otherwise false
     */
    bool isDiagonalNeighbor( Particle& particle1, Particle& particle2) const;

    /**
     * @brief Checks for half of the neighbors (Bottom-Right, Right, Top-right, Top) to enable iteration
     * inside the force calculation using newton's third law
     * @param particle1 Particle for which the neighbor is checked
     * @param particle2 Particle for neighbor check
     * @return Ture if particle2 is one of the neighbors from the upper half
     */
    bool isCalcNeighbor(Particle& particle1, Particle& particle2) const;

private:
    std::array<double, 3> origin; /**< The origin of the membrane */
    int numParticlesWidth; /**< The number of particles in the width of the membrane (x) */
    int numParticlesHeight; /**< The number of particles in the height of the membrane (y) */
    std::unordered_map<int, std::vector<int>> membraneMap; /**< Map storing neighbor relationships via particle id's*/
};
