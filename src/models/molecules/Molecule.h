
#pragma once

#include "models/Particle.h"
#include "models/ParticleContainer.h"
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
    explicit Molecule() = default;

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
     */
    virtual void calculateIntraMolecularForces() = 0;
};
