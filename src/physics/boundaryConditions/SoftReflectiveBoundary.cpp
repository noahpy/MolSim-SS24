
#include "SoftReflectiveBoundary.h"
#include "simulation/LennardJonesDomainSimulation.h"
#include "utils/ArrayUtils.h"
#include <iostream>

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
                // as it previously left it
                pos = pos + diff * normal;
                particle.get().setX(pos);
            }
        }
    }
}

/**
 * @brief Get the dimension that is relevant to the boundary
 * @details E.g. if the point on the boundary is (1,2,3) and the normal vector to that plane is
 * (-1,0,0) (LEFT-Boundary), then return 0 as that is the index of the coordinate of interest
 * @param normal The normal vector to the plane
 * @return The index of the relevant dimension
 */
size_t getRelevantDimension(const std::array<double, 3>& normal)
{
    size_t relevantDimension = 0;
    for (size_t i = 0; i < 3; i++)
        if (normal[i] != 0) {
            relevantDimension = i;
            break;
        }
    return relevantDimension;
}

std::array<double, 3> SoftReflectiveBoundary::getPointOnBoundaryPlane(
    const LennardJonesDomainSimulation& LJDSim)
{
    std::array<double, 3> pointOnBoundaryPlane {};
    switch (position) {
    // To mirror point, we need a point on the plane to mirror it about
    case BOTTOM:
    case LEFT:
    case BACK:
        pointOnBoundaryPlane = LJDSim.getGrid().domainOrigin;
        break;
    case TOP:
    case RIGHT:
    case FRONT:
        pointOnBoundaryPlane = LJDSim.getGrid().domainEnd;
        break;
    }

    return pointOnBoundaryPlane;
}
