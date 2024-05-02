

#include "physics/strategy.h"

PhysicsStrategy::PhysicsStrategy(
    std::function<void(const Simulation&)> calX,
    std::function<void(const Simulation&)> calV,
    std::function<void(const Simulation&)> calF)
{
    this->calX = calX;
    this->calV = calV;
    this->calF = calF;
}
