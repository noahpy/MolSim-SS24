
#include "Membrane.h"

Membrane::Membrane(std::array<double, 3> origin, int numParticlesWidth, int numParticlesHeight)
    : origin(origin), numParticlesWidth(numParticlesWidth), numParticlesHeight(numParticlesHeight), membraneMap() {}

void Membrane::addNeighbor(Particle& particle, Particle& neighbor) {
    membraneMap[std::ref(particle)].emplace_back(std::ref(neighbor));
}

const std::vector<std::reference_wrapper<Particle>>& Membrane::getNeighbors(Particle& particle) const {
    static const std::vector<std::reference_wrapper<Particle>> empty;
    auto it = membraneMap.find(std::ref(particle));
    if (it != membraneMap.end()) {
        return it->second;
    }
    return empty;
}

void Membrane::initMembrane(ParticleContainer& container) {
    // Create a 2D grid of particle references
    std::vector<std::vector<Particle*>> grid(numParticlesWidth, std::vector<Particle*>(numParticlesHeight, nullptr));

    // Iterate through all particles to find those that belong to the membrane
    for (size_t id = 0; id < container.particles.size(); ++id) {
        Particle& particle = container.particles[id];

        // Check if the particle is within the bounds of the membrane
        double x = particle.getX()[0];
        double y = particle.getX()[1];
        if (x >= origin[0] && x < origin[0] + numParticlesWidth && y >= origin[1] && y < origin[1] + numParticlesHeight) {
            // Calculate the grid position of the particle
            int i = static_cast<int>((x - origin[0]) / (container.particles[1].getX()[0] - container.particles[0].getX()[0]));
            int j = static_cast<int>((y - origin[1]) / (container.particles[numParticlesWidth].getX()[1] - container.particles[0].getX()[1]));

            // Set the molecular attribute and store the particle reference in the grid
            particle.setMolecular(true);
            grid[i][j] = &particle;
        }
    }

    // Populate the membraneMap with neighbor relationships
    for (int i = 0; i < numParticlesWidth; i++) {
        for (int j = 0; j < numParticlesHeight; j++) {
            Particle* currentParticle = grid[i][j];

            if (currentParticle) {
                if (i < numParticlesWidth - 1 && grid[i + 1][j]) addNeighbor(*currentParticle, *grid[i + 1][j]); // Right neighbor
                if (i < numParticlesWidth - 1 && j < numParticlesHeight - 1 && grid[i + 1][j + 1]) addNeighbor(*currentParticle, *grid[i + 1][j + 1]); // Top-Right neighbor
                if (j < numParticlesHeight - 1 && grid[i][j + 1]) addNeighbor(*currentParticle, *grid[i][j + 1]); // Top neighbor
                if (i > 0 && j < numParticlesHeight - 1 && grid[i - 1][j + 1]) addNeighbor(*currentParticle, *grid[i - 1][j + 1]); // Top-Left neighbor
                if (i > 0 && grid[i - 1][j]) addNeighbor(*currentParticle, *grid[i - 1][j]); // Left neighbor
                if (i > 0 && j > 0 && grid[i - 1][j - 1]) addNeighbor(*currentParticle, *grid[i - 1][j - 1]); // Bottom-Left neighbor
                if (j > 0 && grid[i][j - 1]) addNeighbor(*currentParticle, *grid[i][j - 1]); // Bottom neighbor
                if (i < numParticlesWidth - 1 && j > 0 && grid[i + 1][j - 1]) addNeighbor(*currentParticle, *grid[i + 1][j - 1]); // Bottom-Right neighbor
            }
        }
    }
}

bool Membrane::isNeighbor( Particle& particle1, Particle& particle2) const {
    auto it = membraneMap.find(std::ref(particle1));
    if (it != membraneMap.end()) {
        const auto& neighbors = it->second;
        return std::find_if(neighbors.begin(), neighbors.end(), [&particle2](const std::reference_wrapper<Particle>& neighbor) {
            return &neighbor.get() == &particle2;
        }) != neighbors.end();
    }
    return false;
}

bool Membrane::isDiagonalNeighbor( Particle& particle1, Particle& particle2) const {
    auto it = membraneMap.find(std::ref(particle1));
    if (it != membraneMap.end()) {
        const auto& neighbors = it->second;
        // The neighbors are stored in the order:
        // Right, Top-Right, Top, Top-Left, Left, Bottom-Left, Bottom, Bottom-Right
        // Diagonal neighbors are at positions: 1 (Top-Right), 3 (Top-Left), 5 (Bottom-Left), 7 (Bottom-Right)
        static const std::vector<int> diagonalPositions = {1, 3, 5, 7};

        for (int pos : diagonalPositions) {
            if (pos < neighbors.size() && &neighbors[pos].get() == &particle2) {
                return true;
            }
        }
    }
    return false;
}

bool Membrane::isCalcNeighbor(Particle& particle1, Particle& particle2) const
{
    auto it = membraneMap.find(std::ref(particle1));
    if (it != membraneMap.end()) {
        const auto& neighbors = it->second;
        // The neighbors are stored in the order:
        // Right, Top-Right, Top, Top-Left, Left, Bottom-Left, Bottom, Bottom-Right
        // Neighbors for calculation are at positions: 0 (Right), 1 (Top-Right), 2 (Top), 3 (Top-Left)
        static const std::vector<int> diagonalPositions = {0, 1, 2, 3};

        for (int pos : diagonalPositions) {
            if (pos < neighbors.size() && &neighbors[pos].get() == &particle2) {
                return true;
            }
        }
    }
    return false;
}
