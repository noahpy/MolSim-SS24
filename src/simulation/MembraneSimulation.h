
#pragma once

#include "MixedLJSimulation.h"
#include "models/molecules/Molecule.h"
#include "analytics/Analyzer.h"

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
     * @param stationaryParticleTypes
     * @param LJParams A map that resolves particle type to their epsilon and sigma values
     * @param domainOrigin The origin of the simulation domain
     * @param domainSize The size of the simulation domain
     * @param cutoff The cutoff radius of the simulation
     * @param boundaryConfig The boundary condition configuration specification
     * @param analyzer The analyzer object to analyze the simulation
     * @param gravity_constant The gravity constant to be used
     * @param thermostat The thermostat to be used in the simulation
     * @param molecules The molecules to be used in the simulation
     * @param frequency The frequency for writing outputs (default = 10)
     * @param updateFrequency The frequency for updating the grid (default = 10)
     * @param analysisFrequency The frequency for analysis (default = 10000)
     * @param read_file Whether to read the input file (default = true)
     * @param n_thermostat The number of steps between thermostat updates (default = 1000)
     * @param doProfile Whether to profile the simulation (default = false)
     */
    MembraneSimulation(
        double time,
        double delta_t,
        double end_time,
        ParticleContainer& container,
        PhysicsStrategy& strat,
        std::unique_ptr<FileWriter> writer,
        std::unique_ptr<FileReader> reader,
        std::map<unsigned , bool> stationaryParticleTypes,
        const std::map<unsigned, std::pair<double, double>>& LJParams,
        std::array<double, 3> domainOrigin,
        std::array<double, 3> domainSize,
        double cutoff,
        const BoundaryConfig& boundaryConfig,
        std::unique_ptr<Analyzer> analyzer,
        double gravity_constant,
        std::unique_ptr<Thermostat> thermostat,
        std::vector<std::unique_ptr<Molecule>> molecules,
        unsigned frequency = 10,
        unsigned updateFrequency = 10,
        size_t analysisFrequency = 10000,
        bool read_file = true,
        unsigned n_thermostat = 1000,
        bool doProfile = false);

    /**
     * @brief Run the simulation
     * @return void
     */
    void runSim() override;

    /**
     * @brief Get the molecules in the simulation
     * @return The molecules in the simulation
     */
    [[nodiscard]] const std::vector<std::unique_ptr<Molecule>>& getMolecules() const { return molecules; }

protected:
    std::vector<std::unique_ptr<Molecule>> molecules; /**< The molecules in the simulation */
};
