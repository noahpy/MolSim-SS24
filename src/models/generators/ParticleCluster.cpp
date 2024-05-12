
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
}