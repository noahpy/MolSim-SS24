
#include "OverflowBoundary.h"
#include "models/linked_cell/cell/Cell.h"
#include "simulation/LennardJonesDomainSimulation.h"

void OverflowBoundary::preUpdateBoundaryHandling(Simulation& simulation)
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

    for (const auto& particle : particlesToRemove) {
        spdlog::info("Deleted {}", particle.get().toString());
        simulation.container.removeParticle(particle);
    }
}
