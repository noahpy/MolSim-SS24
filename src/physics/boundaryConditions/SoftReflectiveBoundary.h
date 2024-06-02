
#include "physics/boundaryConditions/BoundaryCondition.h"

class SoftReflectiveBoundary : public BoundaryCondition {
public:
    using BoundaryCondition::BoundaryCondition;

    void preUpdateBoundaryHandling(Simulation& simulation) override;
    void postUpdateBoundaryHandling(Simulation& simulation) override {
        // Do nothing
    };
};
