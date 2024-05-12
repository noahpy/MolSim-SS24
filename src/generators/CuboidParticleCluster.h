
#include "generators/ParticleCluster.h"

/**
 * @brief A class to model a cluster of particles in the shape of a cuboid, see ParticleCluster
 */
class CuboidParticleCluster : public ParticleCluster {
public:
    /**
     * @brief Constructor for the CuboidParticleCluster class
     * @param origin The origin of the cluster, will be the bottom left corner of the cuboid
     * @param numParticlesWidth The number of particles in the width of the cuboid (x)
     * @param numParticlesHeight The number of particles in the height of the cuboid (y)
     * @param numParticlesDepth The number of particles in the depth of the cuboid (z)
     * @param spacing The spacing between the particles
     * @param mass The mass of the particles
     * @param initialVelocity The initial velocity of the particles
     * @param meanVelocity The mean velocity of the particles
     * @param dimensions The dimensions of the cluster (2 or 3) will be passed to
     * maxwellBoltzmannDistributedVelocity()
     */
    CuboidParticleCluster(
        std::array<double, 3> origin,
        int numParticlesWidth,
        int numParticlesHeight,
        int numParticlesDepth,
        double spacing,
        double mass,
        std::array<double, 3> initialVelocity,
        double meanVelocity,
        size_t dimensions);

    /**
     * @brief Get the total number of particles in the cluster
     * @return The total number of particles in the cluster i.e. numParticlesWidth *
     * numParticlesHeight * numParticlesDepth
     */
    size_t getTotalNumberOfParticles() const override;
    /**
     * @brief Generate the cluster of particles
     * @param particles The vector to insert the particles into
     * @param insertionIndex The index to start inserting the particles at (will be incremented)
     * @return void
     */
    void generateCluster(std::vector<Particle>& particles, size_t& insertionIndex) const override;
    /**
     * @brief Get a string representation of the cluster
     * @return A string representation of the cluster
     */
    std::string toString() const override;

protected:
    int numParticlesWidth; /**< The number of particles in the width of the cuboid (x) */
    int numParticlesHeight; /**< The number of particles in the height of the cuboid (y) */
    int numParticlesDepth; /**< The number of particles in the depth of the cuboid (z) */
    double spacing; /**< The spacing between the particles */
};
