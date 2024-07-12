
#pragma once
#include "Molecule.h"
#include <map>

struct ParticleComparator {
    bool operator()(const std::reference_wrapper<Particle>& lhs, const std::reference_wrapper<Particle>& rhs) const {
        return &lhs.get() < &rhs.get();
    }
};

/**
 * @brief A map from particles to their neighbors
 */
typedef std::map<std::reference_wrapper<Particle>, std::vector<std::reference_wrapper<Particle>>, ParticleComparator> NeighborParticleMap;

/**
 * @brief A class to model a molecules of particles
 */
class Membrane : public Molecule {
public:
    /**
     * @brief Construct a new Membrane object
     * @param origin The origin of the membrane
     * @param numParticlesWidth The number of particles in width
     * @param numParticlesHeight The number of particles in height
     * @param numParticlesDepth The number of particles in depth
     * @param spacing The spacing between particles
     * @param mass The mass of the particles
     * @param initialVelocity The initial velocity of the particles
     * @param meanVelocity The mean velocity of the particles
     * @param dimensions The dimensions of the particles
     * @param ptype The particle type
     * @param r0 The equilibrium distance
     * @param k The spring constant
     * @param root_arg The root particle of the molecule. Will be overwritten after generate molecule
     */
    Membrane(
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
        Particle& root_arg);

    /**
     * @brief Generate the membrane
     * @param container The container to generate the membrane in
     */
    void generateMolecule(ParticleContainer& container) override;

    /**
     * @brief Calculate the membrane specific intra-molecular forces using the harmonic potential
     */
    void calculateIntraMolecularForces() override;

    /**
     * @brief Calculate the harmonic forces for a particle and its neighbors (will be called recursively)
     * @param Particle The root particle
     */
    void calculateHarmonicForces(std::reference_wrapper<Particle>& membraneRoot);

protected:
    std::reference_wrapper<Particle> root; /**< The root particle of the molecule */
    /**
     * Idea: Only store the neighbors TOP, TOP-RIGHT, RIGHT, BOTTOM-RIGHT
     * This way, we can simply calculate the harmonic potential for those neighbors
     * Then use newton's third law -> this way we only loop over all particles but get all forces
     */
    NeighborParticleMap directNeighbors; /**< The direct neighbors of the particles (only the ones that are required to calculate the forces) */
    NeighborParticleMap verticalNeighbors; /**< The vertical neighbors of the particles (only the ones that are required to calculate the forces) */

    std::array<double, 3> origin; /**< The origin of the membrane */
    int numParticlesWidth; /**< The number of particles in width */
    int numParticlesHeight; /**< The number of particles in height */
    int numParticlesDepth; /**< The number of particles in depth */
    double spacing; /**< The spacing between particles */
    double mass; /**< The mass of the particles */
    std::array<double, 3> initialVelocity; /**< The initial velocity of the particles */
    double meanVelocity; /**< The mean velocity of the particles */
    size_t dimensions; /**< The dimensions of the particles */
    unsigned ptype; /**< The particle type */
    double r0; /**< The equilibrium distance */
    double k; /**< The spring constant */
};
