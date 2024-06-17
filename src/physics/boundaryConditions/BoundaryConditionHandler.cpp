#include "physics/boundaryConditions/BoundaryConditionHandler.h"
#include "physics/boundaryConditions/OverflowBoundary.h"
#include "physics/boundaryConditions/PeriodicBoundary.h"
#include "physics/boundaryConditions/SoftReflectiveBoundary.h"
#include <spdlog/spdlog.h>
#include <algorithm>

BoundaryConditionHandler::BoundaryConditionHandler(
    const BoundaryConfig& boundaryConfig, const CellGrid& cellGrid)
    : boundaryConditions()
    , dimensionality((boundaryConfig.boundaryMap.size() == 6) ? 3 : 2)
    , boundaryConfig(boundaryConfig)
{
    if (boundaryConfig.boundaryMap.size() != 6 && boundaryConfig.boundaryMap.size() != 4) {
        spdlog::error("BoundaryConfig must have 4 (2D) or 6 (3D) boundaries.");
        exit(EXIT_FAILURE);
    }

    // Make the map into a vector of pairs that can be sorted
    std::vector<std::pair<Position, BoundaryType>> pairs;
    for (auto boundary : boundaryConfig.boundaryMap) {
        pairs.emplace_back(boundary);
    }

    // Sort the boundaries by their priority. The highest priority will be first
    // This way, the loop in preUpdateBoundaryHandling and postUpdateBoundaryHandling will regard
    // the priorities
    std::sort(pairs.begin(), pairs.end(), compareBoundaryConfigMap);

    for (auto boundary : pairs) {
        Position position = boundary.first;
        BoundaryType type = boundary.second;

        if (type == PERIODIC &&
            boundaryConfig.boundaryMap.at(oppositePosition(position)) != PERIODIC) {
            spdlog::error(
                "Periodic boundary conditions must be defined in pairs i.e. opposite sides must be "
                "both periodic. Miss-match between {} and {} side",
                getPositionString(position),
                getPositionString(oppositePosition(position)));
            exit(EXIT_FAILURE);
        }

        switch (type) {
        case BoundaryType::OVERFLOW:
            boundaryConditions.push_back(std::make_unique<OverflowBoundary>(position));
            break;
        case BoundaryType::SOFT_REFLECTIVE:
            boundaryConditions.push_back(std::make_unique<SoftReflectiveBoundary>(position));
            break;
        case BoundaryType::PERIODIC:
            boundaryConditions.push_back(
                std::make_unique<PeriodicBoundary>(position, boundaryConfig, cellGrid));
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
