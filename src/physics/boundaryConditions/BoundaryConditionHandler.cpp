#include "physics/boundaryConditions/BoundaryConditionHandler.h"
#include "physics/boundaryConditions/OverflowBoundary.h"
#include "physics/boundaryConditions/SoftReflectiveBoundary.h"

BoundaryConditionHandler::BoundaryConditionHandler(const BoundaryConfig& boundaryConfig)
    : boundaryConditions()
{
    for (auto boundary : boundaryConfig.boundaryMap) {
        Position position = boundary.first;
        BoundaryType type = boundary.second;

        switch (type) {
        case BoundaryType::OVERFLOW:
            boundaryConditions.push_back(std::make_unique<OverflowBoundary>(position));
            break;
        case BoundaryType::SOFT_REFLECTIVE:
            boundaryConditions.push_back(std::make_unique<SoftReflectiveBoundary>(position));
            break;
        default:
            spdlog::error("Boundary type not recognized.");
            break;
        }
    }
}
