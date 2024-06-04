
#include "simulation/LennardJonesDomainSimulation.h"

/**
 * @brief The OverflowBoundary class is a class that represents a boundary condition where particles
 * that leave the domain are removed
 */
class OverflowBoundary : public BoundaryCondition {
public:
    // lift constructor
    using BoundaryCondition::BoundaryCondition;

    /**
     * @brief The call to apply the boundary condition to the simulation before all updates are
     * made. This does nothing, as all changes are performed after the updates
     * @param simulation The simulation to apply the boundary to
     * @return void
     */
    void preUpdateBoundaryHandling(Simulation& simulation) override {
        // Do nothing
    };
    /**
     * @brief The call to apply the boundary condition to the simulation after all updates are made.
     * This will remove the particles that left the domain
     * @param simulation The simulation to apply the boundary to
     * @return void
     */
    void postUpdateBoundaryHandling(Simulation& simulation) override;
};
