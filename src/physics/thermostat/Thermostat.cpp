
#include "Thermostat.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <cmath>
#include <spdlog/spdlog.h>

Thermostat::Thermostat(double init, double target, double delta, size_t dim)
    : init(init)
    , target(target)
    , delta(delta)
    , dim(dim)
{
}

void Thermostat::initializeBrownianMotion(ParticleContainer& container) const
{
    for (auto& p : container) {
        p.setV(maxwellBoltzmannDistributedVelocity(std::sqrt(init / p.getM()), dim));
    }
}

void Thermostat::updateT(ParticleContainer& container) const
{
    // Calculate current Temperature
    double T_current =
        getTotalKineticEnergy(container) / (double)(container.activeParticleCount * dim);

    double beta = getBeta(T_current);

    spdlog::debug(
        "Current Temperature: {}, Target: {}, beta: {}",
        T_current,
        target,
        beta);

    for (auto& p : container) {
        p.setV(beta * p.getV());
    }
}

double Thermostat::getTotalKineticEnergy(ParticleContainer& container) const
{
    // Calculate kinetic energy
    double E = 0;
    for (auto& p : container)
        if (p.getActivity())
            E += p.getM() * (ArrayUtils::DotProduct(p.getV()));

    return E;
}

double Thermostat::getBeta(double T_current) const
{
    // Calculate new Temperature
    double diff = target - T_current;
    if (std::abs(diff) > delta)
        diff = (diff > 0) ? delta : -delta;
    double T_new = diff + T_current;

    // calculate scaling factor beta
    return std::sqrt(T_new / T_current);
}

std::string Thermostat::getName() const
{
    return "Thermostat";
}
