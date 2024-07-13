
#include "physics/velocityCal/velocityCal.h"
#include "simulation/planetSim.h"
#include "utils/ArrayUtils.h"
#include <spdlog/spdlog.h>

void velocity_stroemer_verlet(const Simulation& sim)
{
    #pragma omp parallel for
    for (auto& p : sim.container) {
        if (p.getIsNotStationary()) {
            // v = v + Δt * (F + F_old) / (2 * m)
            auto tmp = p.getV() + (sim.delta_t / (2 * p.getM())) * (p.getOldF() + p.getF());
            p.setV(tmp);
        }
    }
}
