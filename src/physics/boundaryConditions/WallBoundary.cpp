
#include "WallBoundary.h"
#include <spdlog/spdlog.h>

WallBoundary::WallBoundary(Position p_position, const CellGrid& cellGrid)
    : BoundaryCondition(p_position)
    , cellGrid(cellGrid)
{
}

void WallBoundary::init(size_t thickness, const Particle& particle)
{
    isInitialized = true;
    // TODO create wall and store in particles
}

void WallBoundary::preUpdateBoundaryHandling(Simulation& simulation)
{
    checkInitialized();
    for (CellIndex haloCellIndex : cellGrid.haloCellIterator(position)) {
        cellGrid.cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]->setParticles(particles[haloCellIndex]);
    }
}

void WallBoundary::postUpdateBoundaryHandling(Simulation& simulation)
{
    checkInitialized();
    for (CellIndex haloCellIndex : cellGrid.haloCellIterator(position)) {
            cellGrid.cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]->clearParticles();
    }
}

void WallBoundary::checkInitialized() const
{
    if (!isInitialized) {
        spdlog::error("WallBoundary at {} is not initialized", position);
        exit(EXIT_FAILURE);
    }
}
