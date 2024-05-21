
#include "ParticleCluster.h"

ParticleCluster::ParticleCluster(
    std::array<double, 3> origin,
    double mass,
    std::array<double, 3> initialVelocity,
    double meanVelocity,
    size_t dimensions)
    : origin(origin)
    , mass(mass)
    , initialVelocity(initialVelocity)
    , meanVelocity(meanVelocity)
    , dimensions(dimensions)
{
    if (dimensions != 2 && dimensions != 3) {
        spdlog::warn(
            "The dimensions specified for the brownian motion of a particle cluster was set to {}. "
            "It can only be of values 2 or 3",
            dimensions);
        throw std::invalid_argument("Dimensions must be 2 or 3.");
    }
}