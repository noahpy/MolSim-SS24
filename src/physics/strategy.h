
#pragma once

#include "simulation/baseSimulation.h"
#include <functional>

/**
 * @brief Strategy pattern for physics calculations
 * @details This is a strategy pattern for physics calculations.
 * This allows to specify which physics calculations to perform at runtime,
 * by passing the function references.
 * By passing the simulation object to the functions, the functions can
 * use members which are only defined in subclasses of Simulation.
 */
class PhysicsStrategy {
public:
    /**
     * @brief Construct a new Physics Strategy object
     * @param calX The function to calculate the position from the passed simulation
     * @param calV The function to calculate the velocity from the passed simulation
     * @param calF The function to calculate the force from the passed simulation
     * @return PhysicsStrategy object
     */
    PhysicsStrategy(
        std::function<void(const Simulation&)> calX,
        std::function<void(const Simulation&)> calV,
        std::function<void(const Simulation&)> calF);

    /**
     * @brief Function to calculate the positions within the simulation
     */
    std::function<void(const Simulation&)> calX;
    /**
     * @brief Function to calculate the velocities within the simulation
     */
    std::function<void(const Simulation&)> calV;
    /**
     * @brief Function to calculate the forces within the simulation
     */
    std::function<void(const Simulation&)> calF;
};
