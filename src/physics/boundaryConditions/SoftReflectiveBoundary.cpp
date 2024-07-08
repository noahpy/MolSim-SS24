
#include "SoftReflectiveBoundary.h"
#include "simulation/LennardJonesDomainSimulation.h"
#include "utils/ArrayUtils.h"

// forward declare
size_t getRelevantDimension(const std::array<double, 3>& normal);

void SoftReflectiveBoundary::preUpdateBoundaryHandling(Simulation& simulation)
{
    /*
     * Note the particles will always be in the boundary cells, as the post update moves any that
     * leave the domain back in to it
     */
    const LennardJonesDomainSimulation& LGDSim =
        static_cast<const LennardJonesDomainSimulation&>(simulation);

    // reset the insertion index, to reuse already created particles
    insertionIndex = 0;

    // Get the position of the halo cell by mirroring the particle about the boundary
    std::array<double, 3> normal = getNormalVectorOfBoundary(position);
    for (auto boundaryCellIndex : LGDSim.getGrid().boundaryCellIterator(position)) {
        for (auto& particle :
             LGDSim.getGrid()
                 .cells[boundaryCellIndex[0]][boundaryCellIndex[1]][boundaryCellIndex[2]]
                 ->getParticles()) {
            std::array<double, 3> pos = particle.get().getX();
            std::array<double, 3> pointOnBoundaryPlane = getPointOnBoundaryPlane(LGDSim);

            std::array<double, 3> diff = pointOnBoundaryPlane - pos;
            double dotProd =
                std::inner_product(std::begin(diff), std::end(diff), std::begin(normal), 0.0);
            std::array<double, 3> haloPosition = pos + dotProd * 2 * normal;

            if (ArrayUtils::L2Norm(haloPosition - pos) >
                LGDSim.getRepulsiveDistance(particle.get().getType())) {
                // The particle is too far away from the boundary, so we don't need to create a halo
                // particle -> otherwise, it would attract
                continue;
            }

            // Get the corresponding halo cell
            CellIndex neighboringHaloCellIndex = filterHaloNeighbors(
                LGDSim.getGrid()
                    .cells[boundaryCellIndex[0]][boundaryCellIndex[1]][boundaryCellIndex[2]]
                    ->haloNeighbours);

            if (insertionIndex < insertedParticles.size()) {
                // simply update, if there is already a particle to reuse
                insertedParticles[insertionIndex]->setX(haloPosition);
                insertedParticles[insertionIndex]->setV({ 0, 0, 0 });
                insertedParticles[insertionIndex]->setF({ 0, 0, 0 });
                insertedParticles[insertionIndex]->setOldF({ 0, 0, 0 });
                insertedParticles[insertionIndex]->setM(particle.get().getM());
            } else {
                // create a new particle
                Particle haloParticle(
                    haloPosition, { 0, 0, 0 }, particle.get().getM(), particle.get().getType(), false);
                haloParticle.setActivity(false);
                insertedParticles.push_back(std::make_unique<Particle>(haloParticle));
            }

            // add the particle to the halo cell
            LGDSim.getGrid()
                .cells[neighboringHaloCellIndex[0]][neighboringHaloCellIndex[1]]
                      [neighboringHaloCellIndex[2]]
                ->addParticle(*insertedParticles[insertionIndex++].get());
        }
    }

    // erase all leftover particles, if there are any
    if (insertionIndex < insertedParticles.size()) {
        insertedParticles.erase(
            insertedParticles.begin() + (long)insertionIndex, insertedParticles.end());
    }
}

void SoftReflectiveBoundary::postUpdateBoundaryHandling(Simulation& simulation)
{
    const LennardJonesDomainSimulation& LGDSim =
        static_cast<const LennardJonesDomainSimulation&>(simulation);

    // reset old halo references used previously -> needed here, as halo particles could have
    // crossed into the domain
    for (auto haloCellIndex : LGDSim.getGrid().haloCellIterator(position)) {
        LGDSim.getGrid()
            .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
            ->clearParticles();
    }

    // Get the position of the halo cell by mirroring the particle about the boundary
    std::array<double, 3> normal = getNormalVectorOfBoundary(position);

    for (auto boundaryCellIndex : LGDSim.getGrid().boundaryCellIterator(position)) {
        for (auto& particle :
             LGDSim.getGrid()
                 .cells[boundaryCellIndex[0]][boundaryCellIndex[1]][boundaryCellIndex[2]]
                 ->getParticles()) {
            std::array<double, 3> pos = particle.get().getX();
            std::array<double, 3> pointOnBoundaryPlane = getPointOnBoundaryPlane(LGDSim);

            // We need to check, if the particle has moved beyond the boundary. If so, set it back
            // into the domain
            size_t relevantDimension = getRelevantDimension(normal);
            /* pos - pointOnBoundaryPlane -> vector from pointOnBoundaryPlane to pos
             * This vector should point inward i.e. towards the center, as long as it is inside the
             * domain The normal vector is also always pointing inward Therefore the product will
             * only be greater than 0, if the particle is inside the domain (same sign)
             */
            double diff = (pos - pointOnBoundaryPlane)[relevantDimension];
            bool isInsideDomain = diff * normal[relevantDimension] > 0;
            if (!isInsideDomain) {
                // set the position back into the domain
                pos[relevantDimension] = pointOnBoundaryPlane[relevantDimension];
                // normal is always pointing inward, thus set the particle as far into the boundary,
                // as it previously left it. Diff must be made abs to make sure it is place towards
                // the right direction
                pos = pos + std::abs(diff) * normal;
                particle.get().setX(pos);
            }
        }
    }
}
