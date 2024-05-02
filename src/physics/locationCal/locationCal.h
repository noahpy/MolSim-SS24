
# pragma once
#include "simulation/baseSimulation.h"

/**
 * @brief Calculate the new positions of the particles using the Stroemer-Verlet algorithm
 * @param sim The simulation object to calculate the new positions for
 * @return void
 */
void location_stroemer_verlet(const Simulation& sim);
