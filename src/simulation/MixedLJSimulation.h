
#include "LennardJonesDomainSimulation.h"

/**
 * @brief Type to store the LJ parameters in. Here the key are the types of the particles to get the param for
 */
typedef std::map<std::pair<int, int>, double> MixLJParamMap;

/**
 * @brief Simulation class for the Lennard-Jones simulation with mixed LJ parameters
 * @details This class is a subclass of the Lennard-Jones domain simulation class and is used to simulate different particles in one simulation. For that the Lorentz-Berthelot mixing rules are used
 */
class MixedLJSimulation : public LennardJonesDomainSimulation{
public:
    /**
     * @brief Construct a new Mixed Lennard Jones Domain Simulation object
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
     * @param boundaryConfig The boundary condition configuration specification
     * @param gravity_constant The gravity constant to be used
     * @param T_init The initial temperature
     * @param T_target The target temperature
     * @param delta_T The maximal temperature change in one step
     * @param frequency The frequency for writing outputs (default = 10)
     * @param updateFrequency The frequency for updating the grid (default = 10)
     * @param read_file Whether to read the input file (default = true)
     * @param n_thermostat The number of steps between thermostat updates (default = 1000
     */
    MixedLJSimulation(
        double time,
        double delta_t,
        double end_time,
        ParticleContainer& container,
        PhysicsStrategy& strat,
        std::unique_ptr<FileWriter> writer,
        std::unique_ptr<FileReader> reader,
        std::vector<std::pair<int, double>> epsilons,
        std::vector<std::pair<int, double>> sigmas,
        std::array<double, 3> domainOrigin,
        std::array<double, 3> domainSize,
        double cutoff,
        const BoundaryConfig& boundaryConfig,
        double gravity_constant,
        double T_init,
        double T_target,
        double delta_T,
        unsigned frequency = 10,
        unsigned updateFrequency = 10,
        bool read_file = true,
        unsigned n_thermostat = 1000
        );

protected:
    MixLJParamMap epsilons; /**< The mixed epsilon parameters of the Lennard-Jones potential */
    MixLJParamMap sigmas; /**< The mixed sigma parameters of the Lennard-Jones potential */
    MixLJParamMap alphas; /**< -24 * epsilon */
    MixLJParamMap betas; /**< sigma^6 */
    MixLJParamMap gammas; /**< -2 * sigma^12 */

    double gravity_constant; /**< The gravity constant */
    double T_init; /**< The initial temperature */
    double T_target; /**< The target temperature */
    double delta_T; /**< The maximal temperature change in one step */
    unsigned n_thermostat; /**< The number of steps between thermostat updates */

private:
    // ---- Hide singe epsilon and sigma -------//
    using LennardJonesSimulation::getSigma;     //
    using LennardJonesSimulation::setSigma;     //
    using LennardJonesSimulation::getEpsilon;   //
    using LennardJonesSimulation::setEpsilon;   //
    using LennardJonesSimulation::getAlpha;     //
    using LennardJonesSimulation::getBeta;      //
    using LennardJonesSimulation::getGamma;     //
    // -----------------------------------------//

};
