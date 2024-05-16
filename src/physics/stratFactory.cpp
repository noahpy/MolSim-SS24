
#include "physics/stratFactory.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include <spdlog/spdlog.h>

PhysicsStrategy stratFactory(unsigned simulation_type)
{
    switch (simulation_type) {
    case 0:
        return { location_stroemer_verlet, velocity_stroemer_verlet, force_gravity_V2 };
    case 1:
        return { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones };
    default:
        spdlog::info("Unknown simulation type, proceeding with default physics strategy.");
        return { location_stroemer_verlet, velocity_stroemer_verlet, force_gravity_V2 };
    }
}
