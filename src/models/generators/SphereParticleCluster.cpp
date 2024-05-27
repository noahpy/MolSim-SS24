
#include "SphereParticleCluster.h"
#include <iostream>

SphereParticleCluster::SphereParticleCluster(
    std::array<double, 3> origin,
    size_t radius,
    size_t sphereDimensions,
    double spacing,
    double mass,
    std::array<double, 3> initialVelocity,
    double meanVelocity,
    size_t brownianMotionDimensions)
    : ParticleCluster(origin, mass, initialVelocity, meanVelocity, brownianMotionDimensions)
    , sphereRadius(radius)
    , sphereDimensions(sphereDimensions)
    , spacing(spacing)
{
    if (sphereDimensions != 2 && sphereDimensions != 3) {
        spdlog::warn(
            "The dimensions specified for a spherical particle cluster was set to {}. "
            "It can only be of values 2 or 3",
            dimensions);
        throw std::invalid_argument("Dimensions must be 2 or 3.");
    }

    if (radius == 0) {
        spdlog::warn("Radius of a sphere set to 0. This will not generate any particles");
        totalNumberOfParticles = 0;
        return;
    }

    // Initialize the total number of particles to be generated
    totalNumberOfParticles = getNumberOfParticlesDisc(getRealRadius(sphereRadius));
    double z_offset = spacing;
    if (sphereDimensions == 3 && sphereRadius > 0)
        for (size_t discRadius = sphereRadius - 1; discRadius > 0; discRadius--) {
            double realRadius = getRealRadius(sphereRadius, z_offset);
            totalNumberOfParticles += 2 * getNumberOfParticlesDisc(realRadius);
            z_offset += spacing;
        }
}

size_t SphereParticleCluster::getTotalNumberOfParticles() const
{
    return totalNumberOfParticles;
}

void SphereParticleCluster::generateRing(
    std::vector<Particle>& particles,
    size_t& insertionIndex,
    double realRadius,
    double z_offset) const
{
    if (realRadius < spacing) {
        // if the radius is one, there will only be a single particle
        std::array<double, 3> position { origin[0], origin[1], origin[2] + z_offset };
        std::array<double, 3> velocity =
            initialVelocity + maxwellBoltzmannDistributedVelocity(meanVelocity, dimensions);
        Particle particle = Particle(position, velocity, mass);

        particles[insertionIndex++] = particle;
    } else if (realRadius >= spacing) {
        double circumference = 2 * realRadius * M_PI;
        auto numParticles = (size_t)(circumference / spacing);
        double fullRotation = (2 * M_PI);
        auto step = (double)(fullRotation / (double)numParticles);
        double radianAngle = 0;
        while (radianAngle + step / 2 < fullRotation) {
            double x_coordinate = std::cos(radianAngle) * realRadius;
            double y_coordinate = std::sin(radianAngle) * realRadius;

            std::array<double, 3> position { origin[0] + x_coordinate,
                                             origin[1] + y_coordinate,
                                             origin[2] + z_offset };

            std::array<double, 3> velocity =
                initialVelocity + maxwellBoltzmannDistributedVelocity(meanVelocity, dimensions);
            Particle particle = Particle(position, velocity, mass);

            particles[insertionIndex++] = particle;

            radianAngle += step;
        }
    }
}

void SphereParticleCluster::generateDisc(
    std::vector<Particle>& particles,
    size_t& insertionIndex,
    double realRadius,
    double z_offset) const
{
    // single point
    if (std::abs(realRadius) < 1e-6) {
        generateRing(particles, insertionIndex, realRadius, z_offset);
        return;
    }
    // A disc is a collection of rings decreasing in radius
    auto numSteps = (size_t)(realRadius / spacing);
    double step = realRadius / (double)(numSteps);
    while (std::abs(realRadius) > 1e-8) {
        generateRing(particles, insertionIndex, realRadius, z_offset);
        realRadius -= step;
    }
}

void SphereParticleCluster::generateCluster(
    std::vector<Particle>& particles, size_t& insertionIndex) const
{
    if (sphereRadius == 0) return; // no particles to generate

    // A sphere is a stack of discs decreasing in radius
    generateDisc(particles, insertionIndex, getRealRadius(sphereRadius), 0);
    double z_offset_a = spacing, z_offset_b = -1 * spacing;
    if (sphereDimensions == 3 && sphereRadius > 0)
        for (size_t discRadius = sphereRadius - 1; discRadius > 0; discRadius--) {
            double realRadius = getRealRadius(sphereRadius, z_offset_a);
            generateDisc(particles, insertionIndex, realRadius, z_offset_a);
            generateDisc(particles, insertionIndex, realRadius, z_offset_b);

            z_offset_a += spacing;
            z_offset_b -= spacing;
        }
}

size_t SphereParticleCluster::getNumberOfParticlesDisc(double realRadius) const
{
    if (std::abs(realRadius) < 1e-6)
        return 1;

    size_t num = 0;
    auto numSteps = (size_t)(realRadius / spacing);
    double step = realRadius / (double)(numSteps);
    while (std::abs(realRadius) > 1e-8) {
        num += getNumberOfParticlesRing(realRadius);
        realRadius -= step;
    }

    return num;
}

size_t SphereParticleCluster::getNumberOfParticlesRing(double realRadius) const
{
    if (realRadius < spacing)
        return 1;
    else {
        double circumference = 2 * realRadius * M_PI;
        auto numParticles = (size_t)(circumference / spacing);

        return numParticles;
    }
}

std::string SphereParticleCluster::toString() const
{
    std::ostringstream oss;
    oss << "origin: " << origin[0] << ", " << origin[1] << ", " << origin[2] << "; ";
    oss << "mass: " << mass << "; ";
    oss << "initialVelocity: " << initialVelocity[0] << ", " << initialVelocity[1] << ", "
        << initialVelocity[2] << "; ";
    oss << "meanVelocity: " << meanVelocity << "; ";
    oss << "dimensions: " << dimensions << "; ";
    oss << "radius: " << sphereRadius << "; ";
    oss << "sphereDimensions: " << sphereDimensions << "; ";
    oss << "spacing: " << spacing << "; ";
    return oss.str();
}
