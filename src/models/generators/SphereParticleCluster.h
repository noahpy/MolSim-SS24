
#pragma once
#include "ParticleCluster.h"

class SphereParticleCluster : public ParticleCluster {
public:
    /**
     * @brief Constructor to initialize a spherical particle clusters
     * @details A sphere is either a singe disc in the case of 2D spheres or a stack of discs with
     * decreasing radii. Discs are a collection of rings.
     * @param origin The center of the sphere
     * @param radius The radius of the sphere to be generated in number of particles (i.e. 1 -> a
     * single particle, 2 -> single particle in the center surrounded by a single circle of
     * particles, etc.)
     * @param sphereDimensions The dimensionality of the sphere (must be 2 i.e. disc or 3 i.e.
     * sphere)
     * @param spacing The spacing between the particles (i.e. separation between the discs and the
     * distance along the circumference of a given ring)
     * @param mass The mass of the particles within the cluster
     * @param initialVelocity The initial velocity of all particles in the cluster
     * @param meanVelocity The mean velocity of all particles in the cluster (passed to
     * maxwellBoltzmannDistributedVelocity())
     * @param brownianMotionDimensions The dimensions of the cluster (<= 3) will be passed to
     * maxwellBoltzmannDistributedVelocity() if set to 0 no brownian motion will be added, if set to
     * 1 then only on the x axis, then x+y, and finally x+y+z axis
     */
    SphereParticleCluster(
        std::array<double, 3> origin,
        size_t radius,
        size_t sphereDimensions,
        double spacing,
        double mass,
        std::array<double, 3> initialVelocity,
        double meanVelocity,
        size_t brownianMotionDimensions);

    /**
     * @brief Get the total number of particles in the cluster
     * @return The total number of particles in the spherical cluster
     */
    size_t getTotalNumberOfParticles() const override;
    /**
     * @brief Generate the spherical cluster of particles
     * @param particles The vector to insert the particles into
     * @param insertionIndex The index to start inserting the particles at (will be incremented)
     * @return void
     */
    void generateCluster(std::vector<Particle>& particles, size_t& insertionIndex) const override;
    /**
     * @brief Get a string representation of the cluster
     * @return A string representation of the cluster
     */
    std::string toString() const override;

protected:
    size_t sphereRadius; /**< The radius of the sphere to be generated. This is a natural number
                            indicating the number of particles along the radius*/
    double spacing; /**< The spacing between the particles. This is also the spacing between the
                       discs and the distance between particles along the circumference */
    size_t sphereDimensions; /**< The dimensionality of the sphere. Can be 2 (a single disc) or 3 (a
                                sphere) */

private:
    size_t totalNumberOfParticles; /**< The total amount of particles that will be generated */
    /**
     * @brief Convert a radius in particles into a radius of distance
     * @param radius The radius in particles
     * @return The radius in distance ((radius_in_particles - 1) * spacing)
     */
    double getRealRadius(size_t radius) const
    {
        if (radius == 0)
            return 0;

        return ((double)radius - 1) * spacing;
    }
    /**
     * @brief Get the real radius of a ring given the radius of the sphere and the offset along the
     * z axis
     * @param radius The radius of the circle in number of particles
     * @param offset The offset of the disc on the z-axis
     * @return The real radius regarding the offset of the disc
     */
    double getRealRadius(size_t radius, double offset) const
    {
        if (std::abs(getRealRadius(radius) - offset) < 1e-6) {
            return 0; // if they are the same, then the radius must be 0
        }
        return std::sqrt(std::pow(getRealRadius(radius), 2) - std::pow(offset, 2));
    }

    /**
     * @brief Get the number of particles in a disc of a given radius in particles
     * @param realRadius The actual radius of the ring to generate
     * @return The number of particles in the disc
     */
    size_t getNumberOfParticlesDisc(double realRadius) const;
    /**
     * @brief Get the number of particles in a ring of a given radius in particles
     * @param realRadius The actual radius of the ring to generate
     * @return The number of particles in the ring
     */
    size_t getNumberOfParticlesRing(double realRadius) const;

    /**
     * @brief Generate a disc into the particles vector starting at the index provided
     * @param particles The vector reference into which to insert
     * @param insertionIndex The index from which to start inserting from
     * @param realRadius The actual radius of the ring to generate
     * @param z_offset The offset along the z axis from the origin
     */
    void generateDisc(
        std::vector<Particle>& particles,
        size_t& insertionIndex,
        double realRadius,
        double z_offset) const;
    /**
     * @brief Generate a ring of particles into the particles vector starting at the index provided
     * @param particles The vector reference into which to insert
     * @param insertionIndex The index from which to start inserting from
     * @param realRadius The actual radius of the ring to generate
     * @param z_offset The offset along the z axis from the origin
     */
    void generateRing(
        std::vector<Particle>& particles,
        size_t& insertionIndex,
        double realRadius,
        double z_offset) const;
};
