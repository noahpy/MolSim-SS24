
#include "IndividualThermostat.h"
#include "utils/ArrayUtils.h"
#include <spdlog/spdlog.h>

IndividualThermostat::IndividualThermostat(double init, double target, double delta, size_t dim)
    : Thermostat(init, target, delta, dim)
    , numFixParticles(0)
    , isInitialized(false)
{
}

void IndividualThermostat::updateT(Simulation& sim)
{
    if (!isInitialized)
        initialize(sim);

    // Calculate current Temperature
    double T_current =
        getTotalKineticEnergy(sim) /
        static_cast<double>((sim.container.activeParticleCount - numFixParticles) * dim);

    double beta = getBeta(T_current);

    spdlog::debug("Current Temperature: {}, Target: {}, beta: {}", T_current, target, beta);

    std::array<double, 3> meanVelocity = getMeanVelocity(sim);
    for (auto& p : sim.container) {
        if (p.getActivity() && p.getIsNotStationary())
            p.setV(beta * (p.getV() - meanVelocity) + meanVelocity);
    }
}

double IndividualThermostat::getTotalKineticEnergy(Simulation& sim)
{
    double E = 0;
    std::array<double, 3> meanVelocity = getMeanVelocity(sim);
    for (auto& p : sim.container)
        if (p.getActivity() && p.getIsNotStationary())
            E += p.getM() * (ArrayUtils::DotProduct(p.getV() - meanVelocity));
    return E;
}

std::array<double, 3> IndividualThermostat::getMeanVelocity(Simulation& sim) const
{
    std::array<double, 3> meanVelocity = { 0, 0, 0 };
    for (auto& p : sim.container)
        if (p.getActivity() && p.getIsNotStationary())
            meanVelocity = meanVelocity + p.getV();

    meanVelocity =
        (1.0 / static_cast<double>(sim.container.activeParticleCount - numFixParticles)) *
        meanVelocity;
    return meanVelocity;
}

void IndividualThermostat::initialize(Simulation& sim)
{
    if (isInitialized)
        return;

    for (auto& p : sim.container)
        if (!p.getIsNotStationary())
            numFixParticles++;

    isInitialized = true;
}

std::string IndividualThermostat::getName() const
{
    return "IndividualThermostat";
}
