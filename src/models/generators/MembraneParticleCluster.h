
#pragma once
#include "ParticleCluster.h"

#include <models/MembraneMap.h>

/**
 * @brief A class to model a cluster of particles in the shape of a membrane, see ParticleCluster
 */
class MembraneParticleCluster : public ParticleCluster {
public:
    /**
     * @brief Constructor for the MembraneParticleCluster class
     * @param origin The origin of the cluster, will be the bottom left corner of the membrane
     * @param numParticlesWidth The number of particles in the width of the membrane (x)
     * @param numParticlesHeight The number of particles in the height of the membrane (y)
     * @param spacing The spacing between the particles
     * @param mass The mass of the particles
     * @param initialVelocity The initial velocity of the particles
     * @param meanVelocity The mean velocity of the particles
     * @param dimensions The dimensions of the cluster (<= 3). Will be passed to
     * maxwellBoltzmannDistributedVelocity()
     */
    MembraneParticleCluster(
        std::array<double, 3> origin,
        int numParticlesWidth,
        int numParticlesHeight,
        double spacing,
        double mass,
        std::array<double, 3> initialVelocity,
        double meanVelocity,
        size_t dimensions);

    /**
     * @brief Get the total number of particles in the cluster
     * @return The total number of particles in the cluster i.e. numParticlesWidth *
     * numParticlesHeight
     */
    size_t getTotalNumberOfParticles() const override;

    /**
     * @brief Generate the cluster of particles
     * @param particles The vector to insert the particles into
     * @param insertionIndex The index to start inserting the particles at (will be incremented)
     * @return void
     */
    void generateMembrane(std::vector<Particle>& particles, size_t& insertionIndex, MembraneMap& membraneMap) const;

    /**
     * @brief Get a string representation of the cluster
     * @return A string representation of the cluster
     */
    std::string toString() const override;

protected:
    int numParticlesWidth; /**< The number of particles in the width of the membrane (x) */
    int numParticlesHeight; /**< The number of particles in the height of the membrane (y) */
    double spacing; /**< The spacing between the particles */
};