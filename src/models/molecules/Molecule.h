
#pragma once

#include "models/Particle.h"
#include "models/ParticleContainer.h"
#include "models/linked_cell/CellGrid.h"
#include <functional>
#include <vector>

/**
 * @brief A class to model a molecule. It has a root particle and all particles of the molecules
 */
class Molecule {

public:
    /**
     * @brief Constructor for the Molecule class. Will initialize root and particles empty
     */
    explicit Molecule(unsigned ptype) : ptype(ptype) {};

    /**
     * @brief Default destructor for the Molecule class
     */
    virtual ~Molecule() = default;

    /**
     * @brief Create the necessary particles in the molecule
     * @param container The particle container to insert into
     * @param moleculeID The molecule ID (unique identifier for the molecule)
     */
    virtual void generateMolecule(ParticleContainer& container, size_t moleculeID) = 0;

    /**
     * @brief Calculate the molecular specific intra-molecular forces
     * @param cellGrid The cell simulation grid to calculate the forces
     */
    virtual void calculateIntraMolecularForces(const CellGrid& cellGrid) = 0;

    /**
     * @brief Initialize the Lennard-Jones parameters for the membrane
     * @param epsilon The epsilon value
     * @param sigma The sigma value
     */
    void initLJParams(double epsilon, double sigma);

    /**
     * @brief Get the particle type
     * @return The particle type
     */
    [[nodiscard]] unsigned getPtype() const { return ptype; }

protected:
    unsigned ptype; /**< The particle type */
    double alpha; /**< The alpha value for the LJ potential */
    double beta; /**< The beta value for the LJ potential */
    double gamma; /**< The gamma value for the LJ potential */
    double cutoffRadiusSquared; /**< The cutoff radius squared for the LJ potential */
    size_t ID; /**< The molecule ID */
};
