
#include "physics/stratFactory.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include <spdlog/spdlog.h>

PhysicsStrategy stratFactory(SimulationType simulation_type, ParallelType parallel_type)
{
    switch (simulation_type) {
    case SimulationType::PLANET:
        spdlog::info("Initializing Force Gravity Strat...");
        return { location_stroemer_verlet, velocity_stroemer_verlet, force_gravity_V2 };
    case SimulationType::LJ:
        spdlog::info("Initializing Force LJ Strat...");
        return { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones };
    case SimulationType::LINKED_LJ:
        spdlog::info("Initializing Force LJ LC Strat...");
        return { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones_lc };
    case SimulationType::DOMAIN_LJ:
        spdlog::info("Initializing Force LJ Domain Strat...");
        return { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones_lc };
    case SimulationType::MIXED_LJ:
        spdlog::info("Initializing Force LJ Mixed Strat...");
        if (parallel_type == ParallelType::STATIC) {
            spdlog::info("Parallel strategy: static");
            return { location_stroemer_verlet,
                     velocity_stroemer_verlet,
                     force_mixed_LJ_gravity_lc };
        }
        else {
            spdlog::info("Parallel strategy: task");
            return { location_stroemer_verlet,
                     velocity_stroemer_verlet,
                     force_mixed_LJ_gravity_lc_task };
        }
    case SimulationType::MEMBRANE_LJ:
        spdlog::info("Initializing Force Membrane Strat...");
        return { location_stroemer_verlet, velocity_stroemer_verlet, force_membrane };
    default:
        spdlog::warn("Unknown simulation type, proceeding with default physics strategy.");
        return { location_stroemer_verlet, velocity_stroemer_verlet, force_gravity_V2 };
    }
}
