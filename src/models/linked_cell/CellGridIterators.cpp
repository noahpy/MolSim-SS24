
#include "CellGrid.h"
#include "models/linked_cell/cell/Cell.h"

// TODO Move the creation the iterators to the class and then only return the fresh ones
/* ########### BoundaryIterator Implementation ########### */
CellGrid::BoundaryIterator::BoundaryIterator(
    Position position, std::array<size_t, 3> gridDimensions, bool end)
    : gridDimensions(gridDimensions)
    , index(0)
{
    std::pair<size_t, size_t> relevantBoundaryIndices;
    size_t irrelevantBoundary;
    switch (position) {
    case LEFT:
    case RIGHT:
        relevantBoundaryIndices = { 1, 2 };
        irrelevantBoundary = 0;
        break;
    case TOP:
    case BOTTOM:
        relevantBoundaryIndices = { 0, 2 };
        irrelevantBoundary = 1;
        break;
    case FRONT:
    case BACK:
        relevantBoundaryIndices = { 0, 1 };
        irrelevantBoundary = 2;
        break;
    }

    size_t coordinateIrrelevantAxis;
    switch (position) {
    case LEFT:
    case TOP:
    case FRONT:
        coordinateIrrelevantAxis = 1;
        break;
    case RIGHT:
    case BOTTOM:
    case BACK:
        coordinateIrrelevantAxis = gridDimensions[irrelevantBoundary] - 2;
        break;
    }

    std::vector<CellIndex> relevantBoundaries(
        gridDimensions[relevantBoundaryIndices.first] *
        gridDimensions[relevantBoundaryIndices.second]);
    size_t insertionIndex = 0;

    for (size_t i = 1; i < gridDimensions[relevantBoundaryIndices.first] - 1; i++)
        for (size_t j = 1; j < gridDimensions[relevantBoundaryIndices.second] - 1; j++) {
            CellIndex boundary;
            if (irrelevantBoundary == 0)
                boundary = { coordinateIrrelevantAxis, i, j };
            else if (irrelevantBoundary == 1)
                boundary = { i, coordinateIrrelevantAxis, j };
            else
                boundary = { i, j, coordinateIrrelevantAxis };

            relevantBoundaries.at(insertionIndex++) = boundary;
        }
}

CellGrid::BoundaryIterator CellGrid::BoundaryIterator::begin()
{
    BoundaryIterator startIterator = *this;
    startIterator.index = 0;
    return startIterator;
}

CellGrid::BoundaryIterator CellGrid::BoundaryIterator::end()
{
    BoundaryIterator endIterator = *this;
    endIterator.index = boundaries.size();
    return endIterator;
}

CellIndex CellGrid::BoundaryIterator::operator*() const
{
    if (index < boundaries.size())
        return boundaries.at(index);
    return { 0, 0, 0 };
}

CellGrid::BoundaryIterator& CellGrid::BoundaryIterator::operator++()
{
    if (index < boundaries.size())
        ++index;
    return *this;
}

bool CellGrid::BoundaryIterator::operator!=(const BoundaryIterator& other) const
{
    return this->index != other.index;
}

/* ########### HaloIterator Implementation ########### */
CellGrid::HaloIterator::HaloIterator(std::vector<CellIndex>& halos, Position position, bool end)
    : halos(halos)
{
    if (end) {
        index = halos.size();
        return;
    }
    index = 0;
}

CellIndex CellGrid::HaloIterator::operator*() const
{
    if (index < halos.size())
        return halos.at(index);
    return { 0, 0, 0 };
}

CellGrid::HaloIterator& CellGrid::HaloIterator::operator++()
{
    if (index < halos.size())
        ++index;
    return *this;
}

bool CellGrid::HaloIterator::operator!=(const HaloIterator& other) const
{
    return this->index != other.index;
}
