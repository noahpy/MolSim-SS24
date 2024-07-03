
#include "MembraneParticleCluster.h"

MembraneParticleCluster::MembraneParticleCluster(
    std::array<double, 3> origin,
    int numParticlesWidth,
    int numParticlesHeight,
    double spacing,
    double mass,
    std::array<double, 3> initialVelocity,
    double meanVelocity,
    size_t dimensions)
    : ParticleCluster(origin, mass, initialVelocity, meanVelocity, dimensions, ptype)
    , numParticlesWidth(numParticlesWidth)
    , numParticlesHeight(numParticlesHeight)
    , spacing(spacing)
{
}

size_t MembraneParticleCluster::getTotalNumberOfParticles() const
{
    return numParticlesWidth * numParticlesHeight;
}

void MembraneParticleCluster::generateMembrane(
    std::vector<Particle>& particles, size_t& insertionIndex, MembraneMap& membraneMap) const
{
    std::vector<std::vector<size_t>> grid(numParticlesWidth, std::vector<size_t>(numParticlesHeight));

    for (int i = 0; i < numParticlesWidth; i++) { // x
        for (int j = 0; j < numParticlesHeight; j++) { // y
            // Calculate position
            std::array<double, 3> position = { origin[0] + i * spacing,
                                               origin[1] + j * spacing,
                                               origin[2] };

            // Calculate velocity based on Maxwell-Boltzmann distribution to include brownian motion
            std::array<double, 3> velocity =
                initialVelocity + maxwellBoltzmannDistributedVelocity(meanVelocity, dimensions);

            // Create particle with molecular attribute set to true
            Particle particle = Particle(position, velocity, mass);
            particle.setMolecular(true);

            // Add particle to container
            particles[insertionIndex] = particle;
            grid[i][j] = insertionIndex;

            insertionIndex++;
        }
    }

    // Now populate the membraneMap for each particle
    for (int i = 0; i < numParticlesWidth; i++) {
        for (int j = 0; j < numParticlesHeight; j++) {
            size_t currentParticleIndex = grid[i][j];

            if (i < numParticlesWidth - 1) membraneMap.addNeighbor(currentParticleIndex, grid[i + 1][j]); // Right neighbor
            if (i < numParticlesWidth - 1 && j < numParticlesHeight - 1) membraneMap.addNeighbor(currentParticleIndex, grid[i + 1][j + 1]); // Right-Top neighbor
            if (j < numParticlesHeight - 1) membraneMap.addNeighbor(currentParticleIndex, grid[i][j + 1]); // Top neighbor
            if (i > 0 && j < numParticlesHeight - 1) membraneMap.addNeighbor(currentParticleIndex, grid[i - 1][j + 1]); // Top-left neighbor
        }
    }

    spdlog::info("Generated MembraneParticleCluster {}", toString());
}

std::string MembraneParticleCluster::toString() const
{
    std::ostringstream oss;
    oss << "origin: " << origin[0] << ", " << origin[1] << ", " << origin[2] << "; ";
    oss << "mass: " << mass << "; ";
    oss << "initialVelocity: " << initialVelocity[0] << ", " << initialVelocity[1] << ", "
        << initialVelocity[2] << "; ";
    oss << "meanVelocity: " << meanVelocity << "; ";
    oss << "dimensions: " << dimensions << "; ";
    oss << "numParticlesWidth: " << numParticlesWidth << "; ";
    oss << "numParticlesHeight: " << numParticlesHeight << "; ";
    oss << "spacing: " << spacing << "; ";
    return oss.str();
}