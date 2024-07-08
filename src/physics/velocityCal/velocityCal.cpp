
#include "physics/velocityCal/velocityCal.h"
#include "simulation/planetSim.h"
#include "utils/ArrayUtils.h"

void velocity_stroemer_verlet(const Simulation& sim)
{
#pragma omp parallel for
    for (auto& p : sim.container) {
        if (p.getIsNotStationary())
            // v = v + Δt * (F + F_old) / (2 * m)
            p.setV(p.getV() + (sim.delta_t / (2 * p.getM())) * (p.getOldF() + p.getF()));
    }
}
