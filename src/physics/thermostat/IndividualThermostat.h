#pragma once

#include "Thermostat.h"
class IndividualThermostat : public Thermostat {
public:
    /**
     * @brief Constructor for IndividualThermostat.
     * @param init Initial temperature.
     * @param target Target temperature.
     * @param delta Maximum temperature change.
     * @param dim The dimension used.
     */
    IndividualThermostat(double init, double target, double delta, size_t dim);

    /**
     * @brief Updates the temperature of the particles in the simulation. Ignoring the mean velocity
     * and any immobile particles.
     * @param sim The simulation to update the temperature of.
     */
    void updateT(Simulation& sim) override;

    /**
     * @brief Calculates the total kinetic energy of the particles in the simulation while ignoring
     * the mean velocity. This is used in problem sheet 5
     * @attention This is not the total kinetic energy of the system, but actually double of it. This is because the temperature calculations also divide by 2.
     * @param sim The simulation to get the total kinetic energy for.
     * @return The total kinetic energy of the particles in the simulation (ignoring mean velocity).
     */
    double getTotalKineticEnergy(Simulation& sim) override;

    /**
     * @brief Get the name of the thermostat.
     * @return The name of the thermostat.
     */
    [[nodiscard]] std::string getName() const override;

    /**
     * @brief Initializes the thermostat.
     * @param sim The simulation to initialize the thermostat for.
     */
    void initialize(Simulation& sim);

    /**
     * @brief Calculates the mean velocity of the particles in the simulation.
     * @param container The container of particles in the simulation.
     * @return The mean velocity of the particles in the simulation.
     */
    std::array<double, 3> getMeanVelocity(Simulation& sim) const;

protected:
    size_t numFixParticles; /**< The number of fixed particles in the simulation. */
    bool isInitialized = false; /**< Whether the thermostat has been initialized. */
};
