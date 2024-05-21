
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
    }

    // Initialize the total number of particles to be generated
    totalNumberOfParticles = getNumberOfParticlesDisc(sphereRadius);
    if (sphereDimensions == 3 && sphereRadius > 0)
        for (size_t discRadius = sphereRadius - 1; discRadius > 0; discRadius--)
            totalNumberOfParticles += 2 * getNumberOfParticlesDisc(discRadius);
}

size_t SphereParticleCluster::getTotalNumberOfParticles() const
{
    return totalNumberOfParticles;
}

void SphereParticleCluster::generateRing(
    std::vector<Particle>& particles, size_t& insertionIndex, size_t radius, double z_offset) const
{
    if (radius == 1) {
        // if the radius is one, there will only be a single particle
        std::array<double, 3> position { origin[0], origin[1], origin[2] + z_offset };
        std::array<double, 3> velocity =
            initialVelocity + maxwellBoltzmannDistributedVelocity(meanVelocity, dimensions);
        Particle particle = Particle(position, velocity, mass);

        particles[insertionIndex++] = particle;
    } else if (radius > 1){
        double realRadius = getRealRadius(radius);
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
    std::vector<Particle>& particles, size_t& insertionIndex, size_t radius, double z_offset) const
{
    // A disc is a collection of rings decreasing in radius
    while (radius > 0) {
        generateRing(particles, insertionIndex, radius, z_offset);
        radius -= 1;
    }
}

void SphereParticleCluster::generateCluster(
    std::vector<Particle>& particles, size_t& insertionIndex) const
{
    // A sphere is a stack of discs decreasing in radius
    generateDisc(particles, insertionIndex, sphereRadius, 0);
    double z_offset_a = spacing, z_offset_b = -1 * spacing;
    if (sphereDimensions == 3 && sphereRadius > 0) {
        for (size_t discRadius = sphereRadius - 1; discRadius > 0; discRadius--) {
            generateDisc(particles, insertionIndex, discRadius, z_offset_a);
            generateDisc(particles, insertionIndex, discRadius, z_offset_b);

            z_offset_a += spacing;
            z_offset_b -= spacing;
        }
    }

    spdlog::info("Generated SphereCluster: {}", toString());
}

size_t SphereParticleCluster::getNumberOfParticlesDisc(size_t radius) const
{
    size_t num = 0;
    while (radius > 0) {
        num += getNumberOfParticlesRing(radius--);
    }

    return num;
}

size_t SphereParticleCluster::getNumberOfParticlesRing(size_t radius) const
{
    if (radius == 1)
        return 1;
    else {
        double realRadius = getRealRadius(radius);
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
