
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
    // Calculate kinetic energy
    double E = 0;
    for (auto& p : container) {
        E += p.getM() * (ArrayUtils::DotProduct(p.getV()));
    }

    // Calculate current Temperature
    double T_current = E / (container.activeParticleCount * dim);

    // Calculate new Temperature
    double diff = target - T_current;
    if (std::abs(diff) > delta)
        diff = (diff > 0) ? delta : -delta;
    double T_new = diff + T_current;

    // Update velocities with scaling factor beta
    double beta = std::sqrt(T_new / T_current);

    spdlog::debug(
        "Current Temperature: {}, Target: {}, Step target: {}, beta: {}",
        T_current,
        target,
        T_new,
        beta);

    for (auto& p : container) {
        p.setV(beta * p.getV());
    }
}
