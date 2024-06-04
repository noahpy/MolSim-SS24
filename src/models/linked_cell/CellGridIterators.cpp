
#include "CellGrid.h"
#include "models/linked_cell/cell/Cell.h"
#include <iostream>

// forward declare
std::pair<size_t, std::pair<size_t, size_t>> getBoundaryIndices(Position position);
size_t getCoordinateIrrelevantAxis(
    Position position, std::array<size_t, 3> gridDimensions, size_t irrelevantBoundary, CellType cellType);
size_t getNumberOfRelevantBoundaries(size_t dim1, size_t dim2, size_t dimZ);

// TODO Move the creation the iterators to the class and then only return the fresh ones
/* ########### BoundaryIterator Implementation ########### */
CellGrid::BoundaryIterator::BoundaryIterator(
    Position position, std::array<size_t, 3> gridDimensions, bool end)
    : gridDimensions(gridDimensions)
    , index(0)
{
    size_t irrelevantBoundary = getBoundaryIndices(position).first;
    std::pair<size_t, size_t> relevantBoundaryIndices = getBoundaryIndices(position).second;

    size_t coordinateIrrelevantAxis = getCoordinateIrrelevantAxis(
        position, gridDimensions, irrelevantBoundary, CellType::Boundary);

    // Make space for the required indices
    std::vector<CellIndex> relevantBoundaries(getNumberOfRelevantBoundaries(
        (gridDimensions[relevantBoundaryIndices.first] - 2),
        (gridDimensions[relevantBoundaryIndices.second] - 2),
        gridDimensions[2]));
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

    boundaries = relevantBoundaries;
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
CellGrid::HaloIterator::HaloIterator(
    Position position, std::array<size_t, 3> gridDimensions, bool end)
    : gridDimensions(gridDimensions)
    , index(0)
{
    size_t irrelevantBoundary = getBoundaryIndices(position).first;
    std::pair<size_t, size_t> relevantBoundaryIndices = getBoundaryIndices(position).second;

    size_t coordinateIrrelevantAxis =
        getCoordinateIrrelevantAxis(position, gridDimensions, irrelevantBoundary, CellType::Halo);

    std::vector<CellIndex> relevantBoundaries(getNumberOfRelevantBoundaries(
        gridDimensions[relevantBoundaryIndices.first],
        gridDimensions[relevantBoundaryIndices.second],
        gridDimensions[2]));
    size_t insertionIndex = 0;

    for (size_t i = 0; i < gridDimensions[relevantBoundaryIndices.first]; i++)
        for (size_t j = 0; j < gridDimensions[relevantBoundaryIndices.second]; j++) {
            CellIndex boundary;
            if (irrelevantBoundary == 0)
                boundary = { coordinateIrrelevantAxis, i, j };
            else if (irrelevantBoundary == 1)
                boundary = { i, coordinateIrrelevantAxis, j };
            else
                boundary = { i, j, coordinateIrrelevantAxis };

            relevantBoundaries.at(insertionIndex++) = boundary;
        }

    boundaries = relevantBoundaries;

    // TODO check in 2D case
    //std::cout << "Correct Number (Halo) " << (insertionIndex == relevantBoundaries.size()) << std::endl;
}

CellGrid::HaloIterator CellGrid::HaloIterator::begin()
{
    HaloIterator startIterator = *this;
    startIterator.index = 0;
    return startIterator;
}

CellGrid::HaloIterator CellGrid::HaloIterator::end()
{
    HaloIterator endIterator = *this;
    endIterator.index = boundaries.size();
    return endIterator;
}

CellIndex CellGrid::HaloIterator::operator*() const
{
    if (index < boundaries.size())
        return boundaries.at(index);
    return { 0, 0, 0 };
}

CellGrid::HaloIterator& CellGrid::HaloIterator::operator++()
{
    if (index < boundaries.size())
        ++index;
    return *this;
}

bool CellGrid::HaloIterator::operator!=(const HaloIterator& other) const
{
    return this->index != other.index;
}

//------------ Misc Functions ------------
/**
 * @brief Get the Boundary Indices object. This includes the irrelevantBoundary (first) and the
 * relevantBoundaryIndices (second)
 * @param position The position of the boundary of the domain
 * @return irrelevantBoundary (first), relevantBoundaryIndices (second)
 */
std::pair<size_t, std::pair<size_t, size_t>> getBoundaryIndices(Position position)
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
    return { irrelevantBoundary, relevantBoundaryIndices };
}

/**
 * @brief Get the Coordinate of the Irrelevant Axis
 * @param position The position of the boundary of the domain
 * @param gridDimensions The dimensions of the grid
 * @param irrelevantBoundary The irrelevant boundary
 * @return The coordinate of the irrelevant axis
 */
size_t getCoordinateIrrelevantAxis(
    Position position, std::array<size_t, 3> gridDimensions, size_t irrelevantBoundary, CellType cellType)
{
    size_t coordinateIrrelevantAxis;
    switch (position) {
    case LEFT:
    case TOP:
    case FRONT:
        coordinateIrrelevantAxis = (cellType == CellType::Halo) ? 0 : 1;
        break;
    case RIGHT:
    case BOTTOM:
    case BACK:
        coordinateIrrelevantAxis = gridDimensions[irrelevantBoundary] - 1;
        if (cellType == CellType::Boundary) coordinateIrrelevantAxis-=1;
        break;
    }

    return coordinateIrrelevantAxis;
}

/**
 * @brief Get the Number of Relevant Boundaries
 * @param dim1 The size of the first relevant dimension
 * @param dim2 The size of the second relevant dimension
 * @param dimZ The size of the z axis grid (number of cells along the z axis)
 * @return The number of relevant boundaries
 */
size_t getNumberOfRelevantBoundaries(size_t dim1, size_t dim2, size_t dimZ)
{
    if (dimZ > 1) {
        // 3D
        return dim1 * dim2;
    } else {
        // 2D
        return 2 * dim1 + 2 * dim2 - 4;
    }
}
