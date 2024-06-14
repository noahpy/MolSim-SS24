
#pragma once
#include <array>
#include <stdexcept>
#include <vector>

/**
 * @brief An enum to represent the position across a cube
 */
enum Position { LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK };
static std::vector<Position> allPositions = { Position::LEFT,   Position::RIGHT, Position::TOP,
                                              Position::BOTTOM, Position::FRONT, Position::BACK };

/**
 * @brief Converts a coordinate to a position. This assumes the halo cells added to the bounds i.e.
 * index 1 is the first valid cell
 * @param coordinate The coordinate to convert
 * @param gridDimensions The size of the grid
 * @param keep3D Whether to keep the 3rd dimension
 * @param isHalo Whether to get the sides for a halo cell
 * @return The boundary position of the coordinate
 */
inline std::vector<Position> coordinateToPosition(
    std::array<size_t, 3> coordinate, std::array<size_t, 3> gridDimensions, bool keep3D = true, bool isHalo = false)
{
    size_t offset = isHalo ? 0 : 1;
    std::vector<Position> positions;
    if (coordinate[0] == offset) {
        positions.push_back(Position::LEFT);
    }
    if (coordinate[0] == gridDimensions[0] - 1 - offset) {
        positions.push_back(Position::RIGHT);
    }
    if (coordinate[1] == gridDimensions[1] - 1 - offset) {
        positions.push_back(Position::TOP);
    }
    if (coordinate[1] == offset) {
        positions.push_back(Position::BOTTOM);
    }
    if (coordinate[2] == gridDimensions[2] - 1 - offset && keep3D) {
        positions.push_back(Position::FRONT);
    }
    if (coordinate[2] == offset && keep3D) {
        positions.push_back(Position::BACK);
    }

    if (positions.empty())
        throw std::invalid_argument("Coordinate is not on the boundary");

    return positions;
}

inline std::vector<Position> relCoordinateToPos(std::array<int, 3> coordinate)
{
    std::vector<Position> positions;
    if (coordinate[0] == -1) {
        positions.push_back(Position::LEFT);
    }
    if (coordinate[0] == 1) {
        positions.push_back(Position::RIGHT);
    }
    if (coordinate[1] == 1) {
        positions.push_back(Position::TOP);
    }
    if (coordinate[1] == -1) {
        positions.push_back(Position::BOTTOM);
    }
    if (coordinate[2] == 1) {
        positions.push_back(Position::FRONT);
    }
    if (coordinate[2] == -1) {
        positions.push_back(Position::BACK);
    }
    return positions;
}

inline std::array<double, 3> getNormalVectorOfBoundary(Position position)
{
    switch (position) {
    case Position::LEFT:
        return { 1, 0, 0 };
    case Position::RIGHT:
        return { -1, 0, 0 };
    case Position::TOP:
        return { 0, -1, 0 };
    case Position::BOTTOM:
        return { 0, 1, 0 };
    case Position::FRONT:
        return { 0, 0, -1 };
    case Position::BACK:
        return { 0, 0, 1 };
    default:
        throw std::invalid_argument("Invalid position");
    }
}

inline Position oppositePosition(Position position) {
    switch (position) {
    case Position::LEFT:
        return Position::RIGHT;
    case Position::RIGHT:
        return Position::LEFT;
    case Position::TOP:
        return Position::BOTTOM;
    case Position::BOTTOM:
        return Position::TOP;
    case Position::FRONT:
        return Position::BACK;
    case Position::BACK:
        return Position::FRONT;
    default:
        throw std::invalid_argument("Invalid position");
    }
}

/**
 * @brief Converts a position to a string
 * @param position The position to convert
 * @return The string representation of the position
 */
inline std::string getPositionString(Position position)
{
    switch (position) {
    case Position::LEFT:
        return "LEFT";
    case Position::RIGHT:
        return "RIGHT";
    case Position::TOP:
        return "TOP";
    case Position::BOTTOM:
        return "BOTTOM";
    case Position::FRONT:
        return "FRONT";
    case Position::BACK:
        return "BACK";
    default:
        throw std::invalid_argument("Invalid position");
    }
}
