#include "lennardJonesSim.h"
#include "models/linked_cell/CellGrid.h"
#include "physics/boundaryConditions/BoundaryConditionHandler.h"

/**
 * @brief Simulation class for the Lennard-Jones simulation within a restricted domain
 * @details This class is a subclass of the Lennard-Jones simulation class and is used to simulate the movement of
 * particles according to the Lennard-Jones potential within a restricted domain. This is the simulation class used in the
 * third problem sheet.
 */
class LennardJonesDomainSimulation : public LennardJonesSimulation {
    // lift simulation constructor
    using LennardJonesSimulation::LennardJonesSimulation;

    /**
     * @brief Construct a new Lennard Jones Domain Simulation object
     * @param time The current time of the simulation i.e. start time
     * @param delta_t The time increment for each iteration
     * @param end_time The end time of the simulation
     * @param container The container of particles
     * @param strat The strategy to be used for the physics calculations
     * @param writer The writer object to write the output to file
     * @param reader The reader object to read the input from file
     * @param epsilon The epsilon parameter of the Lennard-Jones potential
     * @param sigma The sigma parameter of the Lennard-Jones potential
     * @param domain The domain in which the particles are restricted
     * @param bcHandler The boundary condition handler
     */
    LennardJonesDomainSimulation(
        double time,
        double delta_t,
        double end_time,
        ParticleContainer& container,
        PhysicsStrategy& strat,
        std::unique_ptr<FileWriter> writer,
        std::unique_ptr<FileReader> reader,
        double epsilon,
        double sigma,
        CellGrid& domain,
        BoundaryConditionHandler& bcHandler);

    /**
     * @brief Run the simulation
     * @return void
     */
    void runSim() override;

protected:
    CellGrid& domain; /**< The domain into which the particles are restricted */
    BoundaryConditionHandler& bcHandler; /**< The boundary condition handler */
};

