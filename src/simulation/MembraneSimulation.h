
#pragma once

#include "MixedLJSimulation.h"
#include "../models/membrane/Membrane.h"

/**
 * @brief Simulation class for the Lennard-Jones simulation with a molecules
 * @details This class is a subclass of the MixedLJSimulation and is used to simulate
 * different particles in one simulation, with an additional molecules.
 */
class MembraneSimulation : public MixedLJSimulation {
public:
    /**
     * @brief Construct a new Membrane Simulation object
     * @param time The current time of the simulation i.e. start time
     * @param delta_t The time increment for each iteration
     * @param end_time The end time of the simulation
     * @param container The container of particles
     * @param strat The strategy to be used for the physics calculations
     * @param writer The writer object to write the output to file
     * @param reader The reader object to read the input from file
     * @param LJParams A map that resolves particle type to their epsilon and sigma values
     * @param domainOrigin The origin of the simulation domain
     * @param domainSize The size of the simulation domain
     * @param cutoff The cutoff radius of the simulation
     * @param boundaryConfig The boundary condition configuration specification
     * @param gravity_constant The gravity constant to be used
     * @param T_init The initial temperature
     * @param T_target The target temperature
     * @param delta_T The maximal temperature change in one step
     * @param frequency The frequency for writing outputs (default = 10)
     * @param updateFrequency The frequency for updating the grid (default = 10)
     * @param read_file Whether to read the input file (default = true)
     * @param n_thermostat The number of steps between thermostat updates (default = 1000)
     */
    MembraneSimulation(
        double time,
        double delta_t,
        double end_time,
        ParticleContainer& container,
        PhysicsStrategy& strat,
        std::unique_ptr<FileWriter> writer,
        std::unique_ptr<FileReader> reader,
        const std::map<unsigned, std::pair<double, double>>& LJParams,
        std::array<double, 3> domainOrigin,
        std::array<double, 3> domainSize,
        double cutoff,
        std::array<double, 3> membraneOrigin,
        int numParticlesWidth,
        int numParticlesHeight,
        double k,
        double r_0,
        double spacing,
        const BoundaryConfig& boundaryConfig,
        double gravity_constant,
        double T_init,
        double T_target,
        double delta_T,
        unsigned frequency = 10,
        unsigned updateFrequency = 10,
        bool read_file = true,
        unsigned n_thermostat = 1000,
        bool doProfile = false);

    /**
     * @brief Run the simulation
     * @return void
     */
    void runSim() override;

    /**
     * @brief Get Stiffness constant
     * @return k
     */
    double getK() const { return k; }

    /**
     * @brief Get average bond length inside molecule
     * @return r_0
     */
    double getR_0() const { return r_0; }

    const Membrane& getMembrane() const { return membrane; }

protected:
    std::array<double, 3> membraneOrigin;
    int numParticlesWidth;
    int numParticlesHeight;
    double k;
    double r_0;
    double spacing;
    Membrane membrane; /**< The molecules used in the simulation */
};
