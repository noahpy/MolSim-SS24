
#include "simulation/LennardJonesDomainSimulation.h"

class OverflowBoundary : public BoundaryCondition {
public:
    using BoundaryCondition::BoundaryCondition;

    void preUpdateBoundaryHandling(Simulation& simulation) override {
        // Do nothing
    };
    void postUpdateBoundaryHandling(Simulation& simulation) override;
};
