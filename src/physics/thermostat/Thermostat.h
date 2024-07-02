
#pragma once

#include "models/ParticleContainer.h"

/**
 * @class Thermostat
 * @brief Class to apply a thermostat to particles.
 *
 * This class provides methods for initialization and updating of temperature on all particles,
 * according to given initial and target temperatures.
 */
class Thermostat {
public:
    /**
     * @brief Default constructor for Thermostat.
     */
    Thermostat() = default;

    /**
     * @brief Constructor for Thermostat.
     * @param init Initial temperature.
     * @param target Target temperature.
     * @param delta Maximum temperature change.
     * @param dim The dimension used.
     */
    Thermostat(double init, double target, double delta, size_t dim);

    /**
     * @brief Initialization with Brownian Motion using the Maxwell-Boltzmann distribution.
     * @param sim The simulation to initialize.
     */
    void initializeBrownianMotion(ParticleContainer& sim) const;

    /**
     * @brief Updates the temperature in our simulation.
     *        After caldulating the kinetic energy, current temperature
     *        and new temperature, the velocities of all particles
     *        are scaled with the scaling factor beta.
     * @param sim The simulation to apply the temperature update to.
     */
    virtual void updateT(ParticleContainer& sim) const;

    /**
     * @brief Calculates the total kinetic energy of the particles in the simulation.
     * @param container The container of particles in the simulation.
     * @return The total kinetic energy of the particles in the simulation.
     */
    virtual double getTotalKineticEnergy(ParticleContainer& container) const;

    /**
     * @brief Get the name of the thermostat.
     * @return The name of the thermostat.
     */
    [[nodiscard]] virtual std::string getName() const;

    /**
     * @brief Calculates the scaling factor beta.
     * @param T_current The current temperature.
     * @return The scaling factor beta.
     */
    [[nodiscard]] double getBeta(double T_current) const;

    /**
     * @brief Get the initial temperature.
     * @return The initial temperature.
     */
    [[nodiscard]] double getInit() const { return init; }
    /**
     * @brief Get the target temperature.
     * @return The target temperature.
     */
    [[nodiscard]] double getTarget() const { return target; }
    /**
     * @brief Get the maximum temperature change.
     * @return The maximum temperature change.
     */
    [[nodiscard]] double getDelta() const { return delta; }
    /**
     * @brief Get the dimension used.
     * @return The dimension used.
     */
    [[nodiscard]] size_t getDim() const { return dim; }

protected:
    // Initial temperature
    double init;
    // Target temperature
    double target;
    // Maximum temperature change
    double delta;
    // dimension of simulation
    size_t dim;
};
