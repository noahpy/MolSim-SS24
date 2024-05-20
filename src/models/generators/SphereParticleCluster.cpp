
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
        std::array<double, 3> position { origin[0], origin[1], origin[2] + z_offset };
        std::array<double, 3> velocity =
            initialVelocity + maxwellBoltzmannDistributedVelocity(meanVelocity, dimensions);
        Particle particle = Particle(position, velocity, mass);

        particles[insertionIndex++] = particle;
    } else {
        double realRadius = getRealRadius(radius);
        double circumference = std::pow(realRadius, 2) * M_PI;
        auto numParticles = (size_t)(circumference / spacing);
        double fullRotation = (2 * M_PI);
        auto step = (double)(fullRotation / (double)numParticles);
        double radianAngle = 0;
        while (radianAngle < fullRotation) {
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
    while (radius > 0) {
        generateRing(particles, insertionIndex, radius, z_offset);
        radius -= 1;
    }
}

void SphereParticleCluster::generateCluster(
    std::vector<Particle>& particles, size_t& insertionIndex) const
{
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
    std::cout << insertionIndex << "; " << getTotalNumberOfParticles() << std::endl;
}

size_t SphereParticleCluster::getNumberOfParticlesDisc(size_t radius) const
{
    size_t num = 0;
    while (radius > 1) {
        num += getNumberOfParticlesRing(radius--);
    }

    if (radius == 1)
        num++;

    return num;
}

size_t SphereParticleCluster::getNumberOfParticlesRing(size_t radius) const
{
    if (radius == 1)
        return 1;
    else {
        double realRadius = getRealRadius(radius);
        double circumference = std::pow(realRadius, 2) * M_PI;
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
