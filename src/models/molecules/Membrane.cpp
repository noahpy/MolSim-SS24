
#include "Membrane.h"
#include "physics/forceCal/forceCal.h"
#include "simulation/MembraneSimulation.h"
#include "spdlog/spdlog.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <sstream>

Membrane::Membrane(
    std::array<double, 3> origin,
    int numParticlesWidth,
    int numParticlesHeight,
    int numParticlesDepth,
    double spacing,
    double mass,
    std::array<double, 3> initialVelocity,
    double meanVelocity,
    size_t dimensions,
    unsigned ptype,
    double r0,
    double k)
    : Molecule(ptype)
    , origin(origin)
    , numParticlesWidth(numParticlesWidth)
    , numParticlesHeight(numParticlesHeight)
    , numParticlesDepth(numParticlesDepth)
    , spacing(spacing)
    , mass(mass)
    , initialVelocity(initialVelocity)
    , meanVelocity(meanVelocity)
    , dimensions(dimensions)
    , r0(r0)
    , diagR0(sqrt(2) * r0)
    , k(k)
{
}

void Membrane::generateMolecule(ParticleContainer& container, size_t moleculeID)
{
    if (numParticlesDepth != 1 && numParticlesHeight != 1 && numParticlesWidth != 1) {
        spdlog::error(
            "Membrane can only be generated in 2D. Dimensions: x: {}, y: {}, z: {}",
            "At least one of the dimensions must be 1.",
            numParticlesWidth,
            numParticlesHeight,
            numParticlesDepth);
        exit(EXIT_FAILURE);
    }

    if (moleculeID == 0) {
        spdlog::error("Molecule ID cannot be 0 -> 0 represents no molecule");
        exit(EXIT_FAILURE);
    }

    this->ID = moleculeID;

    size_t firstRelevantDimension = numParticlesWidth == 1 ? 1 : 0;
    size_t secondRelevantDimension = firstRelevantDimension == 1 ? 2 : 1;
    std::array<int, 3> numParticles = { numParticlesWidth, numParticlesHeight, numParticlesDepth };

    ParticleGrid particleGrid(
        numParticles[firstRelevantDimension],
        std::vector<size_t>(numParticles[secondRelevantDimension], 0));

    size_t index = container.particles.size();
    container.particles.resize(
        container.particles.size() + numParticlesWidth * numParticlesHeight * numParticlesDepth);

    // Same as in CuboidParticleCluster but add neighbors
    for (int i = 0; i < numParticlesWidth; i++) { // x
        for (int j = 0; j < numParticlesHeight; j++) { // y
            for (int l = 0; l < numParticlesDepth; l++) { // z
                std::array<int, 3> indices = { i, j, l };

                // Calculate position
                std::array<double, 3> position = { origin[0] + i * spacing,
                                                   origin[1] + j * spacing,
                                                   origin[2] + l * spacing };

                // Calculate velocity based on Maxwell-Boltzmann distribution to include brownian
                // motion
                std::array<double, 3> velocity =
                    initialVelocity + maxwellBoltzmannDistributedVelocity(meanVelocity, dimensions);

                // Create particle
                Particle particle(
                    position, velocity, mass, static_cast<int>(ptype), index, true, moleculeID);

                // Add particle to container
                container.particles[index++] = particle;
                container.activeParticleCount++;

                // Add particle to grid
                particleGrid[indices[firstRelevantDimension]][indices[secondRelevantDimension]] =
                    particle.getID();
            }
        }
    }

    // Set neighbors
    initNeighbors(particleGrid);

    spdlog::info("Generated Membrane: {}", toString());
}

void Membrane::initNeighbors(const ParticleGrid& particleGrid)
{
    /**
     * Idea:
     * We only store half of the neighbors
     * This will allow us to use Netwon's 3rd law
     * By using a fix set of neighbors we avoid doubly calculating forces
     * Essentially we create a directed, non-cyclic graph with a single root
     * From this root, we can calculate all intra molecular forces of the membrane
     * Also, we know that the particle ID is its index in the particle vector
     */
    std::vector<std::array<int, 2>> offsetKernelsDirect = {
        { 0, 1 }, // Top
        { 1, 0 } // Right
    };
    std::vector<std::array<int, 2>> offsetKernelsDiag = {
        { 1, 1 }, // Top-Right
        { 1, -1 } // Bottom-Right
    };
    std::vector<std::array<int, 2>> otherNeighbors = {
        { 0, -1 }, // Bottom
        { -1, 0 }, // Left
        { -1, -1 }, // Bottom-Left
        { -1, 1 } // Top-Left
    };

    for (int x = 0; x < particleGrid.size(); x++) { // x
        for (int y = 0; y < particleGrid[0].size(); y++) { // z
            directNeighbors[particleGrid[x][y]] = {};
            diagNeighbors[particleGrid[x][y]] = {};

            for (auto& offset : offsetKernelsDirect) {
                int i = offset[0] + x;
                int j = offset[1] + y;
                if (i >= 0 && i < particleGrid.size() && j >= 0 && j < particleGrid[0].size()) {
                    directNeighbors[particleGrid[x][y]].push_back(particleGrid[i][j]);
                    neighboringRelations.insert({getKey(particleGrid[x][y], particleGrid[i][j]), true});
                }
            }

            for (auto& offset : offsetKernelsDiag) {
                int i = offset[0] + x;
                int j = offset[1] + y;
                if (i >= 0 && i < particleGrid.size() && j >= 0 && j < particleGrid[0].size()) {
                    diagNeighbors[particleGrid[x][y]].push_back(particleGrid[i][j]);
                    neighboringRelations.insert({getKey(particleGrid[x][y], particleGrid[i][j]), true});
                }
            }

            for (auto& offset : otherNeighbors) {
                int i = offset[0] + x;
                int j = offset[1] + y;
                if (i >= 0 && i < particleGrid.size() && j >= 0 && j < particleGrid[0].size())
                    neighboringRelations.insert({getKey(particleGrid[x][y], particleGrid[i][j]), true});
            }
        }
    }
}

void Membrane::calculateIntraMolecularForces(const Simulation& sim)
{
    const MembraneSimulation& mem_sim = static_cast<const MembraneSimulation&>(sim);
    const CellGrid& cellGrid = mem_sim.getGrid();

    // Calculate harmonic forces
    calculateHarmonicForces(mem_sim.container);

    // Calculate repulsive-LJ forces for the membrane particles to avoid self-penetration
    // See forceCal.h for the same structure
    for (size_t x = 1; x < cellGrid.cells.size() - 1; ++x)
        for (size_t y = 1; y < cellGrid.cells[0].size() - 1; ++y) {
            bool doLoopFor2D = cellGrid.cells[0][0].size() == 1;
            for (size_t z = 1; z < cellGrid.cells[0][0].size() - 1 || doLoopFor2D; ++z) {
                if (doLoopFor2D) {
                    doLoopFor2D = false; // only do it once
                    z = 0;
                }
                std::list<CellIndex> neighbors = cellGrid.getNeighbourCells({ x, y, z });

                // calculate the LJ forces in the cell
                for (auto it = cellGrid.cells.at(x).at(y).at(z)->beginPairs();
                     it != cellGrid.cells.at(x).at(y).at(z)->endPairs();
                     ++it) {
                    auto pair = *it;
                    // Skip iteration if not both particles are part of this membrane
                    if (pair.first.get().getMoleculeId() != ID ||
                        pair.first.get().getMoleculeId() != pair.second.get().getMoleculeId())
                        continue;

                    // Skip iteration if particles are neighbors -> no repulsion
                    if (isNeighbor(pair.first.get().getID(), pair.second.get().getID()))
                        continue;

                    std::array<double, 3> delta =
                        pair.first.get().getX() - pair.second.get().getX();
                    // Check if the distance is less than the cutoff to only have repulsive forces
                    if (ArrayUtils::DotProduct(delta) < cutoffRadiusSquared) {
                        lj_calc(pair.first, pair.second, alpha, beta, gamma, delta);
                    }
                }

                for (auto i : neighbors)
                    for (auto p1 : cellGrid.cells.at(x).at(y).at(z)->getParticles())
                        for (auto p2 : cellGrid.cells[i[0]][i[1]][i[2]]->getParticles()) {
                            // Skip iteration if not both particles are part of this membrane
                            if (p1.get().getMoleculeId() != ID ||
                                p1.get().getMoleculeId() != p2.get().getMoleculeId())
                                continue;

                            // Skip iteration if particles are neighbors -> no repulsion
                            if (isNeighbor(p1.get().getID(), p2.get().getID()))
                                continue;

                            // Check if the distance is less than the cutoff to only have repulsive
                            // forces
                            std::array<double, 3> delta = p1.get().getX() - p2.get().getX();
                            if (ArrayUtils::DotProduct(delta) < cutoffRadiusSquared) {
                                lj_calc(p1, p2, alpha, beta, gamma, delta);
                            }
                        }
            }
        }
}

void Membrane::calculateHarmonicForces(ParticleContainer& container)
{
    for (auto& pair : directNeighbors)
        if (container.particles[pair.first].getActivity())
            for (auto& neighbor : pair.second)
                if (container.particles[neighbor].getActivity())
                    harmonic_calc(
                        container.particles[pair.first], container.particles[neighbor], k, r0);

    for (auto& pair : diagNeighbors)
        if (container.particles[pair.first].getActivity())
            for (auto& neighbor : pair.second)
                if (container.particles[neighbor].getActivity())
                    harmonic_calc(
                        container.particles[pair.first], container.particles[neighbor], k, diagR0);
}


bool Membrane::isNeighbor(size_t particleID1, size_t particleID2) {
    return neighboringRelations.find(getKey(particleID1, particleID2)) == neighboringRelations.end();
}

std::string Membrane::toString()
{
    std::ostringstream oss;
    oss << "pos: (" << origin[0] << ", " << origin[1] << ", " << origin[2] << ") ";
    oss << "NumParticles: (x: " << numParticlesWidth << ", y: " << numParticlesHeight
        << ", z: " << numParticlesDepth << ") ";
    oss << "mass: " << mass << " ";
    oss << "ptype: " << ptype << " ";
    oss << "r0: " << r0 << " ";
    oss << "k: " << k << " ";
    return oss.str();
}
