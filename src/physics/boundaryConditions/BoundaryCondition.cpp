
#include "BoundaryCondition.h"
#include "simulation/LennardJonesDomainSimulation.h"

std::array<double, 3> BoundaryCondition::getPointOnBoundaryPlane(
    const LennardJonesDomainSimulation& LJDSim)
{
    std::array<double, 3> pointOnBoundaryPlane {};
    switch (position) {
    // To mirror point, we need a point on the plane to mirror it about
    case BOTTOM:
    case LEFT:
    case BACK:
        pointOnBoundaryPlane = LJDSim.getGrid().domainOrigin;
        break;
    case TOP:
    case RIGHT:
    case FRONT:
        pointOnBoundaryPlane = LJDSim.getGrid().domainEnd;
        break;
    }

    return pointOnBoundaryPlane;
}
