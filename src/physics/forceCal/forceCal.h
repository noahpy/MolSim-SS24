
#pragma once
#include "simulation/baseSimulation.h"
#include "simulation/MixedLJSimulation.h"

/**
 * @brief Calculate the forces between particles using the Stroemer-Verlet algorithm
 * @param sim The simulation object to calculate the forces for
 * @return void
 */
void force_gravity(const Simulation& sim);

/**
 * @brief Calculate the forces between particles using the Stroemer-Verlet algorithm, while
 * exploiting F_12 = -F_21 and iterating over distinct pairs
 * @param sim The simulation object to calculate the forces for
 * @return void
 */
void force_gravity_V2(const Simulation& sim);

/**
 * @brief Calculate the forces between particles using the Lennard-Jones potential
 * @param sim The Lennard Jones simulation object to calculate the forces for
 * @return void
 */
void force_lennard_jones(const Simulation& sim);

/**
 * @brief Calculate the forces between particles in a linked-cell structure using the Lennard-Jones
 * potential
 * @param sim The Lennard Jones simulation object to calculate the forces for
 * @return void
 */
void force_lennard_jones_lc(const Simulation& sim);

/**
 * @brief Calculate the forces between particles in a linked-cell structure using the Lennard-Jones
 * potential and the mixing rules to allow multiple particle types & regard gravity
 * @param sim The simulation to calculate the forces for
 */
void force_mixed_LJ_gravity_lc(const Simulation& sim);

/**
 * @brief Calculate the forces between particles in a linked-cell structure using the Lennard-Jones
 * potential, the mixing rules to allow multiple particle types & regard gravity and the harmonic
 * potential between particles of a molecules
 * @param sim The simulation to calculate the forces for
 */
void force_membrane(const Simulation& sim);