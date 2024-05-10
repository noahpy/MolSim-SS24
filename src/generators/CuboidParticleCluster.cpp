
#include "CuboidParticleCluster.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <spdlog/spdlog.h>

CuboidParticleCluster::CuboidParticleCluster(
    std::array<double, 3> origin,
    int numParticlesWidth,
    int numParticlesHeight,
    int numParticlesDepth,
    double spacing,
    double mass,
    std::array<double, 3> initialVelocity,
    double meanVelocity,
    size_t dimensions)
    : origin(origin)
    , mass(mass)
    , initialVelocity(initialVelocity)
    , meanVelocity(meanVelocity)
    , dimensions(dimensions)
    , numParticlesWidth(numParticlesWidth)
    , numParticlesHeight(numParticlesHeight)
    , numParticlesDepth(numParticlesDepth)
    , spacing(spacing)
{
}

size_t CuboidParticleCluster::getTotalNumberOfParticles() const
{
    return numParticlesWidth * numParticlesHeight * numParticlesDepth;
}

void CuboidParticleCluster::generateCluster(
    std::vector<Particle>& particles, size_t& insertionIndex) const
{
    if (dimensions != 2 && dimensions != 3) {
        spdlog::warn(
            "The dimensions specified for a Cuboid Particle cluster was set to {}. It can only be "
            "of values 2 or 3",
            dimensions);
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
                particles[insertionIndex++] = particle;
            }
        }
    }

    spdlog::info("Generated CuboidCluster {}", toString());
}

std::string CuboidParticleCluster::toString() const
{
    std::string str = "origin: " + std::to_string(origin[0]) + ", " + std::to_string(origin[1]) +
                      ", " + std::to_string(origin[2]) + "; ";
    str += "mass: " + std::to_string(mass) + "; ";
    str += "initialVelocity: " + std::to_string(initialVelocity[0]) + ", " +
           std::to_string(initialVelocity[1]) + ", " + std::to_string(initialVelocity[2]) + "; ";
    str += "meanVelocity: " + std::to_string(meanVelocity) + "; ";
    str += "dimensions: " + std::to_string(dimensions) + "; ";
    str += "numParticlesWidth: " + std::to_string(numParticlesWidth) + "; ";
    str += "numParticlesHeight: " + std::to_string(numParticlesHeight) + "; ";
    str += "numParticlesDepth: " + std::to_string(numParticlesDepth) + "; ";
    str += "spacing: " + std::to_string(spacing) + "; ";
    return str;
}
