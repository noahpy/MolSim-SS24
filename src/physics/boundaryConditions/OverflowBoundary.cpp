
#include "OverflowBoundary.h"
#include "models/linked_cell/cell/Cell.h"
#include "simulation/LennardJonesDomainSimulation.h"
#include <iostream>
#include <unordered_map>

void OverflowBoundary::postUpdateBoundaryHandling(Simulation& simulation)
{
    const LennardJonesDomainSimulation& LGDSim =
        static_cast<const LennardJonesDomainSimulation&>(simulation);

    // merge all particles that are in halo cells to remove them
    ParticleRefList particlesToRemove {};
    for (auto haloCellIndex : LGDSim.getGrid().haloCellIterator(position)) {
        // splice moves the pointers within the list, clearing out the particle list of the halo
        // cell
        particlesToRemove.splice(
            particlesToRemove.end(),
            LGDSim.getGrid()
                .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
                ->getParticles());
    }

    // create a map to check in O(1) if a particle is in the list
    std::unordered_map<Particle*, bool> particlesToRemoveMap {};
    for (const auto& particle : particlesToRemove) {
        // particlesToRemoveMap.insert({ &particle.get(), true });
        particle.get().setV({ 0, 0, 0 });
        particle.get().setF({ 0, 0, 0 });
        particle.get().setOldF({ 0, 0, 0 });
        particle.get().setActivity(false);
    }

    /*
     * Problem:
     * If we remove the particles form the particle containter, the references in the cell will be
     * invalid thus the iterator will be invalid that is used in the force calculations.
     * We need to change the way the particle is deleted, or we need to change the way, the
     * grid is updated
     */

    /*

    // Delete the particles, that have left the domain
    if (particlesToRemoveMap.size() > 0)
        std::cout << LGDSim.container.particles.size() << std::endl;
    LGDSim.container.removeParticles(particlesToRemoveMap);
    if (particlesToRemoveMap.size() > 0)
        std::cout << LGDSim.container.particles.size() << std::endl;

    for (auto haloCellIndex : LGDSim.getGrid().haloCellIterator(position)) {
        if (LGDSim.getGrid()
                .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
                ->getParticles()
                .size() > 0) {
            int a = 0;
        }
    }
     */
}
