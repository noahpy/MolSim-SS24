
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/Particle.h"
#include "physics/boundaryConditions/BoundaryConfig.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include "simulation/MembraneSimulation.h"
#include "simulation/baseSimulation.h"
#include <cmath>
#include <gtest/gtest.h>

class calcForceMembrane : public ::testing::Test {
protected:
    double start_time;
    double end_time;
    double delta_t;
    double PRESICION = 10e-5;
    std::unique_ptr<EmptyFileReader> fileReader;
    std::unique_ptr<outputWriter::VTKWriter> writer;
    PhysicsStrategy strat;
    ParticleContainer particles;
    std::array<double, 3> domainOrigin = { -5, -5, -5 };
    std::array<double, 3> domainSize = { 10, 10, 10 };
    const double cutoff = 2.5;
    std::array<double, 3> membraneOrigin = { 0, 0, 0};
    int numParticlesWidth = 3;
    int numParticlesHeight = 1;
    double k = 1;
    double r_0 = 1;
    double spacing = 2;

    calcForceMembrane()
        : start_time(0)
        , end_time(1000)
        , delta_t(0.014)
        , fileReader(std::make_unique<EmptyFileReader>(""))
        , writer(std::make_unique<outputWriter::VTKWriter>())
        , strat { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones }
        , particles { {} }
    {
    }
};

// Test force calculations for linear membrane
TEST_F(calcForceMembrane, calcForceMembrane)
{
    Particle p1 { { 0, 0, 0 }, { 0, 0, 0 }, 1, 3 };
    Particle p2 { { 2, 0, 0 }, { 0, 0, 0 }, 1, 3 };
    Particle p3 { { 4, 0, 0 }, { 0, 0, 0 }, 1, 3 };

    particles = std::vector<Particle> { p1, p2, p3 };

    double epsilon = 3.14159;
    double sigma = 1;
    double gravityConst = 0;

    std::array<double, 3> p1F { 1, 0, 0 };
    std::array<double, 3> p2F { 0, 0, 0 };
    std::array<double, 3> p3F { -1, 0, 0 };
    std::array<std::array<double, 3>, 3> expectedFs = { p1F, p2F, p3F };

    std::map<unsigned, std::pair<double, double>> LJParams {
        { 1, { epsilon, sigma }}, { 2, { epsilon, sigma }}, { 3, { epsilon, sigma } }
    };

    MembraneSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        LJParams,
        domainOrigin,
        domainSize,
        cutoff,
        membraneOrigin,
        numParticlesWidth,
        numParticlesHeight,
        k,
        r_0,
        spacing,
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

    force_membrane(sim);

    unsigned pCount = 0;
    for (auto& p : particles) {
        for (unsigned i = 0; i < 3; i++) {
            EXPECT_NEAR(p.getF().at(i), expectedFs.at(pCount).at(i), PRESICION)
                << "Particle " << pCount << " Dimension " << i << ": Expected "
                << expectedFs.at(pCount).at(i) << " but got: " << p.getF().at(i);
        }
        ++pCount;
    }
}

// Test forces for diagonal membrane particles
TEST_F(calcForceMembrane, calcForceMembraneDiagonal)
{
    // Align particles in a diagonal manner inside the membrane
    Particle p1 { { 0, 0, 0 }, { 0, 0, 0 }, 1, 3 };
    Particle p2 { { 2, 2, 0 }, { 0, 0, 0 }, 1, 3 };
    Particle p3 { { 4, 0, 0 }, { 0, 0, 0 }, 1, 3 };

    particles = std::vector<Particle> { p1, p2, p3 };

    double epsilon = 3.14159;
    double sigma = 1;
    double gravityConst = 0;

    std::array<double, 3> p1F { 1, 1, 0 };
    std::array<double, 3> p2F { 0, -2, 0 };
    std::array<double, 3> p3F { -1, 1, 0 };
    std::array<std::array<double, 3>, 3> expectedFs = { p1F, p2F, p3F };

    std::map<unsigned, std::pair<double, double>> LJParams {
            { 1, { epsilon, sigma }}, { 2, { epsilon, sigma }}, { 3, { epsilon, sigma } }
    };

    MembraneSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        LJParams,
        domainOrigin,
        domainSize,
        cutoff,
        membraneOrigin,
        numParticlesWidth,
        2,
        k,
        r_0,
        spacing,
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

    force_membrane(sim);

    unsigned pCount = 0;
    for (auto& p : particles) {
        for (unsigned i = 0; i < 3; i++) {
            EXPECT_NEAR(p.getF().at(i), expectedFs.at(pCount).at(i), PRESICION)
                << "Particle " << pCount << " Dimension " << i << ": Expected "
                << expectedFs.at(pCount).at(i) << " but got: " << p.getF().at(i);
        }
        ++pCount;
    }
}

// Test forces between membrane particle and non-membrane particles
TEST_F(calcForceMembrane, calcForceMembraneLJ)
{
    // Particles with normed distance and sigma should be calculated correctly
    double c = std::sqrt(3) / 4;
    // Form equilateral triangle
    Particle p1 { { 0, 0, c }, { 0, 0, 0 }, 1, 3 };
    Particle p2 { { 0, 0.5, -c }, { 0, 0, 0 }, 2, 3 };
    Particle p3 { { 0, -0.5, -c }, { 0, 0, 0 }, 3, 3 };

    particles = std::vector<Particle> { p1, p2, p3 };

    double epsilon = 3.14159;
    double sigma = 1;
    double gravityConst = 9;

    std::array<double, 3> p1F { 0, 0 + 1 * gravityConst, epsilon * 24 * 4 * c };
    std::array<double, 3> p2F { 0, epsilon * 24 * 1.5 + 2 * gravityConst, epsilon * 24 * -2 * c };
    std::array<double, 3> p3F { 0, epsilon * 24 * -1.5 + 3 * gravityConst, epsilon * 24 * -2 * c };
    std::array<std::array<double, 3>, 3> expectedFs = { p1F, p2F, p3F };

    std::map<unsigned, std::pair<double, double>> LJParams {
                { 1, { epsilon, sigma }}, { 2, { epsilon, sigma }}, { 3, { epsilon, sigma } }
    };

    MembraneSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        LJParams,
        domainOrigin,
        domainSize,
        cutoff,
        { 0, 0, c },
        1,
        numParticlesHeight,
        k,
        r_0,
        1,
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

    force_membrane(sim);

    EXPECT_EQ(sim.container.particles[0].getMembraneId() != -1, true);
    unsigned pCount = 0;
    for (auto& p : particles) {
        for (unsigned i = 0; i < 3; i++) {
            EXPECT_NEAR(p.getF().at(i), expectedFs.at(pCount).at(i), PRESICION)
                << "Particle " << pCount << " Dimension " << i << ": Expected "
                << expectedFs.at(pCount).at(i) << " but got: " << p.getF().at(i);
        }
        ++pCount;
    }
}

// Test forces between membrane particle and non-membrane particles
TEST_F(calcForceMembrane, calcForceMembraneTruncatedLJ)
{
    Particle p1 { { 0, 0, 0 }, { 0, 0, 0 }, 1, 1 };
    Particle p2 { { 1, 0, 0 }, { 0, 0, 0 }, 1, 1 };

    particles = std::vector<Particle> { p1, p2 };

    double epsilon = 1;
    double sigma = 1;
    double gravityConst = 0;

    std::array<double, 3> p1F { -24, 0, 0 };
    std::array<double, 3> p2F { 24, 0, 0 };

    std::array<std::array<double, 3>, 3> expectedFs = { p1F, p2F };

    std::map<unsigned, std::pair<double, double>> LJParams {
                { 1, { epsilon, sigma }}, { 2, { epsilon, sigma }}, { 3, { epsilon, sigma } }
    };

    MembraneSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        LJParams,
        domainOrigin,
        domainSize,
        cutoff,
        { 0, 0, 0 },
        1,
        numParticlesHeight,
        k,
        r_0,
        1,
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

    sim.container.particles[1].setMembraneId(2);

    force_membrane(sim);

    EXPECT_EQ(sim.container.particles[0].getMembraneId() != -1, true);
    unsigned pCount = 0;
    for (auto& p : particles) {
        for (unsigned i = 0; i < 3; i++) {
            EXPECT_NEAR(p.getF().at(i), expectedFs.at(pCount).at(i), PRESICION)
                << "Particle " << pCount << " Dimension " << i << ": Expected "
                << expectedFs.at(pCount).at(i) << " but got: " << p.getF().at(i);
        }
        ++pCount;
    }
}


// Test forces between membrane particle and non-membrane particles
TEST_F(calcForceMembrane, calcForceMembraneTruncatedLJ2)
{
    Particle p1 { { 0, 0, 0 }, { 0, 0, 0 }, 1, 1 };
    Particle p2 { { 2, 0, 0 }, { 0, 0, 0 }, 1, 1 };

    particles = std::vector<Particle> { p1, p2 };

    double epsilon = 1;
    double sigma = 1;
    double gravityConst = 0;

    std::array<double, 3> p1F { 0, 0, 0 };
    std::array<double, 3> p2F { 0, 0, 0 };

    std::array<std::array<double, 3>, 3> expectedFs = { p1F, p2F };

    std::map<unsigned, std::pair<double, double>> LJParams {
                    { 1, { epsilon, sigma }}, { 2, { epsilon, sigma }}, { 3, { epsilon, sigma } }
    };

    MembraneSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        LJParams,
        domainOrigin,
        domainSize,
        cutoff,
        { 0, 0, 0 },
        1,
        numParticlesHeight,
        k,
        r_0,
        1,
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

    sim.container.particles[1].setMembraneId(2);

    force_membrane(sim);

    EXPECT_EQ(sim.container.particles[0].getMembraneId() != -1, true);
    unsigned pCount = 0;
    for (auto& p : particles) {
        for (unsigned i = 0; i < 3; i++) {
            EXPECT_NEAR(p.getF().at(i), expectedFs.at(pCount).at(i), PRESICION)
                << "Particle " << pCount << " Dimension " << i << ": Expected "
                << expectedFs.at(pCount).at(i) << " but got: " << p.getF().at(i);
        }
        ++pCount;
    }
}