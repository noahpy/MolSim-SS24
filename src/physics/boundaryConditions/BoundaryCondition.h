
#include "Position.h"
#include "simulation/baseSimulation.h"

/**
 * @brief The BoundaryCondition class is an abstract class that defines the interface for boundary conditions
 */
class BoundaryCondition {
public:
    /**
     * @brief Constructor for the BoundaryCondition class
     * @param position The position of the boundary condition
     */
    explicit BoundaryCondition(Position position) : position(position) {}

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

protected:
    Position position; /**< The position of the boundary condition */
};
