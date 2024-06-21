
#pragma once
#include "models/linked_cell/CellGrid.h"
#include "simulation/lennardJonesSim.h"

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
     * @param frequency The frequency for writing outputs
     * @param updateFrequency The frequency for updating the grid
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
        double cutoff,
        unsigned frequency = 10,
        unsigned updateFrequency = 10,
        bool read_file = true);

    /**
     * @brief Run the simulation
     * @return void
     */
    void runSim() override;

    [[nodiscard]] const CellGrid& getGrid() const { return cellGrid; }

    /**
     * @brief Set the origin of the simulation domain
     * @param domainOrigin The origin of the simulation domain
     */
    void setDomainOrigin(const std::array<double, 3>& domainOrigin);

    /**
     * @brief Set the size of the simulation domain
     * @param domainSize The size of the simulation domain
     */
    void setDomainSize(const std::array<double, 3>& domainSize);

    /**
     * @brief Set the cutoff radius of the simulation
     * @param cutoff The cutoff radius of the simulation
     */
    void setCutoff(double cutoff);

    /**
     * @brief Get the origin of the simulation domain
     * @return The origin of the simulation domain
     */
    inline std::array<double, 3> getDomainOrigin() const { return cellGrid.getDomainOrigin(); }

    /**
     * @brief Get the size of the simulation domain
     * @return The size of the simulation domain
     */
    inline std::array<double, 3> getDomainSize() const { return cellGrid.getDomainSize(); }

    /**
     * @brief Get the cutoff radius of the simulation
     * @return The cutoff radius of the simulation
     */
    inline double getCutoff() const { return cellGrid.getCutoffRadius(); }

    /*
     * @brief Get the update frequency of the simulation
     * @return The update frequency of the simulation
     * */
    inline unsigned getUpdateFrequency() const { return updateFrequency; }

protected:
    CellGrid cellGrid;
    unsigned updateFrequency;
};
