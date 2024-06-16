
#pragma once
#include "utils/Position.h"
#include <map>
#include <spdlog/spdlog.h>
#include <string>

enum BoundaryType { OVERFLOW, SOFT_REFLECTIVE, PERIODIC };

class BoundaryConfig {
public:
    BoundaryConfig() = default;

    // 3D Config
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

    // 2D Config
    BoundaryConfig(BoundaryType left, BoundaryType right, BoundaryType top, BoundaryType bottom)
        : boundaryMap { { Position::LEFT, left },
                        { Position::RIGHT, right },
                        { Position::TOP, top },
                        { Position::BOTTOM, bottom } } {};

    std::map<Position, BoundaryType> boundaryMap;
};

/**
 * @brief Get the boundary type from a string
 * @param str
 */
inline BoundaryType getBoundaryType(const std::string& str)
{
    if (str == "outflow") {
        return BoundaryType::OVERFLOW;
    } else if (str == "soft_reflective") {
        return BoundaryType::SOFT_REFLECTIVE;
    } else if (str == "periodic") {
        return BoundaryType::PERIODIC;
    } else {
        spdlog::warn("Unknown boundary type: {}, choosing OVERFLOW", str);
        return BoundaryType::OVERFLOW;
    }
}

/**
 * @brief Get the string representation of a boundary type
 * @param type
 */
inline std::string getBoundaryString(const BoundaryType type)
{
    if (type == BoundaryType::OVERFLOW) {
        return "outflow";
    } else if (type == BoundaryType::SOFT_REFLECTIVE) {
        return "soft_reflective";
    } else if (type == BoundaryType::PERIODIC) {
        return "periodic";
    } else {
        spdlog::warn("Unknown boundary type, choosing OVERFLOW");
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
    case OVERFLOW:
        return 0;
    case PERIODIC:
        return 1;
    case SOFT_REFLECTIVE:
        return 2;
    default:
        spdlog::error("Boundary type not recognized. Its priority has not been specified.");
        throw std::invalid_argument("Boundary type not recognized");
    }
}

/**
 * @brief A comparison function that compares two boundaries by their priority. Returns true, if first < second i.e. the first one is higher priority than the second
 * @param first The first boundary type
 * @param second The second boundary type
 * @return If first < second i.e. the first one is higher priority than the second
 */
inline bool compareBoundaryTypeByPriority(const BoundaryType& first, const BoundaryType& second) {
    return boundaryToPriority(first) < boundaryToPriority(second);
}

/**
 * @brief A comparison function that compares two boundary config map entries by their boundary's priority. Returns true, if a < b i.e. the first one is higher priority than the second
 * @param a The first map entry
 * @param b The second map entry
 * @return a < b w.r.t their boundary type i.e. if a's boundary has higher priority than b's boundary
 */
inline bool compareBoundaryConfigMap(const std::pair<Position, BoundaryType>& a, const std::pair<Position, BoundaryType>& b) {
    BoundaryType typeA = a.second;
    BoundaryType typeB = b.second;
    return compareBoundaryTypeByPriority(typeA, typeB);
}
