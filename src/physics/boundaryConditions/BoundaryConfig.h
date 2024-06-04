#include "utils/Params.h"
#include "utils/Position.h"
#include <map>

class BoundaryConfig {
public:
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
