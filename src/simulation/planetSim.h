
#include "models/ParticleContainer.h"
#include "simulation/baseSimulation.h"

/**
 * @brief Simulation class for the planet simulation
 * @details This class is a subclass of the Simulation class and is used to simulate the movement of
 * planets This is the simulation class used in the first problem sheet
 */
class PlanetSimulation : public Simulation {
public:
    // lift simulation constructor
    using Simulation::Simulation;

    /**
     * @brief Construct a new Planet Simulation object
     * @details This constructor is used to create a new Planet Simulation object and will simply
     * call the constructor of the Simulation class
     * @param time The current time of the simulation i.e. start time
     * @param delta_t The time increment for each iteration
     * @param end_time The end time of the simulation
     * @param container The container of particles
     * @param strat The strategy to be used for the physics calculations
     * @param writer The writer object to write the output to file
     * @param reader The reader object to read the input from file
     * @param stationaryParticleTypes The types of particles which are stationary
     * @param frequency The frequency for writing outputs
     * @return A new Planet Simulation object
     */
    PlanetSimulation(
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
     * @return void
     */
    void runSim() override;
};
