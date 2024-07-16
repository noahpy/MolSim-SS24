
#include "Membrane.h"
#include "physics/forceCal/forceCal.h"
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
    double k,
    Particle& root_arg)
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
    , root(std::reference_wrapper<Particle>(root_arg))
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

    size_t firstRelevantDimension = numParticlesWidth == 1 ? 1 : 0;
    size_t secondRelevantDimension = firstRelevantDimension == 1 ? 2 : 1;
    std::array<int, 3> numParticles = { numParticlesWidth, numParticlesHeight, numParticlesDepth };

    // Ugly but needs to be initialized
    ParticleGrid particleGrid(
        numParticles[firstRelevantDimension],
        std::vector<std::reference_wrapper<Particle>>(
            numParticles[secondRelevantDimension], std::reference_wrapper<Particle>(root)));

    // TODO TMP!!!! FIXME LATER -> Problem: The container will resize and thus make the references
    // invalid
    container.particles =
        std::vector<Particle>(numParticlesWidth * numParticlesHeight * numParticlesDepth);
    int index = 0;

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
                size_t id = container.activeParticleCount; // TODO check id
                Particle particle(position, velocity, mass, static_cast<int>(ptype), id, true, moleculeID);

                // Add particle to container
                container.particles[index++] = particle;
                container.activeParticleCount++;

                // Add particle to grid
                particleGrid[indices[firstRelevantDimension]][indices[secondRelevantDimension]] =
                    std::reference_wrapper<Particle>(container.particles[index - 1]);
            }
        }
    }

    // Set root to the first particle
    root = particleGrid[0][0];
    root.get().setIsMoleculeRoot(true);

    // Set neighbors
    initNeighbors(particleGrid);

    spdlog::info("Generated Membrane: {}", toString());
}

void Membrane::initNeighbors(ParticleGrid& particleGrid)
{
    /**
     * Idea:
     * We only store half of the neighbors
     * This will allow us to use Netwon's 3rd law
     * By using a fix set of neighbors we avoid doubly calculating forces
     * Essentially we create a directed, non-cyclic graph with a single root
     * From this root, we can calculate all intra molecular forces of the membrane
     */
    std::vector<std::array<int, 2>> offsetKernelsDirect = {
        { 0, 1 }, // Top
        { 1, 0 } // Right
    };
    std::vector<std::array<int, 2>> offsetKernelsDiag = {
        { 1, 1 }, // Top-Right
        { 1, -1 } // Bottom-Right
    };

    for (int x = 0; x < particleGrid.size(); x++) { // x
        for (int y = 0; y < particleGrid[0].size(); y++) { // z
            directNeighbors[particleGrid[x][y]] = {};
            diagNeighbors[particleGrid[x][y]] = {};

            for (auto& offset : offsetKernelsDirect) {
                int i = offset[0] + x;
                int j = offset[1] + y;
                if (i >= 0 && i < particleGrid.size() && j >= 0 && j < particleGrid[0].size())
                    directNeighbors[particleGrid[x][y]].push_back(particleGrid[i][j]);
            }

            for (auto& offset : offsetKernelsDiag) {
                int i = offset[0] + x;
                int j = offset[1] + y;
                if (i >= 0 && i < particleGrid.size() && j >= 0 && j < particleGrid[0].size())
                    diagNeighbors[particleGrid[x][y]].push_back(particleGrid[i][j]);
            }
        }
    }
}

void Membrane::calculateIntraMolecularForces(const CellGrid& cellGrid)
{
    // Calculate harmonic forces
    calculateHarmonicForces();

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
                    if (pair.first.get().getMoleculeId() != root.get().getMoleculeId() ||
                        pair.first.get().getMoleculeId() != pair.second.get().getMoleculeId())
                        continue;

                    std::array<double, 3> delta = pair.first.get().getX() - pair.second.get().getX();
                    // Check if the distance is less than the cutoff to only have repulsive forces
                    if (ArrayUtils::DotProduct(delta) < cutoffRadiusSquared) {
                        lj_calc(pair.first, pair.second, alpha, beta, gamma, delta);
                    }
                }

                for (auto i : neighbors)
                    for (auto p1 : cellGrid.cells.at(x).at(y).at(z)->getParticles())
                        for (auto p2 : cellGrid.cells[i[0]][i[1]][i[2]]->getParticles()) {
                            // Skip iteration if not both particles are part of this membrane
                            if (p1.get().getMoleculeId() != root.get().getMoleculeId() ||
                                p1.get().getMoleculeId() != p2.get().getMoleculeId())
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

void Membrane::calculateHarmonicForces()
{
    // Calculate harmonic forces recursively
    for (auto& pair : directNeighbors)
        for (auto& neighbor : pair.second)
            harmonic_calc(pair.first.get(), neighbor.get(), k, r0);

    for (auto& pair : diagNeighbors)
        for (auto& neighbor : pair.second)
            harmonic_calc(pair.first.get(), neighbor.get(), k, diagR0);
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
