
#include "SoftReflectiveBoundary.h"
#include "simulation/LennardJonesDomainSimulation.h"
#include "utils/ArrayUtils.h"

void SoftReflectiveBoundary::preUpdateBoundaryHandling(Simulation& simulation)
{
    const LennardJonesDomainSimulation& LGDSim =
        static_cast<const LennardJonesDomainSimulation&>(simulation);

    // reset the insertion index, to reuse already created particles
    insertionIndex = 0;
    // reset old halo references used previously
    for (auto haloCellIndex : LGDSim.getGrid().haloCellIterator(position))
        LGDSim.getGrid()
            .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
            ->clearParticles();

    for (auto boundaryCellIndex : LGDSim.getGrid().boundaryCellIterator(position)) {
        for (auto& particle :
             LGDSim.getGrid()
                 .cells[boundaryCellIndex[0]][boundaryCellIndex[1]][boundaryCellIndex[2]]
                 ->getParticles()) {
            std::array<double, 3> pos = particle.get().getX();
            std::array<double, 3> pointOnBoundaryPlane {};
            switch (position) {
            // To mirror point, we need a point on the plane to mirror it about
            case BOTTOM:
            case LEFT:
            case BACK:
                pointOnBoundaryPlane = LGDSim.getGrid().domainOrigin;
                break;
            case TOP:
            case RIGHT:
            case FRONT:
                pointOnBoundaryPlane = LGDSim.getGrid().domainEnd;
                break;
            }

            // Get the position of the halo cell by mirroring the particle about the boundary
            std::array<double, 3> normal = getNormalVectorOfBoundary(position);
            std::array<double, 3> diff = pointOnBoundaryPlane - pos;
            double dotProd =
                std::inner_product(std::begin(diff), std::end(diff), std::begin(normal), 0.0);
            std::array<double, 3> haloPosition = pos + dotProd * 2 * normal;

            // Get the corresponding halo cell
            CellIndex neighboringHaloCellIndex = filterHaloNeighbors(
                LGDSim.getGrid()
                    .cells[boundaryCellIndex[0]][boundaryCellIndex[1]][boundaryCellIndex[2]]
                    ->haloNeighbours);

            if (insertionIndex < insertedParticles.size()) {
                // simply update, if there is already a particle to reuse
                insertedParticles[insertionIndex].setX(haloPosition);
                insertedParticles[insertionIndex].setV({ 0, 0, 0 });
                insertedParticles[insertionIndex].setF({ 0, 0, 0 });
                insertedParticles[insertionIndex].setM(particle.get().getM());
            } else {
                // create a new particle
                Particle haloParticle = Particle(haloPosition, { 0, 0, 0 }, particle.get().getM());
                insertedParticles.push_back(haloParticle);
            }

            // add the particle to the halo cell
            LGDSim.getGrid()
                .cells[neighboringHaloCellIndex[0]][neighboringHaloCellIndex[1]]
                      [neighboringHaloCellIndex[2]]
                ->addParticle(insertedParticles[insertionIndex++]);
        }
    }

    // erase all leftover particles, if there are any
    if (insertionIndex < insertedParticles.size()) {
        insertedParticles.erase(
            insertedParticles.begin() + (long)insertionIndex, insertedParticles.end());
    }
}