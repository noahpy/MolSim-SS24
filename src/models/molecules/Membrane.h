
#pragma once
#include "Molecule.h"
#include <map>

/**
 * @brief A 2D grid of particles (Membranes will be represented as 2D grids of particle ids)
 */
typedef std::vector<std::vector<size_t>> ParticleGrid;

/**
 * @brief A map from particles to their neighbors
 */
typedef std::map<size_t , std::vector<size_t>> NeighborParticleMap;

/**
 * @brief A map that stores if two particles are neighbors
 */
typedef std::map<std::pair<size_t, size_t>, bool> NeighboringRelationMap;

/**
 * @brief A class to model a molecules of particles
 */
class Membrane : public Molecule {
public:
    /**
     * @brief Construct a new Membrane object
     * @param origin The origin of the membrane
     * @param numParticlesWidth The number of particles in width
     * @param numParticlesHeight The number of particles in height
     * @param numParticlesDepth The number of particles in depth
     * @param spacing The spacing between particles
     * @param mass The mass of the particles
     * @param initialVelocity The initial velocity of the particles
     * @param meanVelocity The mean velocity of the particles
     * @param dimensions The dimensions of the particles
     * @param ptype The particle type
     * @param r0 The equilibrium distance
     * @param k The spring constant
     */
    Membrane(
        std::array<double, 3> origin,
        int numParticlesWidth,
        int numParticlesHeight,
        int numParticlesDepth,
        double spacing,
        double mass,
        std::array<double, 3> initialVelocity,
        double meanVelocity,
        size_t dimensions,
        unsigned ptype,
        double r0,
        double k);

    /**
     * @brief Generate the membrane
     * @param container The container to generate the membrane in
     * @param moleculeID The molecule ID (unique identifier for the molecule)
     */
    void generateMolecule(ParticleContainer& container, size_t moleculeID) override;

    /**
     * @brief Calculate the membrane specific intra-molecular forces using the harmonic potential
     * @param sim The simulation to calculate the forces for
     */
    void calculateIntraMolecularForces(const Simulation& sim) override;

    /**
     * @brief Get the string representation of the membrane
     * @return The string representation of the membrane
     */
    std::string toString();

    /**
     * @brief Get the direct neighbors of the particles
     * @return The direct neighbors of the particles
     */
    [[nodiscard]] const NeighborParticleMap& getDirectNeighbors() const { return directNeighbors; }

    /**
     * @brief Get the diagonal neighbors of the particles
     * @return The diagonal neighbors of the particles
     */
    [[nodiscard]] const NeighborParticleMap& getDiagNeighbors() const { return diagNeighbors; }

protected:
    /**
     * Idea: Only store the neighbors TOP, TOP-RIGHT, RIGHT, BOTTOM-RIGHT
     * This way, we can simply calculate the harmonic potential for those neighbors
     * Then use newton's third law -> this way we only loop over all particles but get all forces
     */
    NeighborParticleMap directNeighbors; /**< The direct neighbors of the particles (only the ones that are required to calculate the forces) */
    NeighborParticleMap diagNeighbors; /**< The vertical neighbors of the particles (only the ones that are required to calculate the forces) */

    NeighboringRelationMap neighboringRelations; /**< The neighboring relations of the particles */

    std::array<double, 3> origin; /**< The origin of the membrane */
    int numParticlesWidth; /**< The number of particles in width */
    int numParticlesHeight; /**< The number of particles in height */
    int numParticlesDepth; /**< The number of particles in depth */
    double spacing; /**< The spacing between particles */
    double mass; /**< The mass of the particles */
    std::array<double, 3> initialVelocity; /**< The initial velocity of the particles */
    double meanVelocity; /**< The mean velocity of the particles */
    size_t dimensions; /**< The dimensions of the particles */
    double r0; /**< The equilibrium distance */
    double diagR0; /**< The equilibrium distance for diagonal neighbors */
    double k; /**< The spring constant */

    /**
     * @brief Initialize the neighbors of the particles after the gird has been generated
     * @param particleGrid The particle grid of the membrane (flattend to 2D grid)
     */
    void initNeighbors(const ParticleGrid& particleGrid);

    /**
     * @brief Calculate the harmonic forces for all particles of the membrane
     * @param container The container of the particles
     */
    void calculateHarmonicForces(ParticleContainer& container);

    /**
     * @brief Check if two particles are neighbors
     * @param particleID1 The ID of the first particle
     * @param particleID2 The ID of the second particle
     * @return True <=> The particles are neighbors
     */
    bool isNeighbor(size_t particleID1, size_t particleID2);

    /**
     * @brief Get the key for the neighboring relations map
     * @param particleID1 The ID of the first particle
     * @param particleID2 The ID of the second particle
     * @return The key for the neighboring relations map
     */
    inline std::pair<size_t, size_t> getKey(size_t particleID1, size_t particleID2) {
        return particleID1 < particleID2 ? std::make_pair(particleID1, particleID2) : std::make_pair(particleID2, particleID1);
    }
};
