
#include "Membrane.h"

Membrane::Membrane(std::array<double, 3> origin, int numParticlesWidth, int numParticlesHeight)
    : origin(origin), numParticlesWidth(numParticlesWidth), numParticlesHeight(numParticlesHeight) {}

void Membrane::addNeighbor(int particleId, int neighborId) {
    membraneMap[particleId].emplace_back(neighborId);
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
    std::vector<std::vector<int>> grid(numParticlesWidth, std::vector<int>(numParticlesHeight, -1));
    int membraneId = 0;
    double gridSpaceX = (numParticlesWidth - 1) * spacing;
    double gridSpaceY = (numParticlesHeight - 1) * spacing;

    // Iterate through all particles to find those that belong to the membrane
    for (auto& p : container.particles) {

        // Check if the particle is within the bounds of the membrane
        double x = p.getX()[0];
        double y = p.getX()[1];
        if (x >= origin[0] && x <= origin[0] + gridSpaceX && y >= origin[1] && y <= origin[1] + gridSpaceY) {
            // Calculate the grid position of the particle
            int i = static_cast<int>((x - origin[0]) / spacing);
            int j = static_cast<int>((y - origin[1]) / spacing);

            // Set the molecular attribute and store the particle reference in the grid
            p.setMembraneId(membraneId);
            grid[i][j] = membraneId;
            ++membraneId;
        }
    }

    const std::vector<std::pair<int, int>> offsets = {
        {1, 0}, {1, 1}, {0, 1}, {-1, 1},
        {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
    };

    // Populate the membraneMap with neighbor relationships
    for (int i = 0; i < numParticlesWidth; i++) {
        for (int j = 0; j < numParticlesHeight; j++) {
            int currentId = grid[i][j];

            if (currentId != -1) {

                for (const auto& offset : offsets) {
                    int ni = i + offset.first;
                    int nj = j + offset.second;

                    if (ni >= 0 && ni < numParticlesWidth && nj >= 0 && nj < numParticlesHeight && grid[ni][nj] != -1) {
                        addNeighbor(currentId, grid[ni][nj]);
                    } else {
                        addNeighbor(currentId, -1);
                    }
                }
            }
        }
    }
}

bool Membrane::isNeighbor(Particle& particle1, Particle& particle2) const {
    auto it = membraneMap.find(particle1.getMembraneId());
    if (it != membraneMap.end()) {
        const auto& neighbors = it->second;
        return std::find_if(neighbors.begin(), neighbors.end(), [&particle2](const size_t neighbor) {
            return neighbor == particle2.getMembraneId();
        }) != neighbors.end();
    }
    return false;
}

bool Membrane::isDiagonalNeighbor(Particle& particle1, Particle& particle2) const {
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
        // Neighbors for calculation are at positions: 7 (Bottom-Right), 0 (Right), 1 (Top-Right), 2 (Top)
        static const std::vector<int> diagonalPositions = {7, 0, 1, 2};

        for (int pos : diagonalPositions) {
            if (pos < neighbors.size() && neighbors[pos] == particle2.getMembraneId()) {
                return true;
            }
        }
    }
    return false;
}
