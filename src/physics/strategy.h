
#pragma once

#include "simulation/baseSimulation.h"
#include <functional>

class PhysicsStrategy {
public:
    PhysicsStrategy(
        std::function<void(const Simulation&)> calX,
        std::function<void(const Simulation&)> calV,
        std::function<void(const Simulation&)> calF);

    std::function<void(const Simulation&)> calX;
    std::function<void(const Simulation&)> calV;
    std::function<void(const Simulation&)> calF;
};
