
#include "physics/locationCal/locationCal.h"
#include "utils/ArrayUtils.h"

void location_stroemer_verlet(const Simulation& sim)
{
#pragma omp parallel for
    for (auto& p : sim.container) {
        if (p.getIsNotStationary())
            // x = x + Δt * v + (Δt)^2 * F / (2 * m)
            p.setX(
                p.getX() + sim.delta_t * p.getV() +
                (sim.delta_t * sim.delta_t / (2 * p.getM())) * p.getF());
    }
}
