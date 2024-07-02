
#include "IndividualThermostat.h"
#include "utils/ArrayUtils.h"
#include <spdlog/spdlog.h>

IndividualThermostat::IndividualThermostat(double init, double target, double delta, size_t dim)
    : Thermostat(init, target, delta, dim)
{
}

void IndividualThermostat::updateT(ParticleContainer& container) const {
    // Calculate current Temperature
    double T_current =
        getTotalKineticEnergy(container) / (double)(container.activeParticleCount * dim);

    double beta = getBeta(T_current);

    spdlog::debug(
        "Current Temperature: {}, Target: {}, beta: {}",
        T_current,
        target,
        beta);

    std::array<double, 3> meanVelocity = getMeanVelocity(container);
    for (auto& p : container) {
        p.setV(beta * (p.getV()-meanVelocity) + meanVelocity);
    }
}

double IndividualThermostat::getTotalKineticEnergy(ParticleContainer& container) const {
    double E = 0;
    std::array<double, 3> meanVelocity = getMeanVelocity(container);
    for (auto& p : container)
        if (p.getActivity())
            E += p.getM() * (ArrayUtils::DotProduct(p.getV() - meanVelocity));
    return E;
}

std::array<double, 3> IndividualThermostat::getMeanVelocity(ParticleContainer& container) {
    std::array<double, 3> meanVelocity = {0, 0, 0};
    for (auto& p : container)
        if (p.getActivity())
            meanVelocity =  meanVelocity + p.getV();

    meanVelocity = (1/container.activeParticleCount) * meanVelocity;
    return meanVelocity;
}
