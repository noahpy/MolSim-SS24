
#pragma once
#include "models/ParticleContainer.h"
#include "physics/strategy.h"
#include "simulation/baseSimulation.h"

/**
 * @brief A factory function for creating Simulation objects.
 *
 * This function creates and returns a Simulation object based on the simulation type provided,
 * along with other parameters required for simulation initialization.
 *
 * @param sim_type An unsigned integer representing the type of simulation to create.
 * @param start_time The start time of the simulation.
 * @param delta_t The time step for the simulation.
 * @param end_time The end time of the simulation.
 * @param epsilon The epsilon value for the simulation.
 * @param sigma The sigma value for the simulation.
 * @param particles A reference to the ParticleContainer containing particles for the simulation.
 * @param strat A reference to the PhysicsStrategy object defining the simulation strategy.
 * @param writePointer A unique pointer to a FileWriter object for writing simulation data.
 * @param readPointer A unique pointer to a FileReader object for reading simulation data.
 * @return A unique pointer to a Simulation object representing the initialized simulation.
 */
std::unique_ptr<Simulation> simFactory(
    unsigned sim_type,
    double start_time,
    double delta_t,
    double end_time,
    double epsilon,
    double sigma,
    ParticleContainer& particles,
    PhysicsStrategy& strat,
    std::unique_ptr<FileWriter> writePointer,
    std::unique_ptr<FileReader> readPointer);
