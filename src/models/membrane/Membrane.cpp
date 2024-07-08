
#include "Membrane.h"

Membrane::Membrane(std::array<double, 3> origin, int numParticlesWidth, int numParticlesHeight)
    : origin(origin), numParticlesWidth(numParticlesWidth), numParticlesHeight(numParticlesHeight) {}

void Membrane::addNeighbor(Particle& particle, int neighborId) {
    membraneMap[particle.getMembraneId()].emplace_back(neighborId);
}

const std::vector<int>& Membrane::getNeighbors(Particle& particle) const {
    static const std::vector<int> empty;
    auto it = membraneMap.find(particle.getMembraneId());
    if (it != membraneMap.end()) {
        return it->second;
    }
    return empty;
}

void Membrane::initMembrane(ParticleContainer& container, double spacing) {
    // Create a 2D grid of particle references
    std::vector<std::vector<Particle*>> grid(numParticlesWidth, std::vector<Particle*>(numParticlesHeight, nullptr));
    int membraneId = 0;

    // Iterate through all particles to find those that belong to the membrane
    for (auto& p : container.particles) {

        // Check if the particle is within the bounds of the membrane
        double x = p.getX()[0];
        double y = p.getX()[1];
        if (x >= origin[0] && x < origin[0] + numParticlesWidth && y >= origin[1] && y < origin[1] + numParticlesHeight) {
            // Calculate the grid position of the particle
            int i = static_cast<int>((x - origin[0]) / spacing);
            int j = static_cast<int>((y - origin[1]) / spacing);

            // Set the molecular attribute and store the particle reference in the grid
            p.setMembraneId(membraneId);
            grid[i][j] = &p;
            ++membraneId;
        }
    }

    // Populate the membraneMap with neighbor relationships
    for (int i = 0; i < numParticlesWidth; i++) {
        for (int j = 0; j < numParticlesHeight; j++) {
            Particle* currentParticle = grid[i][j];

            if (currentParticle) {
                if (i < numParticlesWidth - 1 && grid[i + 1][j]) addNeighbor(*currentParticle, grid[i + 1][j]->getMembraneId()); // Right neighbor
                else addNeighbor(*currentParticle, -1);
                if (i < numParticlesWidth - 1 && j < numParticlesHeight - 1 && grid[i + 1][j + 1]) addNeighbor(*currentParticle, grid[i + 1][j + 1]->getMembraneId()); // Top-Right neighbor
                else addNeighbor(*currentParticle, -1);
                if (j < numParticlesHeight - 1 && grid[i][j + 1]) addNeighbor(*currentParticle, grid[i][j + 1]->getMembraneId()); // Top neighbor
                else addNeighbor(*currentParticle, -1);
                if (i > 0 && j < numParticlesHeight - 1 && grid[i - 1][j + 1]) addNeighbor(*currentParticle, grid[i - 1][j + 1]->getMembraneId()); // Top-Left neighbor
                else addNeighbor(*currentParticle, -1);
                if (i > 0 && grid[i - 1][j]) addNeighbor(*currentParticle, grid[i - 1][j]->getMembraneId()); // Left neighbor
                else addNeighbor(*currentParticle, -1);
                if (i > 0 && j > 0 && grid[i - 1][j - 1]) addNeighbor(*currentParticle, grid[i - 1][j - 1]->getMembraneId()); // Bottom-Left neighbor
                else addNeighbor(*currentParticle, -1);
                if (j > 0 && grid[i][j - 1]) addNeighbor(*currentParticle, grid[i][j - 1]->getMembraneId()); // Bottom neighbor
                else addNeighbor(*currentParticle, -1);
                if (i < numParticlesWidth - 1 && j > 0 && grid[i + 1][j - 1]) addNeighbor(*currentParticle, grid[i + 1][j - 1]->getMembraneId()); // Bottom-Right neighbor
                else addNeighbor(*currentParticle, -1);
            }
        }
    }
}

bool Membrane::isNeighbor( Particle& particle1, Particle& particle2) const {
    auto it = membraneMap.find(particle1.getMembraneId());
    if (it != membraneMap.end()) {
        const auto& neighbors = it->second;
        return std::find_if(neighbors.begin(), neighbors.end(), [&particle2](const size_t neighbor) {
            return neighbor == particle2.getMembraneId();
        }) != neighbors.end();
    }
    return false;
}

bool Membrane::isDiagonalNeighbor( Particle& particle1, Particle& particle2) const {
    auto it = membraneMap.find(particle1.getMembraneId());
    if (it != membraneMap.end()) {
        const auto& neighbors = it->second;
        // The neighbors are stored in the order:
        // Right, Top-Right, Top, Top-Left, Left, Bottom-Left, Bottom, Bottom-Right
        // Diagonal neighbors are at positions: 1 (Top-Right), 3 (Top-Left), 5 (Bottom-Left), 7 (Bottom-Right)
        static const std::vector<int> diagonalPositions = {1, 3, 5, 7};

        for (int pos : diagonalPositions) {
            if (pos < neighbors.size() && neighbors[pos] == particle2.getMembraneId()) {
                return true;
            }
        }
    }
    return false;
}

bool Membrane::isCalcNeighbor(Particle& particle1, Particle& particle2) const
{
    auto it = membraneMap.find(particle1.getMembraneId());
    if (it != membraneMap.end()) {
        const auto& neighbors = it->second;
        // The neighbors are stored in the order:
        // Right, Top-Right, Top, Top-Left, Left, Bottom-Left, Bottom, Bottom-Right
        // Neighbors for calculation are at positions: 0 (Right), 1 (Top-Right), 2 (Top), 3 (Top-Left)
        static const std::vector<int> diagonalPositions = {0, 1, 2, 3};

        for (int pos : diagonalPositions) {
            if (pos < neighbors.size() && neighbors[pos] == particle2.getMembraneId()) {
                return true;
            }
        }
    }
    return false;
}
