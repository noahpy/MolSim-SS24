
#include "models/ParticleContainer.h"
#include "simulation/baseSimulation.h"

/**
 * @brief Simulation class for the Lennard-Jones simulation
 * @details This class is a subclass of the Simulation class and is used to simulate the movement of
 * particles according to the Lennard-Jones potential. This is the simulation class used in the
 * second problem sheet.
 */
class LennardJonesSimulation : public Simulation {
public:
    // lift simulation constructor
    using Simulation::Simulation;

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
     * @param frequency The frequency for writing outputs
     * @param read_file Set to true to read the input file
     */
    LennardJonesSimulation(
        double time,
        double delta_t,
        double end_time,
        ParticleContainer& container,
        PhysicsStrategy& strat,
        std::unique_ptr<FileWriter> writer,
        std::unique_ptr<FileReader> reader,
        double epsilon,
        double sigma,
        unsigned frequency = 10,
        bool read_file = true);

    /**
     * @brief Run the simulation
     * @return void
     */
    void runSim() override;

    /**
     * @brief Set the epsilon parameter of the Lennard-Jones potential
     * @param eps The new epsilon value
     */
    void setEpsilon(double eps);

    /**
     * @brief Set the sigma parameter of the Lennard-Jones potential
     * @param sig The new sigma value
     */
    void setSigma(double sig);

    /**
     * @brief Get the epsilon parameter of the Lennard-Jones potential
     * @return The epsilon parameter
     */
    double getEpsilon() const { return epsilon; }
    /**
     * @brief Get the sigma parameter of the Lennard-Jones potential
     * @return The sigma parameter
     */
    double getSigma() const { return sigma; }
    /**
     * @brief Get the alpha parameter of the Lennard-Jones potential
     * @details The alpha parameter is calculated as -24 * epsilon and will be used to simplify the
     * calculations
     * @return The alpha parameter i.e. -24 * epsilon
     */
    double getAlpha() const { return alpha; }
    /**
     * @brief Get the beta parameter of the Lennard-Jones potential
     * @details The beta parameter is calculated as sigma^6 and will be used to simplify the
     * calculations
     * @return The beta parameter i.e. sigma^6
     */
    double getBeta() const { return beta; }
    /**
     * @brief Get the gamma parameter of the Lennard-Jones potential
     * @details The gamma parameter is calculated as -2 * sigma^12 and will be used to simplify the
     * calculations
     * @return The gamma parameter i.e. -2 * sigma^12
     */
    double getGamma() const { return gamma; }

    virtual ~LennardJonesSimulation() = default;

protected:
    double epsilon; /**< The epsilon parameter of the Lennard-Jones potential */
    double sigma; /**< The sigma parameter of the Lennard-Jones potential */
    double alpha; /**< -24 * epsilon, will be used to simplify the calculations */
    double beta; /**< sigma^6, will be used to simplify the calculations */
    double gamma; /**< -2 * sigma^12, will be used to simplify the calculations */
};
