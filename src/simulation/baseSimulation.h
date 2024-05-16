
#pragma once

#include "models/ParticleContainer.h"

// forward-declare PhysicsStrategy
class PhysicsStrategy;
// forward-declare FileWriter
class FileWriter;
// forward-declare FileReader
class FileReader;

/**
 * @brief Abstract base class for all simulations
 * @details This class is the base class for all simulations. It contains the
 * members need to do the calculations for the different strategies.
 */
class Simulation {
public:
    /**
     * @brief Construct a new Simulation object
     * @param time The start time of the simulation
     * @param delta_t The time increment
     * @param end_time The end time of the simulation
     * @param container The particle container which holds all particles
     * @param strat The strategy which is used to calculate the physics
     * @param writer The output writer
     * @param reader The input reader
     * @return A new Simulation object
     */
    Simulation(
        double time,
        double delta_t,
        double end_time,
        ParticleContainer& container,
        PhysicsStrategy& strat,
        FileWriter& writer,
        FileReader& reader);

    /**
     * @brief Run the simulation
     */
    virtual void runSim() = 0;

    double time; /**< The current time of the simulation */
    double delta_t; /**< The time increment */
    double end_time; /**< The end time of the simulation */
    unsigned iteration = 0; /**< The current iteration of the simulation */
    ParticleContainer& container; /**< The particle container which holds all particles */

protected:
    PhysicsStrategy& strategy; /**< The strategy which is used to calculate the physics */
    FileWriter& writer; /**< The output writer */
    FileReader& reader; /**< The input reader */
};
