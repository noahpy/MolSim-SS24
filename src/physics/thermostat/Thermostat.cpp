
#include "Thermostat.h"
#include <valarray>
#include "../../utils/MaxwellBoltzmannDistribution.h"
#include "../../utils/ArrayUtils.h"

Thermostat::Thermostat(double init, double target, double delta, size_t dim)
    : init(init), target(target), delta(delta), dim(dim)
{
}

void Thermostat::initializeBrownianMotion(const Simulation& sim) const
{
    for (auto& p: sim.container) {
        p.setV(maxwellBoltzmannDistributedVelocity(std::sqrt(init / p.getM()), dim));
    }
}


void Thermostat::updateT(const Simulation& sim) const
{
    // Calculate kinetic energy
    double E = 0;
    for (auto& p : sim.container) {
        E += (p.getM() * (ArrayUtils::DotProduct(p.getV()))) / 2;
    }

    // Calculate current Temperature
    double T_current = (2 * E) / (static_cast<double>(sim.container.particles.size()) * static_cast<double>(dim));

    // Calculate new Temperature
    double T_new = target - T_current;
    if (std::abs(T_new) > delta) T_new = (T_new > 0) ? delta : -delta;
    T_new += T_current;

    // Update velocities with scaling factor beta
    double beta = std::sqrt(T_new / T_current);
    for (auto& p : sim.container) {
        p.setV(beta * p.getV());
    }
}
