
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
    }

    void SetUp() override
    {
        std::vector<Particle> particles = {};
        this->container = ParticleContainer { particles };

        ParticleGenerator generator { this->container };
    }
};

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

// Test if the 2D sphere is stable i.e. the particles do not move more than 5% out of the radius during simulation
TEST_F(SphereParticleGeneratorTest, Cluster2DStable)
{
    size_t radius = 10;
    double delta_t = 0.0014;
    double end_time = delta_t * 1000;

    double tolerance = 1.05;

    ParticleGenerator generator { container };
    SphereParticleCluster cluster (
        origin,
        radius,
        2,
        spacing,
        mass,
        initialVelocity,
        meanVelocity,
        2);
    generator.registerCluster(std::make_unique<SphereParticleCluster>(cluster));
    generator.generateClusters();

    // Initialize reader
    EmptyFileReader fileReader("");
    // Initialize writer
    EmptyFileWriter writer;

    auto readerPointer = std::make_unique<EmptyFileReader>(fileReader);
    auto writerPointer = std::make_unique<EmptyFileWriter>(writer);

    // Intialize physics strategy
    PhysicsStrategy strat { location_stroemer_verlet,
                            velocity_stroemer_verlet,
                            force_lennard_jones };


    // Setup simulation
    LennardJonesSimulation sim (0, delta_t, end_time, container, strat, std::move(writerPointer), std::move(readerPointer), 5, 1 );

    double time = 0;
    int iteration = 0;
    while (time < end_time) {

        if (iteration++ % 10 == 0) {
            // Check if distance is leq than 105% of the radius
            for (auto& p : container) {
                double dist = ArrayUtils::L2Norm(p.getX() - origin);
                EXPECT_LE(dist, radius * spacing * tolerance);
            }
        }

        strat.calF(sim);
        strat.calV(sim);
        strat.calX(sim);

        time += delta_t;
    }
}

// Test if 3D sphere is stable i.e. the particles do not move more than 5% out of the radius during simulation
TEST_F(SphereParticleGeneratorTest, Cluster3DStable)
{
    size_t radius = 7;
    double delta_t = 0.0014;
    double end_time = delta_t * 1000;

    double tolerance = 1.05;

    ParticleGenerator generator { container };
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

    // Initialize reader
    EmptyFileReader fileReader("");
    // Initialize writer
    EmptyFileWriter writer;

    auto readerPointer = std::make_unique<EmptyFileReader>(fileReader);
    auto writerPointer = std::make_unique<EmptyFileWriter>(writer);

    // Intialize physics strategy
    PhysicsStrategy strat { location_stroemer_verlet,
                            velocity_stroemer_verlet,
                            force_lennard_jones };


    // Setup simulation
    LennardJonesSimulation sim (0, delta_t, end_time, container, strat, std::move(writerPointer), std::move(readerPointer), 5, 1 );

    // Do the simulation and check in between
    double time = 0;
    int iteration = 0;
    while (time < end_time) {

        if (++iteration % 100 == 0) {
            // Check if distance is leq than 105% of the radius
            for (auto& p : container) {
                double dist = ArrayUtils::L2Norm(p.getX() - origin);
                EXPECT_LE(dist, radius * spacing * tolerance);
            }
        }

        strat.calF(sim);
        strat.calV(sim);
        strat.calX(sim);

        time += delta_t;
    }
}

// Test if number of generated particles equals predicted amount
TEST_F(SphereParticleGeneratorTest, CorrectNumberOfParticles)
{
    spdlog::set_level(spdlog::level::off);

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
