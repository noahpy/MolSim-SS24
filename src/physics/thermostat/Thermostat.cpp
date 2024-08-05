
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

void Thermostat::initializeBrownianMotion(Simulation& sim) const
{
    for (auto& p : sim.container)
        if (p.getIsNotStationary())
            p.setV(maxwellBoltzmannDistributedVelocity(std::sqrt(init / p.getM()), dim));
}

void Thermostat::updateT(Simulation& sim)
{
    // Calculate current Temperature
    double T_current =
        getTotalKineticEnergy(sim) / (double)(sim.container.activeParticleCount * dim);

    double beta = getBeta(T_current);

    spdlog::debug("Current Temperature: {}, Target: {}, beta: {}", T_current, target, beta);

    for (auto& p : sim.container) {
        p.setV(beta * p.getV());
    }
}

double Thermostat::getTotalKineticEnergy(Simulation& sim)
{
    // Calculate kinetic energy
    double E = 0;
    for (auto& p : sim.container)
        E += p.getM() * (ArrayUtils::DotProduct(p.getV()));

    return E;
}

double Thermostat::getBeta(double T_current) const
{
    // if T_current is 0, then beta would become nan -> to continue simulation, return 1
    if (T_current == 0)
        return 1;

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
