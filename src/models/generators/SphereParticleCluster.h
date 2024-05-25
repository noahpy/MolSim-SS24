
#pragma once
#include "ParticleCluster.h"

class SphereParticleCluster : public ParticleCluster {
public:
    /**
     * @brief Constructor to initialize a spherical particle clusters
     * @details A sphere is either a singe disc in the case of 2D spheres or a stack of discs with
     * decreasing radii. Discs are a collection of rings.
     * @param origin The center of the sphere
     * @param radius The radius of the sphere to be generated in number of particles (i.e. 1 -> a single particle, 2 -> single particle in the center surrounded by a single circle of particles, etc.)
     * @param sphereDimensions The dimensionality of the sphere (must be 2 i.e. disc or 3 i.e. sphere)
     * @param spacing The spacing between the particles (i.e. separation between the discs and the distance along the circumference of a given ring)
     * @param mass The mass of the particles within the cluster
     * @param initialVelocity The initial velocity of all particles in the cluster
     * @param meanVelocity The mean velocity of all particles in the cluster (passed to maxwellBoltzmannDistributedVelocity())
     * @param brownianMotionDimensions The dimensions of the cluster (<= 3) will be passed to maxwellBoltzmannDistributedVelocity() if set to 0 no brownian motion will be added, if set to 1 then only on the x axis, then x+y, and finally x+y+z axis
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
    size_t sphereRadius; /**< The radius of the sphere to be generated. This is a natural number indicating the number of particles along the radius*/
    double spacing; /**< The spacing between the particles. This is also the spacing between the discs and the distance between particles along the circumference */
    size_t sphereDimensions; /**< The dimensionality of the sphere. Can be 2 (a single disc) or 3 (a sphere) */

private:
    size_t totalNumberOfParticles; /**< The total amount of particles that will be generated */
    std::vector<size_t> radiusList;

    /**
     * @brief Convert a radius in particles into a radius of distance
     * @param radius The radius in particles
     * @return The radius in distance ((radius_in_particles - 1) * spacing)
     */
    double getRealRadius(size_t radius) const { return ((double) radius - 1) * spacing; }

    /**
     * @brief Get the number of particles in a disc of a given radius in particles
     * @param radius The radius in number of particles
     * @return The number of particles in the disc
     */
    size_t getNumberOfParticlesDisc(size_t radius) const;
    /**
     * @brief Get the number of particles in a ring of a given radius in particles
     * @param radius The radius in number of particles
     * @return The number of particles in the ring
     */
    size_t getNumberOfParticlesRing(size_t radius) const;

    /**
     * @brief Generate a disc into the particles vector starting at the index provided
     * @param particles The vector reference into which to insert
     * @param insertionIndex The index from which to start inserting from
     * @param radius The radius of the disc to generate
     * @param z_offset The offset along the z axis from the origin
     */
    void generateDisc(
        std::vector<Particle>& particles,
        size_t& insertionIndex,
        size_t radius,
        double z_offset) const;
    /**
     * @brief Generate a ring of particles into the particles vector starting at the index provided
     * @param particles The vector reference into which to insert
     * @param insertionIndex The index from which to start inserting from
     * @param radius The radius of the ring to generate
     * @param z_offset The offset along the z axis from the origin
     */
    void generateRing(
        std::vector<Particle>& particles,
        size_t& insertionIndex,
        size_t radius,
        double z_offset) const;
};
