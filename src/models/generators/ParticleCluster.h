
#pragma once

#include "models/Particle.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <spdlog/spdlog.h>
#include <vector>

/**
 * @brief An abstract class to model a cluster of particles
 */
class ParticleCluster {
public:
    /**
     * @brief Constructor for the ParticleCluster class
     * @param origin The origin of the cluster
     * @param mass The mass of the particles
     * @param initialVelocity The initial velocity of the particles
     * @param meanVelocity The mean velocity of the particles
     * @param dimensions The dimensions of the cluster (<= 3). Will be passed to
     * maxwellBoltzmannDistributedVelocity() The dimensions of the cluster (<= 3) will be passed to
     * maxwellBoltzmannDistributedVelocity() if set to 0 no brownian motion will be added, if set to
     * 1 then only on the x axis, then x+y, and finally x+y+z axis
     * @param stationaryParticleTypes A map of particle types that are stationary
     * @param ptype The particle type for this cluster
     */
    ParticleCluster(
        std::array<double, 3> origin,
        double mass,
        std::array<double, 3> initialVelocity,
        double meanVelocity,
        size_t dimensions,
        const std::map<unsigned, bool>& stationaryParticleTypes,
        unsigned ptype);

    /**
     * @brief Destructor for the ParticleCluster class
     */
    virtual ~ParticleCluster() = default;

    /**
     * @brief Get the total number of particles in the cluster
     * @return The total number of particles in the cluster
     */
    virtual size_t getTotalNumberOfParticles() const = 0;
    /**
     * @brief Generate the cluster of particles
     * @param particles The vector to insert the particles into
     * @param insertionIndex The index to start inserting the particles at (will be incremented)
     */
    virtual void generateCluster(
        std::vector<Particle>& particles, size_t& insertionIndex) const = 0;
    /**
     * @brief Get a string representation of the cluster
     * @return The string representation of the cluster
     */
    virtual std::string toString() const = 0;

protected:
    std::array<double, 3> origin; /**< The origin of the cluster */
    double mass; /**< The mass of the particles */
    std::array<double, 3> initialVelocity; /**< The initial velocity of the particles */
    double meanVelocity; /**< The mean velocity of the particles */
    size_t dimensions; /**< The dimensions of the cluster (2 or 3) */
    unsigned ptype; /**< The particle type for this cluster */
    bool isNotStationary; /**< Whether the particle type is stationary */
};
