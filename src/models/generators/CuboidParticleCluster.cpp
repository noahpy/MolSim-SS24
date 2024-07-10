
#include "CuboidParticleCluster.h"

CuboidParticleCluster::CuboidParticleCluster(
    std::array<double, 3> origin,
    int numParticlesWidth,
    int numParticlesHeight,
    int numParticlesDepth,
    double spacing,
    double mass,
    std::array<double, 3> initialVelocity,
    double meanVelocity,
    size_t dimensions,
    unsigned ptype)
    : ParticleCluster(origin, mass, initialVelocity, meanVelocity, dimensions, ptype)
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
                Particle particle (position, velocity, mass, ptype);

                // Add particle to container
                particles[insertionIndex++] = particle;
            }
        }
    }

    spdlog::info("Generated CuboidCluster {}", toString());
}

std::string CuboidParticleCluster::toString() const
{
    std::ostringstream oss;
    oss << "pos: (" << origin[0] << ", " << origin[1] << ", " << origin[2] << ") ";
    oss << "vel: (" << initialVelocity[0] << ", " << initialVelocity[1] << ", "
        << initialVelocity[2] << ") ";
    oss << "dim: (" << numParticlesWidth << ", " << numParticlesHeight << ", " << numParticlesDepth
        << ") ";
    oss << "mass: " << mass << " ";
    oss << "ptype: " << ptype << " ";
    oss << "meanVel: " << meanVelocity << " ";
    oss << "brownDim: " << dimensions << " ";
    oss << "spacing: " << spacing << " ";
    return oss.str();
}
