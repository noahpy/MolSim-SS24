
#pragma once

#include "models/ParticleContainer.h"
#include <memory>

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
     * @param stationaryParticleTypes The types of particles which are stationary
     * @param frequency The frequency for writing outputs
     * @return A new Simulation object
     */
    Simulation(
        double time,
        double delta_t,
        double end_time,
        ParticleContainer& container,
        PhysicsStrategy& strat,
        std::unique_ptr<FileWriter> writer,
        std::unique_ptr<FileReader> reader,
        std::vector<unsigned> stationaryParticleTypes,
        unsigned frequency = 10);

    /**
     * @brief Run the simulation
     */
    virtual void runSim() = 0;

    double time; /**< The current time of the simulation */
    double delta_t; /**< The time increment */
    double end_time; /**< The end time of the simulation */
    unsigned iteration = 0; /**< The current iteration of the simulation */
    unsigned frequency = 10; /**< The frequency for writing outputs */
    ParticleContainer& container; /**< The particle container which holds all particles */
    std::vector<unsigned> stationaryParticleTypes; /**< The types of particles which are stationary */

    /*
     * @brief Destructor of Simulation
     * */
    virtual ~Simulation() = default;

    /*
     * @brief Set the output file name of the writer
     * @param output Specifies the basename of the output file
     * */
    void setOutputFile(std::string output);

    /*
     * @brief Get the output file name of the writer
     * @return The basename of the output file
     * */
    std::string getOutputFile() const;

protected:
    PhysicsStrategy& strategy; /**< The strategy which is used to calculate the physics */
    std::unique_ptr<FileWriter> writer; /**< The output writer */
    std::unique_ptr<FileReader> reader; /**< The input reader */
};
