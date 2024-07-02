
#pragma once
#include "LennardJonesDomainSimulation.h"
#include "physics/thermostat/Thermostat.h"

/**
 * @brief Type to store the LJ parameters in. Here the key are the types of the particles to get the
 * param for
 */
typedef std::map<std::pair<int, int>, double> MixLJParamMap;

/**
 * @brief Simulation class for the Lennard-Jones simulation with mixed LJ parameters
 * @details This class is a subclass of the Lennard-Jones domain simulation class and is used to
 * simulate different particles in one simulation. For that the Lorentz-Berthelot mixing rules are
 * used
 */
class MixedLJSimulation : public LennardJonesDomainSimulation {
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
     * @param stationaryParticleTypes The types of particles which are stationary
     * @param LJParams A map that resolves particle type to their epsilon and sigma values
     * @param domainOrigin The origin of the simulation domain
     * @param domainSize The size of the simulation domain
     * @param cutoff The cutoff radius of the simulation
     * @param boundaryConfig The boundary condition configuration specification
     * @param gravity_constant The gravity constant to be used
     * @param thermostat The thermostat to be used
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
        std::map<unsigned , bool> stationaryParticleTypes,
        const std::map<unsigned, std::pair<double, double>>& LJParams,
        std::array<double, 3> domainOrigin,
        std::array<double, 3> domainSize,
        double cutoff,
        const BoundaryConfig& boundaryConfig,
        double gravity_constant,
        Thermostat thermostat,
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
     * @brief Get the key to access the LJ param maps
     * @param type1 The type of the first particle
     * @param type2 The type of the second particle
     * @return The pair that is used as the key in the maps
     */
    inline static std::pair<int, int> getMixKey(unsigned type1, unsigned type2)
    {
        if (type1 > type2) {
            std::swap(type1, type2);
        }
        // Always return the pair with the smaller type first
        return std::make_pair(type1, type2);
    }

    /**
     * @brief Get the epsilon of a combination of particles
     * @param type1 Type of the first particle
     * @param type2 Type of the second particle
     * @return The epsilon of both particles according to mixing rules
     */
    double getEpsilon(int type1, int type2) const { return epsilons.at(getMixKey(type1, type2)); }

    /**
     * @brief Get the sigma of a combination of particles
     * @param type1 Type of the first particle
     * @param type2 Type of the second particle
     * @return The sigma of both particles according to mixing rules
     */
    double getSigma(int type1, int type2) const { return sigmas.at(getMixKey(type1, type2)); }

    /**
     * @brief Get the alpha of a combination of particles
     * @param type1 Type of the first particle
     * @param type2 Type of the second particle
     * @return The alpha of both particles according to mixing rules
     */
    double getAlpha(int type1, int type2) const { return alphas.at(getMixKey(type1, type2)); }

    /**
     * @brief Get the beta of a combination of particles
     * @param type1 Type of the first particle
     * @param type2 Type of the second particle
     * @return The beta of both particles according to mixing rules
     */
    double getBeta(int type1, int type2) const { return betas.at(getMixKey(type1, type2)); }

    /**
     * @brief Get the gamma of a combination of particles
     * @param type1 Type of the first particle
     * @param type2 Type of the second particle
     * @return The gama of both particles according to mixing rules
     */
    double getGamma(int type1, int type2) const { return gammas.at(getMixKey(type1, type2)); }

    /**
     * @brief get the gravity constant
     * @return The gravity constant used in the simulation
     */
    double getGravityConstant() const { return gravityConstant; }

    /**
     * @brief Gets the distance for a particle where repulsion starts
     * @param type The type of the particle
     * @return The distance repulsion starts at
     */
    double getRepulsiveDistance(int type) const override;

    /**
     * @brief Get the initial temperature
     * @return The initial temperature
     */
    double getT_init() const { return T_init; }

    /**
     * @brief Get the target temperature
     * @return The target temperature
     */
    double getT_target() const { return T_target; }

    /**
     * @brief Get the delta_T
     * @return The delta_T
     */
    double getDelta_T() const { return delta_T; }

    /**
     * @brief Get the number of thermostat updates
     * @return The number of thermostat updates
     */
    unsigned getN_thermostat() const { return n_thermostat; }

    /**
     * @brief map that stores the different particle types
     */
    const std::map<unsigned, std::pair<double, double>> ljparams;

protected:
    MixLJParamMap epsilons; /**< The mixed epsilon parameters of the Lennard-Jones potential */
    MixLJParamMap sigmas; /**< The mixed sigma parameters of the Lennard-Jones potential */
    MixLJParamMap alphas; /**< -24 * epsilon */
    MixLJParamMap betas; /**< sigma^6 */
    MixLJParamMap gammas; /**< -2 * sigma^12 */

    std::map<unsigned, double>
        repulsiveDistances; /**< The repulsive distances for every particle */

    double gravityConstant; /**< The gravity constant */
    double T_init; /**< The initial temperature */
    double T_target; /**< The target temperature */
    double delta_T; /**< The maximal temperature change in one step */
    unsigned n_thermostat; /**< The number of steps between thermostat updates */
    Thermostat thermostat; /**< The thermostat */

private:
    // ---- Hide singe epsilon and sigma -------//
    using LennardJonesSimulation::getAlpha; //
    using LennardJonesSimulation::getBeta; //
    using LennardJonesSimulation::getEpsilon; //
    using LennardJonesSimulation::getGamma; //
    using LennardJonesSimulation::getSigma; //
    using LennardJonesSimulation::setEpsilon; //
    using LennardJonesSimulation::setSigma; //
    // -----------------------------------------//

    bool doProfile;
};
