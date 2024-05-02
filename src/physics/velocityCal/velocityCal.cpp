
#include "physics/velocityCal/velocityCal.h"
#include "simulation/planetSim.h"
#include "utils/ArrayUtils.h"

void velocity_stroemer_verlet(const Simulation& sim)
{
    // downcast if necessary
    const PlanetSimulation& sim_downcast = dynamic_cast<const PlanetSimulation&>(sim);
    for (auto& p : sim_downcast.container) {
        // v = v + Δt * (F + F_old) / (2 * m)
        p.setV(p.getV() + (sim_downcast.delta_t / (2 * p.getM())) * (p.getOldF() + p.getF()));
    }
}
