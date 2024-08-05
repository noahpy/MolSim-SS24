
#pragma once
#include "physics/thermostat/Thermostat.h"
#include "utils/Params.h"

/**
 * @brief A factory function for creating Thermostat objects.
 * @param type The type of thermostat to create.
 * @param init The initial temperature.
 * @param target The target temperature.
 * @param delta The maximum temperature change.
 * @param dim The dimension used.
 * @return The created thermostat object.
 */
std::unique_ptr<Thermostat> thermostatFactory(
    ThermostatType type, double init, double target, double delta, size_t dim);
