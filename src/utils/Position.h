
#pragma once
#include <vector>

/**
 * @brief An enum to represent the position across a cube
 */
enum Position { LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK };

/**
 * @brief Converts a coordinate to a position. This assumes the halo cells added to the bounds i.e.
 * index 1 is the first valid cell
 * @param coordinate The coordinate to convert
 * @param domainSize The size of the domain
 * @return The boundary position of the coordinate
 */
std::vector<Position> coordinateToPosition(
    std::array<size_t, 3> coordinate, std::array<size_t, 3> domainSize)
{
    std::vector<Position> positions;
    if (coordinate[0] == 1) {
        positions.push_back(Position::LEFT);
    }
    if (coordinate[0] == domainSize[0] - 2) {
        positions.push_back(Position::RIGHT);
    }
    if (coordinate[1] == 1) {
        positions.push_back(Position::TOP);
    }
    if (coordinate[1] == domainSize[1] - 2) {
        positions.push_back(Position::BOTTOM);
    }
    if (coordinate[2] == 1) {
        positions.push_back(Position::FRONT);
    }
    if (coordinate[2] == domainSize[2] - 2) {
        positions.push_back(Position::BACK);
    }

    if (positions.empty())
        throw std::invalid_argument("Coordinate is not on the boundary");

    return positions;
}

std::array<double, 3> getNormalVectorOfBoundary(Position position)
{
    switch (position) {
    case Position::LEFT:
        return { -1, 0, 0 };
    case Position::RIGHT:
        return { 1, 0, 0 };
    case Position::TOP:
        return { 0, 1, 0 };
    case Position::BOTTOM:
        return { 0, -1, 0 };
    case Position::FRONT:
        return { 0, 0, 1 };
    case Position::BACK:
        return { 0, 0, -1 };
    default:
        throw std::invalid_argument("Invalid position");
    }
}
