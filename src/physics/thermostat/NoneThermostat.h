#pragma once

#include "Thermostat.h"

class NoneThermostat : public Thermostat {
public:
    /**
     * @brief Constructor for NoneThermostat
     */
    NoneThermostat() = default;

    /**
     * @brief Does no updates
     * @param sim The simulation to update the temperature of.
     */
    void updateT(Simulation& sim) override {
        // Do nothing
    };

    /**
     * @brief Does no initialization
     * @param sim The simulation to initialize.
     */
    void initializeBrownianMotion(Simulation& sim) const override
    {
        // Do nothing
    }

    /**
     * @brief Does nothing
     * @param sim The simulation to get the total kinetic energy for.
     * @return 0 as it does nothing
     */
    double getTotalKineticEnergy(Simulation& sim) override
    {
        // Do nothing
        return 0;
    };

    /**
     * @brief Get the name of the thermostat.
     * @return The name of the thermostat.
     */
    [[nodiscard]] std::string getName() const override { return "No Thermostat active"; };
};