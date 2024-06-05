
#pragma once
#include "physics/boundaryConditions/BoundaryCondition.h"
#include "physics/boundaryConditions/BoundaryConfig.h"
#include "simulation/baseSimulation.h"

/**
 * @brief The BoundaryConditionHandler class is a class that handles the application of boundary
 * conditions to a simulation
 */
class BoundaryConditionHandler {
public:
    /**
     * @brief Constructor for the BoundaryConditionHandler class
     * @param boundaryConfig The boundary conditions to apply described in the object
     */
    explicit BoundaryConditionHandler(const BoundaryConfig& boundaryConfig);

    /**
     * @brief The call to apply the boundary condition to the simulation before any updates are made
     * @param simulation The simulation to apply the boundary condition to
     * @return void
     */
    void preUpdateBoundaryHandling(Simulation& simulation);

    /**
     * @brief The call to apply the boundary condition to the simulation after all updates are made
     * @param simulation The simulation to apply the boundary condition to
     * @return void
     */
    void postUpdateBoundaryHandling(Simulation& simulation);

    size_t dimensionality; /**< The dimensionality of the simulation as by the provided boundaries */

protected:
    std::vector<std::unique_ptr<BoundaryCondition>>
        boundaryConditions; /**< The boundary conditions to apply */
};
