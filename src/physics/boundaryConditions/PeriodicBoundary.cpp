
#include "PeriodicBoundary.h"
#include "simulation/LennardJonesDomainSimulation.h"
#include "utils/ArrayUtils.h"

PeriodicBoundary::PeriodicBoundary(
    Position position, const BoundaryConfig& boundaryConfig, const CellGrid& cellGrid)
    : BoundaryCondition(position)
    , insertionIndex(0)
    , is2D(boundaryConfig.boundaryMap.size() == 4)
    , innerTranslation(getPeriodicShift(cellGrid))
{
    for (CellIndex boundaryCellIndex : cellGrid.boundaryCellIterator(position)) {
        std::vector<Position> boundarySides =
            coordinateToPosition(boundaryCellIndex, cellGrid.getGridDimensions(), !is2D);
        // Only if the first position is the bound itself, then we want to consider it
        // -> That way we make sure to only transform cells on edges or corners once
        if (boundarySides[0] != position)
            continue;

        // Only look at cells that are affected only by periodic bounds -> otherwise the other bound
        // will hold
        for (Position boundSide : boundarySides) {
            // Bounds must be periodic
            if (boundaryConfig.boundaryMap.at(boundSide) != BoundaryType::PERIODIC)
                continue;
        }

        // Check if the translations have been generated already
        if (translationMap.find(boundarySides) != translationMap.end())
            continue;

        // Add the necessary translations
        fillTranslationMap(
            boundarySides, { boundarySides[0] }, cellGrid); // Always do the single inner one
        if (boundarySides.size() >= 2) {
            fillTranslationMap(boundarySides, { boundarySides[1] }, cellGrid);
            fillTranslationMap(boundarySides, { boundarySides[0], boundarySides[1] }, cellGrid);
        }
        if (boundarySides.size() == 3) {
            fillTranslationMap(boundarySides, { boundarySides[2] }, cellGrid);
            fillTranslationMap(boundarySides, { boundarySides[0], boundarySides[2] }, cellGrid);
            fillTranslationMap(boundarySides, { boundarySides[1], boundarySides[2] }, cellGrid);
            fillTranslationMap(boundarySides, boundarySides, cellGrid);
        }
        if (boundarySides.size() > 3) {
            spdlog::error(
                "A cell has been found to have more than 3 associated sides. This is a bug");
        }
    }
}

void PeriodicBoundary::preUpdateBoundaryHandling(Simulation& simulation)
{
    const LennardJonesDomainSimulation& LGDSim =
        static_cast<const LennardJonesDomainSimulation&>(simulation);

    // reset the insertion index, to reuse already created particles
    insertionIndex = 0;

    for (CellIndex boundaryCellIndex : LGDSim.getGrid().boundaryCellIterator(position)) {
        std::vector<Position> boundarySides = coordinateToPosition(
            boundaryCellIndex, LGDSim.getGrid().getGridDimensions(), !is2D, false);

        // Only if translations need to be applied i.e. there exists an entry
        if (translationMap.find(boundarySides) == translationMap.end())
            continue;

        for (auto& particle :
             LGDSim.getGrid()
                 .cells[boundaryCellIndex[0]][boundaryCellIndex[1]][boundaryCellIndex[2]]
                 ->getParticles()) {
            for (PeriodicBoundShifts& shifts : translationMap.at(boundarySides)) {
                std::array<double, 3> pos = particle.get().getX();
                std::array<double, 3> haloPosition = pos + shifts.first;

                // Ugly but we cannot simply add as size_t != int
                CellIndex haloCellIndex { (size_t)((int)boundaryCellIndex[0] + shifts.second[0]),
                                          (size_t)((int)boundaryCellIndex[1] + shifts.second[1]),
                                          (size_t)((int)boundaryCellIndex[2] + shifts.second[2]) };

                if (insertionIndex < insertedParticles.size()) {
                    // simply update, if there is already a particle to reuse
                    insertedParticles[insertionIndex].setX(haloPosition);
                    insertedParticles[insertionIndex].setV({ 0, 0, 0 });
                    insertedParticles[insertionIndex].setF({ 0, 0, 0 });
                    insertedParticles[insertionIndex].setOldF({ 0, 0, 0 });
                    insertedParticles[insertionIndex].setM(particle.get().getM());
                } else {
                    // create a new particle
                    Particle haloParticle(haloPosition, { 0, 0, 0 }, particle.get().getM());
                    insertedParticles.push_back(haloParticle);
                }

                // add the particle to the halo cell
                LGDSim.getGrid()
                    .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
                    ->addParticle(insertedParticles[insertionIndex++]);
            }
        }
    }

    // erase all leftover particles, if there are any
    if (insertionIndex < insertedParticles.size()) {
        insertedParticles.erase(
            insertedParticles.begin() + (long)insertionIndex, insertedParticles.end());
    }
}

void PeriodicBoundary::postUpdateBoundaryHandling(Simulation& simulation)
{
    const LennardJonesDomainSimulation& LGDSim =
        static_cast<const LennardJonesDomainSimulation&>(simulation);

    // reset old halo references used previously
    for (auto haloCellIndex : LGDSim.getGrid().haloCellIterator(position)) {
        LGDSim.getGrid()
            .cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]
            ->clearParticles();
    }

    for (auto boundaryCellIndex : LGDSim.getGrid().boundaryCellIterator(position)) {
        ParticleRefList& particles =
            LGDSim.getGrid()
                .cells[boundaryCellIndex[0]][boundaryCellIndex[1]][boundaryCellIndex[2]]
                ->getParticles();
        auto it = particles.begin();
        auto end = particles.end();
        while (it != end) {
            Particle& particle = *it;
            std::array<double, 3> pos = particle.getX();

            CellIndex actualCellIndex = LGDSim.getGrid().getIndexFromPos(pos);
            CellType actualCellType = LGDSim.getGrid().determineCellType(actualCellIndex);

            // Check if the particles has entered Halo-territory
            if (actualCellType == CellType::Halo) {
                std::vector<Position> actualSides = coordinateToPosition(
                    actualCellIndex, LGDSim.getGrid().getGridDimensions(), !is2D, true);

                // if that halo cell is also part of the side of the boundary -> e.g. edges in 2D ->
                // Bounds are both sides, halo only one of them
                if (std::find(actualSides.begin(), actualSides.end(), position) ==
                    actualSides.end()) {
                    ++it;
                    continue;
                }
                // When we move the particle by the translation, it will only end up inside the
                // domain iff all periodic bounds around it were applied
                // The update cells will take care of assigning it to its new cell
                // We can do that, as the particle would have left its original cell and thus would
                // no longer be included in any calculations of the new cell. Same for moving it
                std::array<double, 3> newPosition = particle.getX();
                newPosition = newPosition + innerTranslation.first;
                particle.setX(newPosition);

                // Only if the particle has been moved back into the domain, all translations are
                // complete. Thus, the particles must be placed into its new cell
                actualCellIndex = LGDSim.getGrid().getIndexFromPos(newPosition);
                actualCellType = LGDSim.getGrid().determineCellType(actualCellIndex);
                if (actualCellType != CellType::Halo) {
                    // It was in halo before, now it is back inside the domain
                    it = particles.erase(it); // remove it from its old cell (boundary)
                    LGDSim.getGrid()
                        .cells.at(actualCellIndex[0])
                        .at(actualCellIndex[1])
                        .at(actualCellIndex[2])
                        ->addParticle(particle); // into its new cell (inside domain)

                    continue; // to not increment the iterator later again
                }
            }

            ++it;
        }
    }
}

void PeriodicBoundary::fillTranslationMap(
    const std::vector<Position>& key,
    const std::vector<Position>& boundarySides,
    const CellGrid& cellGrid)
{
    PeriodicBoundShifts shiftToAdd = { {}, {} };
    for (Position side : boundarySides) {
        PeriodicBoundShifts addedShift = getPeriodicShiftFromPosition(side, cellGrid);
        shiftToAdd.first = shiftToAdd.first + addedShift.first;
        shiftToAdd.second = shiftToAdd.second + addedShift.second;
    }
    translationMap[key].push_back(shiftToAdd);
}

PeriodicBoundShifts PeriodicBoundary::getPeriodicShiftFromPosition(
    Position pos, const CellGrid& grid)
{
    // Used for the translational math
    std::array<double, 3> normal = getNormalVectorOfBoundary(pos);
    size_t relevantDimension = getRelevantDimension(normal);
    std::array<int, 3> normalNat = { 0, 0, 0 };
    normalNat[relevantDimension] = (int)normal[relevantDimension];

    // To get the positional shift to move particles from the halo cells to the opposite boundary
    // cells, we can use the domain size.
    // The direction to move the particles in is the same, as the direction the normal vector is
    // pointing. Thus multiplying the grid dimensions with the direction of the normal vector will
    // yield the desired shift
    std::array<double, 3> positionalShift = grid.getDomainSize()[relevantDimension] * normal;
    // -1 for num to index
    // -1 for boundary to halo
    std::array<int, 3> cellIndexShift =
        (grid.getGridDimensions()[relevantDimension] - 1 - 1) * normalNat;

    return { positionalShift, cellIndexShift };
}
