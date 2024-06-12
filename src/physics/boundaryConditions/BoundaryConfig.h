
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
    if (str == "overflow") {
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
        return "overflow";
    } else if (type == BoundaryType::SOFT_REFLECTIVE) {
        return "soft_reflective";
    } else if (type == BoundaryType::PERIODIC) {
        return "periodic";
    } else {
        spdlog::warn("Unknown boundary type, choosing OVERFLOW");
        return "overflow";
    }
}
