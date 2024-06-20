
#include "OverflowBoundary.h"
#include "simulation/LennardJonesDomainSimulation.h"
#include <iostream>

void OverflowBoundary::preUpdateBoundaryHandling(Simulation& simulation)
{
    const LennardJonesDomainSimulation& LGDSim =
        static_cast<const LennardJonesDomainSimulation&>(simulation);

    if (LGDSim.iteration == 109) {
        std::cout << "Preupdate start(" << getPositionString(position)
                  << "): " << LGDSim.getGrid().cells[0][1][0]->getParticles().size() << std::endl;
        auto pos = LGDSim.getGrid().cells[0][1][0]->getParticles().back().get().getX();
        std::cout << "pos: " << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;
    }

    for (auto haloCellIndex : LGDSim.getGrid().haloCellIterator(position)) {
        // splice moves the pointers within the list, clearing out the particle list of the halo
        // cell
        for (auto& particle : LGDSim.getGrid()
                                  .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
                                  ->getParticles()) {
            if (particle.get().getActivity()) {
                /* spdlog::info("Deleted in Outflow {}", particle.get().toString()); */
                simulation.container.removeParticle(particle.get());
            }
        }
        LGDSim.getGrid()
            .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
            ->clearParticles();
    }
}
