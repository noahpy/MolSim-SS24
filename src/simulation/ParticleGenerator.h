
#include "models/ParticleContainer.h"

/**
 * @brief Class to generate particles for the simulation
 */
class ParticleGenerator {
public:
    /**
     * @brief Constructor for the ParticleGenerator class
     * @param container The container to store the particles in
     * @return A ParticleGenerator object
     */
    explicit ParticleGenerator(ParticleContainer& container);

    /**
     * @brief Generate a cuboid of particles
     * @details Generates a cuboid of particles with the given parameters. The particles will be
     * placed in the container provided in the constructor. The cuboid will contain particles of
     * equal mass and initial velocity. Each particle will have a random velocity based on the
     * meanVelocity and Maxwell-Boltzmann distribution as described in the
     * maxwellBoltzmannDistributedVelocity() function.
     * @param origin The coordinate of the lower left front-side corner
     * @param numParticlesWidth Number of particles in the width dimension
     * @param numParticlesHeight Number of particles in the height dimension
     * @param numParticlesDepth Number of particles in the depth dimension
     * @param spacing The spacing between the particles
     * @param mass The mass of the particles
     * @param initialVelocity The initial velocity of the particles
     * @param meanVelocity The mean velocity of the particles. Will be used in combination with
     * Maxwell-Boltzmann distribution to generate brownian motion
     * @param dimensions The number of dimensions for which the velocity vector shall be generated.
     * Set this to 2 or 3. Will be passed to maxwellBoltzmannDistributedVelocity()
     *
     * @return void
     */
    void generateCuboid(
        std::array<double, 3> origin,
        int numParticlesWidth,
        int numParticlesHeight,
        int numParticlesDepth,
        double spacing,
        double mass,
        std::array<double, 3> initialVelocity,
        double meanVelocity,
        size_t dimensions);

private:
    ParticleContainer& container; /**< The container to store the particles in */
};
