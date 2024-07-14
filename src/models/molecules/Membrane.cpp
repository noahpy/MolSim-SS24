
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
    : origin(origin)
    , numParticlesWidth(numParticlesWidth)
    , numParticlesHeight(numParticlesHeight)
    , numParticlesDepth(numParticlesDepth)
    , spacing(spacing)
    , mass(mass)
    , initialVelocity(initialVelocity)
    , meanVelocity(meanVelocity)
    , dimensions(dimensions)
    , ptype(ptype)
    , r0(r0)
    , diagR0(sqrt(2) * r0)
    , k(k)
    , root(root_arg)
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
                Particle particle(position, velocity, mass, static_cast<int>(ptype), moleculeID);

                // Add particle to container
                container.particles.push_back(particle);

                // Add particle to grid
                particleGrid[indices[firstRelevantDimension]][indices[secondRelevantDimension]] =
                    std::reference_wrapper<Particle>(particle);
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

    for (int x = 0; x < particleGrid.size(); x++) // x
        for (int y = 0; y < particleGrid[0].size(); y++) { // z

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

void Membrane::calculateIntraMolecularForces()
{
    // Calculate harmonic forces
    calculateHarmonicForces(root);
}

void Membrane::calculateHarmonicForces(std::reference_wrapper<Particle>& particle)
{
    // Calculate harmonic forces recursively
    for (auto& neighbor : directNeighbors.at(particle)) {
        calculateHarmonicForces(neighbor);
        harmonic_calc(particle.get(), neighbor.get(), k, r0);
    }
    for (auto& neighbor : diagNeighbors.at(particle)) {
        calculateHarmonicForces(neighbor);
        harmonic_calc(particle.get(), neighbor.get(), k, diagR0);
    }
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
