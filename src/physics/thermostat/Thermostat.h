
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
    void updateT(ParticleContainer& sim) const;

private:
    // Initial temperature
    double init;
    // Target temperature
    double target;
    // Maximum temperature change
    double delta;
    // dimension of simulation
    size_t dim;
};