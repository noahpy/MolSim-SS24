
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/Particle.h"
#include "physics/boundaryConditions/BoundaryConfig.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include "simulation/MembraneSimulation.h"
#include <cmath>
#include <gtest/gtest.h>
#include <models/generators/CuboidParticleCluster.h>
#include <models/generators/ParticleGenerator.h>

class cuboidToMembrane : public ::testing::Test {
protected:
    double start_time;
    double end_time;
    double delta_t;
    double PRESICION = 10e-5;
    std::unique_ptr<EmptyFileReader> fileReader;
    std::unique_ptr<outputWriter::VTKWriter> writer;
    PhysicsStrategy strat;
    ParticleContainer container;
    double epsilon = 3.14159;
    double sigma = 1;
    double gravityConst = 0;
    std::array<double, 3> domainOrigin = { 0, 0, 0 };
    std::array<double, 3> domainSize = { 148, 148, 148 };
    const double cutoff = 2.5;
    std::array<double, 3> membraneOrigin = { 15, 15, 1.5 };
    int membraneWidth = 10;
    int membraneHeight = 10;
    double k = 1;
    double r_0 = 1;
    double membraneSpacing = 2.2;
    std::array<double, 3> cuboidOrigin = { 15, 15, 1.5 };
    int numParticlesWidth = 10;
    int numParticlesHeight = 10;
    int numParticlesDepth = 1;
    double spacing = 2.2;
    double mass = 1.0;
    std::array<double, 3> initialVelocity = { 0, 0, 0 };
    double meanVelocity = 0.0;
    size_t dimensions = 2;

    cuboidToMembrane()
        : start_time(0)
        , end_time(1000)
        , delta_t(0.014)
        , fileReader(std::make_unique<EmptyFileReader>(""))
        , writer(std::make_unique<outputWriter::VTKWriter>())
        , strat { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones }
        , container { {} }
    {
    }

    void SetUp() override
    {
        std::vector<Particle> particles = {};
        this->container = ParticleContainer { particles };

        ParticleGenerator generator { this->container };
        CuboidParticleCluster cluster = CuboidParticleCluster(
            cuboidOrigin,
            numParticlesWidth,
            numParticlesHeight,
            numParticlesDepth,
            spacing,
            mass,
            initialVelocity,
            meanVelocity,
            dimensions);
        generator.registerCluster(std::make_unique<CuboidParticleCluster>(cluster));
        generator.generateClusters();
    }
};

// Test force calculations for linear membrane
TEST_F(cuboidToMembrane, CuboidToMembrane)
{
    std::map<unsigned, std::pair<double, double>> LJParams {
            { 1, { epsilon, sigma }}, { 2, { epsilon, sigma }}, { 3, { epsilon, sigma } }
    };

    EXPECT_EQ(container.particles.empty(), false);

    MembraneSimulation sim(
        start_time,
        delta_t,
        end_time,
        container,
        strat,
        std::move(writer),
        std::move(fileReader),
        LJParams,
        domainOrigin,
        domainSize,
        cutoff,
        membraneOrigin,
        membraneWidth,
        membraneHeight,
        k,
        r_0,
        membraneSpacing,
        BoundaryConfig(
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW,
            BoundaryType::OUTFLOW),
        gravityConst,
        0,
        0,
        0);

    std::array<int, 900> expectedIds = {};
    for (int i = 0; i < 900; i++) {
        expectedIds[i] = i;
    }

    int i = 0;
    for (auto& p : container.particles) {
        EXPECT_EQ(p.getMembraneId() == expectedIds[i], true);
        ++i;
    }
}
