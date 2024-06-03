
#include "SoftReflectiveBoundary.h"
#include "utils/ArrayUtils.h"

void SoftReflectiveBoundary::preUpdateBoundaryHandling(Simulation& simulation)
{
    // reset the insertion index, to reuse already created particles
    insertionIndex = 0;
    // reset old halo references used previously
    clearAllHaloParticleReferences();

    const LennardJonesDomainSimulation& LGDSim =
        static_cast<const LennardJonesDomainSimulation&>(simulation);

    for (auto boundaryCellIndex : LGDSim.domain.boundaryCellIterator(position)) {
        for (auto& particle :
             LGDSim.domain.cells[boundaryCellIndex[0]][boundaryCellIndex[1]][boundaryCellIndex[2]]
                 ->getParticles()) {
            std::array<double, 3> pos = particle.get().getX();
            std::array<double, 3> pointOnBoundaryPlane {};
            switch (position) {
            // To mirror point, we need a point on the plane to mirror it about
            case TOP:
            case LEFT:
            case FRONT:
                pointOnBoundaryPlane = LGDSim.domain.domainOrigin;
                break;
            case BOTTOM:
            case RIGHT:
            case BACK:
                pointOnBoundaryPlane = LGDSim.domain.domainEnd;
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
                LGDSim.domain
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
            LGDSim.domain.cells[boundaryCellIndex[0]][boundaryCellIndex[1]][boundaryCellIndex[2]]
                ->addParticle(insertedParticles[insertionIndex++]);
        }
    }

    // erase all leftover particles, if there are any
    if (insertionIndex < insertedParticles.size()) {
        insertedParticles.erase(
            insertedParticles.begin() + (long)insertionIndex, insertedParticles.end());
    }
}