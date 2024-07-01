
#include "WallBoundary.h"

WallBoundary::WallBoundary(Position p_position, const CellGrid& cellGrid, size_t thickness, const Particle& particle)
    : BoundaryCondition(p_position)
    , cellGrid(cellGrid)
{
}

void WallBoundary::preUpdateBoundaryHandling(Simulation& simulation)
{
    for (CellIndex haloCellIndex : cellGrid.haloCellIterator(position)) {
        cellGrid.cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]->setParticles(particles[haloCellIndex]);
    }
}

void WallBoundary::postUpdateBoundaryHandling(Simulation& simulation)
{
    for (CellIndex haloCellIndex : cellGrid.haloCellIterator(position)) {
            cellGrid.cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]->clearParticles();
    }
}
