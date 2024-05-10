
#include "ParticleGenerator.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"

ParticleGenerator::ParticleGenerator(ParticleContainer& container)
    : container(container)
{
}

void ParticleGenerator::generateCuboid(
    const std::array<double, 3> origin,
    const int numParticlesWidth,
    const int numParticlesHeight,
    const int numParticlesDepth,
    const double spacing,
    const double mass,
    const std::array<double, 3> initialVelocity,
    const double meanVelocity,
    const size_t dimensions)
{
    if (dimensions != 2 && dimensions != 3) {
        throw std::invalid_argument("Dimensions must be 2 or 3.");
    }

    for (int i = 0; i < numParticlesWidth; i++) { // x
        for (int j = 0; j < numParticlesHeight; j++) { // y
            for (int k = 0; k < numParticlesDepth; k++) { // z
                // Calculate position
                std::array<double, 3> position = { origin[0] + i * spacing,
                                                   origin[1] + j * spacing,
                                                   origin[2] + k * spacing };

                // Calculate velocity based on Maxwell-Boltzmann distribution to include brownian
                // motion
                std::array<double, 3> velocity =
                    initialVelocity + maxwellBoltzmannDistributedVelocity(meanVelocity, dimensions);

                // Create particle
                Particle particle = Particle(position, velocity, mass);

                // Add particle to container
                this->container.addParticle(particle);
            }
        }
    }
}
