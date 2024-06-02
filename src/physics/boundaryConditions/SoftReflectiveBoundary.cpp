
#include "SoftReflectiveBoundary.h"
#include "simulation/LennardJonesDomainSimulation.h"
#include "utils/ArrayUtils.h"

void SoftReflectiveBoundary::preUpdateBoundaryHandling(Simulation& simulation)
{
    const LennardJonesDomainSimulation& LGDSim = static_cast<const LennardJonesDomainSimulation&>(simulation);

    // Rather I have a beginBoundaryParticles(position) that only gives the ones relevant
    // Maybe even directly the cells by iterating over the cells matrix
    for (auto it = LGDSim.domain.beginBoundaryParticles(); it != LGDSim.domain.endBoundaryParticles(); ++it) {
        auto boundaryCellIndex = *it;
        bool isRelevant = false;
        for (Position cellPosition : LGDSim.domain.cells[boundaryCellIndex[0]][boundaryCellIndex[1]][boundaryCellIndex[2]]->boundarySides) {
            if (cellPosition == position) {
                isRelevant = true;
                break;
            }
        }
        if (!isRelevant) continue; // Skip boundary cell if it does not belong to the boundary

        // How to I loop efficiently?
        for (auto& particle : LGDSim.domain.cells[boundaryCellIndex[0]][boundaryCellIndex[1]][boundaryCellIndex[2]]->getParticles()) {
            std::array<double, 3> pos = particle.get().getX();
            std::array<double, 3> pointOnBoundary = {1,2,3}; // TODO: Get a point on the boundary. Is this saved in the cell maybe?
            std::array<double, 3> normal = getNormalVectorOfBoundary(position);

            std::array<double, 3> diff = pointOnBoundary - pos;
            double dotProd = std::inner_product(std::begin(diff), std::end(diff), std::begin(normal), 0.0);
            std::array<double, 3> haloPosition = pos + dotProd*2*normal;

            // Now update the force of the particle with one step using the halo particle only
            // But this would be overwritten by the force calculation in the next step
            // I do not need to add the particle itself to the halo cells

            // For overflow, how do I remove the particle?
        }
    }
}