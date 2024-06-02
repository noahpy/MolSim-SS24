
#pragma once
#include "simulation/lennardJonesSim.h"
#include "models/linked_cell/CellGrid.h"

/**
 * @brief Simulation class for the Lennard-Jones simulation
 * @details This class is a subclass of the Simulation class and is used to simulate the movement of
 * particles according to the Lennard-Jones potential, by utilizing the linked-cell approach.
 */
class LinkedLennardJonesSimulation : public LennardJonesSimulation {
public:
    /**
     * @brief Construct a new Lennard Jones Simulation object
     * @param time The current time of the simulation i.e. start time
     * @param delta_t The time increment for each iteration
     * @param end_time The end time of the simulation
     * @param container The container of particles
     * @param strat The strategy to be used for the physics calculations
     * @param writer The writer object to write the output to file
     * @param reader The reader object to read the input from file
     * @param epsilon The epsilon parameter of the Lennard-Jones potential
     * @param sigma The sigma parameter of the Lennard-Jones potential
     * @param domainOrigin The origin of the simulation domain
     * @param domainSize The size of the simulation domain
     * @param cutoff The cutoff radius of the simulation
     */
    LinkedLennardJonesSimulation(
        double time,
        double delta_t,
        double end_time,
        ParticleContainer& container,
        PhysicsStrategy& strat,
        std::unique_ptr<FileWriter> writer,
        std::unique_ptr<FileReader> reader,
        double epsilon,
        double sigma,
        std::array<double, 3> domainOrigin,
        std::array<double, 3> domainSize, 
        double cutoff);

    /**
     * @brief Run the simulation
     * @return void
     */
    void runSim() override;

protected:
    CellGrid cellGrid;

};
