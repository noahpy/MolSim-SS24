
#pragma once
#include "utils/Position.h"
#include <map>
#include <spdlog/spdlog.h>
#include <string>

enum class BoundaryType { OUTFLOW, SOFT_REFLECTIVE, PERIODIC, STATIC };

/**
 * @brief The BoundaryConfig class is a class that holds the configuration for the boundaries of a
 * simulation
 */
class BoundaryConfig {
public:
    /**
     * @brief Construct a new Boundary Config object
     */
    BoundaryConfig() = default;

    /**
     * @brief Construct a new Boundary Config object for 3D
     * @param left The boundary type for the left side
     * @param right The boundary type for the right side
     * @param top The boundary type for the top side
     * @param bottom The boundary type for the bottom side
     * @param front The boundary type for the front side
     * @param back The boundary type for the back side
     */
    BoundaryConfig(
        BoundaryType left,
        BoundaryType right,
        BoundaryType top,
        BoundaryType bottom,
        BoundaryType front,
        BoundaryType back)
        : boundaryMap { { Position::LEFT, left },   { Position::RIGHT, right },
                        { Position::TOP, top },     { Position::BOTTOM, bottom },
                        { Position::FRONT, front }, { Position::BACK, back } } {};

    /**
     * @brief Construct a new Boundary Config object for 2D
     * @param left The boundary type for the left side
     * @param right The boundary type for the right side
     * @param top The boundary type for the top side
     * @param bottom The boundary type for the bottom side
     */
    BoundaryConfig(BoundaryType left, BoundaryType right, BoundaryType top, BoundaryType bottom)
        : boundaryMap { { Position::LEFT, left },
                        { Position::RIGHT, right },
                        { Position::TOP, top },
                        { Position::BOTTOM, bottom } } {};

    std::map<Position, BoundaryType> boundaryMap; /**< The map of boundaries */
};

/**
 * @brief Get the boundary type from a string
 * @param str
 */
inline BoundaryType getBoundaryType(const std::string& str)
{
    if (str == "outflow") {
        return BoundaryType::OUTFLOW;
    } else if (str == "soft_reflective") {
        return BoundaryType::SOFT_REFLECTIVE;
    } else if (str == "periodic") {
        return BoundaryType::PERIODIC;
    } else if (str == "static") {
        return BoundaryType::STATIC;
    } else {
        spdlog::warn("Unknown boundary type: {}, choosing OUTFLOW", str);
        return BoundaryType::OUTFLOW;
    }
}

/**
 * @brief Get the string representation of a boundary type
 * @param type
 */
inline std::string getBoundaryString(const BoundaryType type)
{
    if (type == BoundaryType::OUTFLOW) {
        return "outflow";
    } else if (type == BoundaryType::SOFT_REFLECTIVE) {
        return "soft_reflective";
    } else if (type == BoundaryType::PERIODIC) {
        return "periodic";
    } else if (type == BoundaryType::STATIC) {
        return "static";
    } else {
        spdlog::warn("Unknown boundary type, choosing OUTFLOW");
        return "overflow";
    }
}

/**
 * @brief Gets the priority of a Boundary type. Lower means higher priority.
 * @param type The boundary type to get the priority of
 * @return The priority of the boundary type (lower => higher priority)
 */
inline size_t boundaryToPriority(const BoundaryType& type)
{
    switch (type) {
    case BoundaryType::OUTFLOW:
        return 0;
    case BoundaryType::STATIC:
        return 1;
    case BoundaryType::PERIODIC:
        return 2;
    case BoundaryType::SOFT_REFLECTIVE:
        return 3;
    default:
        spdlog::error("Boundary type not recognized. Its priority has not been specified.");
        throw std::invalid_argument("Boundary type not recognized");
    }
}

/**
 * @brief A comparison function that compares two boundaries by their priority. Returns true, if
 * first < second i.e. the first one is higher priority than the second
 * @param first The first boundary type
 * @param second The second boundary type
 * @return If first < second i.e. the first one is higher priority than the second
 */
inline bool compareBoundaryTypeByPriority(const BoundaryType& first, const BoundaryType& second)
{
    return boundaryToPriority(first) < boundaryToPriority(second);
}

/**
 * @brief A comparison function that compares two boundary config map entries by their boundary's
 * priority. Returns true, if a < b i.e. the first one is higher priority than the second
 * @param a The first map entry
 * @param b The second map entry
 * @return a < b w.r.t their boundary type i.e. if a's boundary has higher priority than b's
 * boundary
 */
inline bool compareBoundaryConfigMap(
    const std::pair<Position, BoundaryType>& a, const std::pair<Position, BoundaryType>& b)
{
    BoundaryType typeA = a.second;
    BoundaryType typeB = b.second;
    return compareBoundaryTypeByPriority(typeA, typeB);
}
