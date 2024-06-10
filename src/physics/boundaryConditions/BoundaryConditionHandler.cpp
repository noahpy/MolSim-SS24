#include "physics/boundaryConditions/BoundaryConditionHandler.h"
#include "physics/boundaryConditions/OverflowBoundary.h"
#include "physics/boundaryConditions/SoftReflectiveBoundary.h"
#include <spdlog/spdlog.h>

BoundaryConditionHandler::BoundaryConditionHandler(const BoundaryConfig& boundaryConfig)
    : boundaryConditions()
    , dimensionality((boundaryConfig.boundaryMap.size() == 6) ? 3 : 2)
    , boundaryConfig(boundaryConfig)
{
    if (boundaryConfig.boundaryMap.size() != 6 && boundaryConfig.boundaryMap.size() != 4) {
        spdlog::error("BoundaryConfig must have 4 (2D) or 6 (3D) boundaries.");
        exit(EXIT_FAILURE);
    }

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
            exit(EXIT_FAILURE);
        }
    }
}

void BoundaryConditionHandler::preUpdateBoundaryHandling(Simulation& simulation)
{
    for (auto& bc : boundaryConditions) {
        bc->preUpdateBoundaryHandling(simulation);
    }
}

void BoundaryConditionHandler::postUpdateBoundaryHandling(Simulation& simulation)
{
    for (auto& bc : boundaryConditions) {
        bc->postUpdateBoundaryHandling(simulation);
    }
}
