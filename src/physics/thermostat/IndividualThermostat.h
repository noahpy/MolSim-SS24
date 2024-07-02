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

    void updateT(ParticleContainer& sim) const override;

    /**
     * @brief Calculates the total kinetic energy of the particles in the simulation while ignoring the mean velocity. This is used in problem sheet 5
     * @param container The container of particles in the simulation.
     * @return The total kinetic energy of the particles in the simulation (ignoring mean velocity).
     */
    double getTotalKineticEnergy(ParticleContainer& container) const override;

    /**
     * @brief Get the name of the thermostat.
     * @return The name of the thermostat.
     */
    [[nodiscard]] std::string getName() const override;

protected:
    /**
     * @brief Calculates the mean velocity of the particles in the simulation.
     * @param container The container of particles in the simulation.
     * @return The mean velocity of the particles in the simulation.
     */
    static std::array<double, 3> getMeanVelocity(ParticleContainer& container) ;
};
