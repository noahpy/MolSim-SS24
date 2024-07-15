
#pragma once

#include "physics/boundaryConditions/BoundaryConditionHandler.h"
#include "simulation/linkedLennardJonesSim.h"

/**
 * @brief Forward declaration of the Analyzer class
 */
class Analyzer;

/**
 * @brief Simulation class for the Lennard-Jones simulation within a restricted domain and linked
 * cell algorithm
 * @details This class is a subclass of the Lennard-Jones simulation class using linked cells and is
 * used to simulate the movement of particles according to the Lennard-Jones potential within a
 * restricted domain. This is the simulation class used in the third problem sheet.
 */
class LennardJonesDomainSimulation : public LinkedLennardJonesSimulation {
public:
    // lift simulation constructor
    using LinkedLennardJonesSimulation::LinkedLennardJonesSimulation;

    /**
     * @brief Construct a new Lennard Jones Domain Simulation object
     * @param time The current time of the simulation i.e. start time
     * @param delta_t The time increment for each iteration
     * @param end_time The end time of the simulation
     * @param container The container of particles
     * @param strat The strategy to be used for the physics calculations
     * @param writer The writer object to write the output to file
     * @param reader The reader object to read the input from file
     * @param stationaryParticleTypes The types of particles which are stationary
     * @param epsilon The epsilon parameter of the Lennard-Jones potential
     * @param sigma The sigma parameter of the Lennard-Jones potential
     * @param domainOrigin The origin of the simulation domain
     * @param domainSize The size of the simulation domain
     * @param cutoff The cutoff radius of the simulation
     * @param boundaryConfig The boundary condition configuration specification
     * @param analyzer The analyzer object to analyze the simulation
     * @param frequency The frequency for writing outputs (default = 10)
     * @param updateFrequency The frequency for updating the grid (default = 10)
     * @param analysisFrequency The frequency for analyzing the simulation (default = 10000)
     * @param read_file Whether to read the input file (default = true)
     */
    LennardJonesDomainSimulation(
        double time,
        double delta_t,
        double end_time,
        ParticleContainer& container,
        PhysicsStrategy& strat,
        std::unique_ptr<FileWriter> writer,
        std::unique_ptr<FileReader> reader,
        std::map<unsigned , bool> stationaryParticleTypes,
        double epsilon,
        double sigma,
        std::array<double, 3> domainOrigin,
        std::array<double, 3> domainSize,
        double cutoff,
        const BoundaryConfig& boundaryConfig,
        std::unique_ptr<Analyzer> analyzer,
        unsigned frequency = 10,
        unsigned updateFrequency = 10,
        size_t analysisFrequency = 10000,
        bool read_file = true);

    /**
     * @brief Run the simulation
     * @return void
     */
    void runSim() override;

    BoundaryConditionHandler bcHandler; /**< The boundary condition handler */

    /**
     * @brief Gets the distance for a particle where repulsion starts
     * @param type The type of the particle
     * @return The distance repulsion starts at
     */
    virtual double getRepulsiveDistance(int type) const;

protected:
    size_t analysisFrequency; /**< The frequency for analyzing the simulation */
    std::unique_ptr<Analyzer> analyzer; /**< The analyzer object */

private:
    double repulsiveDistance;
};
