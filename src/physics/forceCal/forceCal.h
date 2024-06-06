
#pragma once
#include "simulation/baseSimulation.h"
#include "simulation/linkedLennardJonesSim.h"

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
