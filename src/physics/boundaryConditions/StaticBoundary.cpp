
#include "StaticBoundary.h"
#include <spdlog/spdlog.h>

StaticBoundary::StaticBoundary(Position p_position, const CellGrid& cellGrid)
    : BoundaryCondition(p_position)
    , cellGrid(cellGrid)
{
}

void StaticBoundary::init()
{
    isInitialized = true;
    // TODO create wall and store in particles
    // Idea: Take partilces from the halo cell to the position of the wall
    // Store the ref list
    // That will then be reused. But! Keep overflow in mind
}

void StaticBoundary::preUpdateBoundaryHandling(Simulation& simulation)
{
    checkInitialized();
    for (CellIndex haloCellIndex : cellGrid.haloCellIterator(position)) {
        cellGrid.cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]->setParticles(particles[haloCellIndex]);
    }
}

void StaticBoundary::postUpdateBoundaryHandling(Simulation& simulation)
{
    checkInitialized();
    for (CellIndex haloCellIndex : cellGrid.haloCellIterator(position)) {
            cellGrid.cells[haloCellIndex[0]][haloCellIndex[1]][haloCellIndex[2]]->clearParticles();
    }
}

void StaticBoundary::checkInitialized() const
{
    if (!isInitialized) {
        spdlog::error("StaticBoundary at {} is not initialized", position);
        exit(EXIT_FAILURE);
    }
}
