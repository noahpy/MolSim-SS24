
#include "ThermostatFactory.h"
#include "IndividualThermostat.h"

std::unique_ptr<Thermostat> thermostatFactory(
    ThermostatType type, double init, double target, double delta, size_t dim)
{
    switch (type) {
    case ThermostatType::CLASSICAL:
        return std::make_unique<Thermostat>(Thermostat(init, target, delta, dim));
    case ThermostatType::INDIVIDUAL:
        return std::make_unique<Thermostat>(IndividualThermostat(init, target, delta, dim));
    default:
        throw std::invalid_argument("Invalid thermostat type");
    }
}
