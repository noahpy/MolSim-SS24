
#pragma once
#include "physics/strategy.h"
#include "utils/Params.h"

/**
 * @brief A factory function for creating PhysicsStrategy objects.
 *
 * This function creates and returns a PhysicsStrategy object based on the simulation type provided.
 *
 * @param simulation_type An unsigned integer representing the type of simulation strategy to
 * create.
 * @return A PhysicsStrategy object representing the appropriate simulation strategy.
 */
PhysicsStrategy stratFactory(SimulationType simulation_type);
