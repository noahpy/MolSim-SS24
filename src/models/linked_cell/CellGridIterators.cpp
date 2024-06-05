
#include "CellGrid.h"
#include "models/linked_cell/cell/Cell.h"
#include <iostream>

// forward declare
std::pair<size_t, std::pair<size_t, size_t>> getBoundaryIndices(Position position);
size_t getCoordinateIrrelevantAxis(
    Position position,
    std::array<size_t, 3> gridDimensions,
    size_t irrelevantBoundary,
    CellType cellType);
size_t getNumberOfRelevantBoundaries(std::array<size_t, 3> gridDimensions, Position position, CellType cellType, bool is2D);

// TODO Move the creation the iterators to the class and then only return the fresh ones
/* ########### BoundaryIterator Implementation ########### */
CellGrid::BoundaryIterator::BoundaryIterator(
    Position position, std::array<size_t, 3> gridDimensions, bool is2D)
    : gridDimensions(gridDimensions)
    , index(0)
    , is2D(is2D)
{
    if (is2D && (position == FRONT || position == BACK)) {
        // if we are in 2D there are no front and Back boundaries
        boundaries = {};
        return;
    }

    size_t irrelevantBoundary = getBoundaryIndices(position).first;
    std::pair<size_t, size_t> relevantBoundaryIndices = getBoundaryIndices(position).second;

    size_t coordinateIrrelevantAxis = getCoordinateIrrelevantAxis(
        position, gridDimensions, irrelevantBoundary, CellType::Boundary);

    // Make space for the required indices
    std::vector<CellIndex> relevantBoundaries(getNumberOfRelevantBoundaries(
        gridDimensions, position, CellType::Boundary, is2D));
    size_t insertionIndex = 0;

    for (size_t i = 1; i < gridDimensions[relevantBoundaryIndices.first] - 1; i++) {
        CellIndex boundary;

        if (is2D) {
            if (irrelevantBoundary == 0)
                boundary = { coordinateIrrelevantAxis, i, 0 };
            else if (irrelevantBoundary == 1)
                boundary = { i, coordinateIrrelevantAxis, 0 };
            // there cannot be irrelevantBoundary == 2 in 2D

            relevantBoundaries.at(insertionIndex++) = boundary;
        } else {
            for (size_t j = 1; j < gridDimensions[relevantBoundaryIndices.second] - 1; j++) {
                if (irrelevantBoundary == 0)
                    boundary = { coordinateIrrelevantAxis, i, j };
                else if (irrelevantBoundary == 1)
                    boundary = { i, coordinateIrrelevantAxis, j };
                else
                    boundary = { i, j, coordinateIrrelevantAxis };

                relevantBoundaries.at(insertionIndex++) = boundary;
            }
        }

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
    Position position, std::array<size_t, 3> gridDimensions, bool is2D)
    : gridDimensions(gridDimensions)
    , index(0)
    , is2D(is2D)
{
    if (is2D && (position == FRONT || position == BACK)) {
        // if we are in 2D there are no front and Back halos
        boundaries = {};
        return;
    }

    size_t irrelevantBoundary = getBoundaryIndices(position).first;
    std::pair<size_t, size_t> relevantBoundaryIndices = getBoundaryIndices(position).second;

    size_t coordinateIrrelevantAxis =
        getCoordinateIrrelevantAxis(position, gridDimensions, irrelevantBoundary, CellType::Halo);

    std::vector<CellIndex> relevantBoundaries(getNumberOfRelevantBoundaries(
        gridDimensions, position, CellType::Halo, is2D));
    size_t insertionIndex = 0;

    for (size_t i = 0; i < gridDimensions[relevantBoundaryIndices.first]; i++) {
        CellIndex boundary;

        if (is2D) {
            if (irrelevantBoundary == 0)
                boundary = { coordinateIrrelevantAxis, i, 0 };
            else if (irrelevantBoundary == 1)
                boundary = { i, coordinateIrrelevantAxis, 0 };
            // there cannot be irrelevantBoundary == 2 in 2D

            relevantBoundaries.at(insertionIndex++) = boundary;
        } else {
            for (size_t j = 0; j < gridDimensions[relevantBoundaryIndices.second]; j++) {
                if (irrelevantBoundary == 0)
                    boundary = { coordinateIrrelevantAxis, i, j };
                else if (irrelevantBoundary == 1)
                    boundary = { i, coordinateIrrelevantAxis, j };
                else
                    boundary = { i, j, coordinateIrrelevantAxis };

                relevantBoundaries.at(insertionIndex++) = boundary;
            }
        }

    }

    boundaries = relevantBoundaries;

    // TODO check in 2D case
    // std::cout << "Correct Number (Halo) " << (insertionIndex == relevantBoundaries.size()) <<
    // std::endl;
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
    Position position,
    std::array<size_t, 3> gridDimensions,
    size_t irrelevantBoundary,
    CellType cellType)
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
        if (cellType == CellType::Boundary)
            coordinateIrrelevantAxis -= 1;
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
size_t getNumberOfRelevantBoundaries(
    std::array<size_t, 3> gridDimensions, Position position, CellType cellType, bool is2D)
{
    std::pair<size_t, size_t> relevantBoundaryIndices = getBoundaryIndices(position).second;
    size_t numDim1 = gridDimensions[relevantBoundaryIndices.first],
           numDim2 = gridDimensions[relevantBoundaryIndices.second];

    switch (cellType) {
    case CellType::Boundary:
        numDim1 -= 2;
        numDim2 -= 2;
        break;
    case CellType::Halo:
        break;
    default:
        return 0;
    }

    if (is2D)
        return numDim1;
    else
        return numDim1 * numDim2;
}
