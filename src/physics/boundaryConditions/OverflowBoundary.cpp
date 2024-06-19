
#include "OverflowBoundary.h"
#include "simulation/LennardJonesDomainSimulation.h"

void OverflowBoundary::preUpdateBoundaryHandling(Simulation& simulation)
{
    const LennardJonesDomainSimulation& LGDSim =
        static_cast<const LennardJonesDomainSimulation&>(simulation);

    for (auto haloCellIndex : LGDSim.getGrid().haloCellIterator(position)) {
        // splice moves the pointers within the list, clearing out the particle list of the halo
        // cell
        for (auto& particle : LGDSim.getGrid()
                                  .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
                                  ->getParticles()) {
            if (particle.get().getActivity()) {
                spdlog::info("Deleted in Outflow {}", particle.get().toString());
                simulation.container.removeParticle(particle.get());
            }
        }
        LGDSim.getGrid()
            .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
            ->clearParticles();
    }
}
