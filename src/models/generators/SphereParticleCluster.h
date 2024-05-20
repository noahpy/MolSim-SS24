
#pragma once
#include "ParticleCluster.h"

class SphereParticleCluster : public ParticleCluster {
public:
    SphereParticleCluster(
        std::array<double, 3> origin,
        size_t radius,
        size_t sphereDimensions,
        double spacing,
        double mass,
        std::array<double, 3> initialVelocity,
        double meanVelocity,
        size_t brownianMotionDimensions);

    size_t getTotalNumberOfParticles() const override;
    void generateCluster(std::vector<Particle>& particles, size_t& insertionIndex) const override;
    std::string toString() const override;

protected:
    size_t sphereRadius;
    double spacing;
    size_t sphereDimensions;

private:
    size_t totalNumberOfParticles;
    double getRealRadius(size_t radius) const { return ((double) radius - 1) * spacing; }

    size_t getNumberOfParticlesDisc(size_t radius) const;
    size_t getNumberOfParticlesRing(size_t radius) const;

    void generateDisc(
        std::vector<Particle>& particles,
        size_t& insertionIndex,
        size_t radius,
        double z_offset) const;
    void generateRing(
        std::vector<Particle>& particles,
        size_t& insertionIndex,
        size_t radius,
        double z_offset) const;
};
