
#pragma once
#include "simulation/baseSimulation.h"
#include "utils/Position.h"

// forward declare
class LennardJonesDomainSimulation;

/**
 * @brief The BoundaryCondition class is an abstract class that defines the interface for boundary
 * conditions
 */
class BoundaryCondition {
public:
    /**
     * @brief Constructor for the BoundaryCondition class
     * @param position The position of the boundary condition
     */
    explicit BoundaryCondition(Position position)
        : position(position)
    {
    }

    // Virtual destructor
    virtual ~BoundaryCondition() = default;

    /**
     * @brief The call to apply the boundary condition to the simulation before any updates are made
     * @param simulation The simulation to apply the boundary condition to
     * @return void
     */
    virtual void preUpdateBoundaryHandling(Simulation& simulation) = 0;
    /**
     * @brief The call to apply the boundary condition to the simulation after all updates are made
     * @param simulation The simulation to apply the boundary condition to
     * @return void
     */
    virtual void postUpdateBoundaryHandling(Simulation& simulation) = 0;

    /**
     * @brief Returns a point on the boundary-plane
     * @param LJDSim The simulation for wich to get the point for
     * @return A point on the boundary-plane
     */
    std::array<double, 3> getPointOnBoundaryPlane(const LennardJonesDomainSimulation& LJDSim);

    /**
     * @brief Get the dimension that is relevant to the boundary
     * @details E.g. if the point on the boundary is (1,2,3) and the normal vector to that plane is
     * (-1,0,0) (LEFT-Boundary), then return 0 as that is the index of the coordinate of interest
     * @param normal The normal vector to the plane
     * @return The index of the relevant dimension
     */
    static size_t getRelevantDimension(const std::array<double, 3>& normal)
    {
        size_t relevantDimension = 0;
        for (size_t i = 0; i < 3; i++)
            if (normal[i] != 0) {
                relevantDimension = i;
                break;
            }
        return relevantDimension;
    }

protected:
    Position position; /**< The position of the boundary condition */
};
