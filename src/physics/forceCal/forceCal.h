
#pragma once
#include "simulation/baseSimulation.h"

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