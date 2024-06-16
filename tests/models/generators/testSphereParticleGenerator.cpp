
#include "models/generators/ParticleGenerator.h"
#include "utils/ArrayUtils.h"
#include <gtest/gtest.h>
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/emptyWriter.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/velocityCal/velocityCal.h"
#include "physics/strategy.h"

class SphereParticleGeneratorTest : public ::testing::Test {
protected:
    std::array<double, 3> origin = { 1, 2, 3 };
    double spacing = 1.0;
    double mass = 1.0;
    std::array<double, 3> initialVelocity = { 2, 3, 4 };
    double meanVelocity = 1.0;
    size_t dimensions = 3;
    ParticleContainer container;

    SphereParticleGeneratorTest()
        : origin(std::array<double, 3> { 1, 2, 3 })
        , spacing(1.1225)
        , mass(1.0)
        , initialVelocity(std::array<double, 3> { 0, 0, 0 })
        , meanVelocity(1.0)
        , dimensions(3)
        , container(ParticleContainer { std::vector<Particle> {} })
    {
        spdlog::set_level(spdlog::level::off);
    }

    void SetUp() override
    {
        std::vector<Particle> particles = {};
        this->container = ParticleContainer { particles };

        ParticleGenerator generator { this->container };
    }
};

// Test if number of generated particles equals predicted amount
TEST_F(SphereParticleGeneratorTest, CorrectNumberOfParticles)
{
    ParticleGenerator generator { container };

    for (size_t radius = 0; radius <= 20; radius++) {
        SphereParticleCluster cluster (
            origin,
            radius,
            3,
            spacing,
            mass,
            initialVelocity,
            meanVelocity,
            3);
        generator.registerCluster(std::make_unique<SphereParticleCluster>(cluster));
        generator.generateClusters();

        size_t expected = cluster.getTotalNumberOfParticles();
        EXPECT_EQ(container.particles.size(), expected);
    }
}

// Test edge cases of 0 and 1 radius
TEST_F(SphereParticleGeneratorTest, EdgeCases)
{
    ParticleGenerator generator { container };

    for (size_t radius = 0; radius <= 1; radius++) {
        SphereParticleCluster cluster (
            origin,
            radius,
            3,
            spacing,
            mass,
            initialVelocity,
            meanVelocity,
            3);
        generator.registerCluster(std::make_unique<SphereParticleCluster>(cluster));
        generator.generateClusters();

        if (radius == 0)
            EXPECT_EQ(container.particles.size(), 0);
        if (radius == 1)
            EXPECT_EQ(container.particles.size(), 1);
    }
}

// Test if all positions of particles generated in a sphere are unique
TEST_F(SphereParticleGeneratorTest, UniquePositionsInSphere)
{
    ParticleGenerator generator { this->container };
    SphereParticleCluster cluster (
        origin,
        15,
        3,
        spacing,
        mass,
        initialVelocity,
        meanVelocity,
        dimensions);
    generator.registerCluster(std::make_unique<SphereParticleCluster>(cluster));
    generator.generateClusters();

    for (auto it = container.beginPairs(); it != container.endPairs(); ++it) {
        std::pair<Particle&, Particle&> pt_pair = *it;

        std::array<double, 3> pos1 = pt_pair.first.getX(), pos2 = pt_pair.second.getX();
        double dist = ArrayUtils::L2Norm(pos1 - pos2);

        EXPECT_GT(dist, 0);
    }
}

// Test if all distances between particles is at least the spacing
TEST_F(SphereParticleGeneratorTest, MinimumSpacing)
{
    ParticleGenerator generator { this->container };
    SphereParticleCluster cluster (
        origin,
        15,
        3,
        spacing,
        mass,
        initialVelocity,
        meanVelocity,
        dimensions);
    generator.registerCluster(std::make_unique<SphereParticleCluster>(cluster));
    generator.generateClusters();

    for (auto it = container.beginPairs(); it != container.endPairs(); ++it) {
        std::pair<Particle&, Particle&> pt_pair = *it;

        std::array<double, 3> pos1 = pt_pair.first.getX(), pos2 = pt_pair.second.getX();
        double dist = ArrayUtils::L2Norm(pos1 - pos2);

        EXPECT_TRUE(dist >= spacing || std::abs(dist - spacing) < 1e-6);
    }
}

// Test if the cluster spans a sphere i.e. if the distance to the origin is less or equal to the radius
TEST_F(SphereParticleGeneratorTest, ClusterSpansSphere)
{
    size_t radius = 15;

    ParticleGenerator generator { this->container };
    SphereParticleCluster cluster (
        origin,
        radius,
        3,
        spacing,
        mass,
        initialVelocity,
        meanVelocity,
        dimensions);
    generator.registerCluster(std::make_unique<SphereParticleCluster>(cluster));
    generator.generateClusters();

    for (auto& p : container) {
        double dist = ArrayUtils::L2Norm(p.getX() - origin);
        EXPECT_LE(dist, radius * spacing);
    }
}