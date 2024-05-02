
# pragma once
#include "simulation/baseSimulation.h"

/**
 * @brief Calculate the velocity for the simulation using the Stroemer-Verlet method
 * @param sim The simulation object to calculate the velocities for
 */
void velocity_stroemer_verlet(const Simulation& sim);
