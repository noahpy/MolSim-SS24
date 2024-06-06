
#pragma once
#include "models/ParticleContainer.h"
#include "physics/strategy.h"
#include "simulation/baseSimulation.h"
#include "utils/Params.h"

/**
 * @brief A factory function for creating Simulation objects.
 *
 * This function creates and returns a Simulation object based on the simulation type provided,
 * along with other parameters required for simulation initialization.
 *
 * @param params A reference to the Params object containing simulation parameters.
 * @param particles A reference to the ParticleContainer containing particles for the simulation.
 * @param strat A reference to the PhysicsStrategy object defining the simulation strategy.
 * @param writePointer A unique pointer to a FileWriter object for writing simulation data.
 * @param readPointer A unique pointer to a FileReader object for reading simulation data.
 * @return A unique pointer to a Simulation object representing the initialized simulation.
 */
std::unique_ptr<Simulation> simFactory(
    Params& params,
    ParticleContainer& particles,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writePointer,
    std::unique_ptr<FileReader> readPointer);
