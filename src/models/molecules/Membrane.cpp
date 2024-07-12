
#include "Membrane.h"
#include "spdlog/spdlog.h"

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
    , k(k)
    , root(root_arg)
{
}

void Membrane::generateMolecule(ParticleContainer& container) {
    // Same as in CuboidParticleCluster but add neighbors
}

void Membrane::calculateIntraMolecularForces() {
    // Calculate harmonic forces
    calculateHarmonicForces(root);
}

void Membrane::calculateHarmonicForces(std::reference_wrapper<Particle>& membraneRoot) {
    // Calculate harmonic forces recursively
    for (auto& neighbor : directNeighbors.at(membraneRoot)) {
        calculateHarmonicForces(neighbor);
    }
    for (auto& neighbor : verticalNeighbors.at(membraneRoot)) {
        calculateHarmonicForces(neighbor);
    }
}


