
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/Particle.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include "simulation/baseSimulation.h"
#include "simulation/linkedLennardJonesSim.h"
#include <cmath>
#include <gtest/gtest.h>

class calcForceLJTest : public ::testing::Test {
protected:
    double start_time;
    double end_time;
    double delta_t;
    double PRESICION = 10e-5;
    std::unique_ptr<EmptyFileReader> fileReader;
    std::unique_ptr<outputWriter::VTKWriter> writer;
    PhysicsStrategy strat;
    ParticleContainer particles;

    calcForceLJTest()
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

// Particles with normed distance, epsilon, sigma should be calculated correctly
TEST_F(calcForceLJTest, calcForceLJNormed)
{
    double c = std::sqrt(3) / 4;
    // Form equilateral triangle
    Particle p1 { { 0, 0, c }, { 0, 0, 0 }, 1 };
    Particle p2 { { 0, 0.5, -c }, { 0, 0, 0 }, 1 };
    Particle p3 { { 0, -0.5, -c }, { 0, 0, 0 }, 1 };

    std::array<double, 3> p1F { 0, 0, 24 * 4 * c };
    std::array<double, 3> p2F { 0, 24 * 1.5, 24 * -2 * c };
    std::array<double, 3> p3F { 0, 24 * -1.5, 24 * -2 * c };
    std::array<std::array<double, 3>, 3> expectedFs = { p1F, p2F, p3F };

    double epsilon = 1;
    double sigma = 1;

    LennardJonesSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        {},
        epsilon,
        sigma);

    particles.particles = { p1, p2, p3 };

    force_lennard_jones(sim);

    unsigned pCount = 0;
    for (auto p : particles) {
        for (unsigned i = 0; i < 3; i++) {
            EXPECT_NEAR(p.getF().at(i), expectedFs.at(pCount).at(i), PRESICION)
                << "Particle " << pCount << " Dimension " << i << ": Expected "
                << expectedFs.at(pCount).at(i) << " but got: " << p.getF().at(i);
        }
        ++pCount;
    }
}

// Particles should not experience force when in equilibrium
TEST_F(calcForceLJTest, calcForceLJEquilibrium)
{
    double c = std::sqrt(3) / 4;
    // Form equilateral triangle
    Particle p1 { { 0, 0, c }, { 0, 0, 0 }, 1 };
    Particle p2 { { 0, 0.5, -c }, { 0, 0, 0 }, 1 };
    Particle p3 { { 0, -0.5, -c }, { 0, 0, 0 }, 1 };

    std::array<double, 3> p1F { 0, 0, 0 };
    std::array<double, 3> p2F { 0, 0, 0 };
    std::array<double, 3> p3F { 0, 0, 0 };
    std::array<std::array<double, 3>, 3> expectedFs = { p1F, p2F, p3F };

    double epsilon = 3.14159;
    double sigma = 0.8908987181403393;

    LennardJonesSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        {},
        epsilon,
        sigma);

    particles.particles = { p1, p2, p3 };

    force_lennard_jones(sim);

    unsigned pCount = 0;
    for (auto p : particles) {
        for (unsigned i = 0; i < 3; i++) {
            EXPECT_NEAR(p.getF().at(i), expectedFs.at(pCount).at(i), PRESICION)
                << "Particle " << pCount << " Dimension " << i << ": Expected "
                << expectedFs.at(pCount).at(i) << " but got: " << p.getF().at(i);
        }
        ++pCount;
    }
}

// Particles with normed distance and sigma should be calculated correctly
TEST_F(calcForceLJTest, calcForceLJUnNormed)
{
    double c = std::sqrt(3) / 4;
    // Form equilateral triangle
    Particle p1 { { 0, 0, c }, { 0, 0, 0 }, 1 };
    Particle p2 { { 0, 0.5, -c }, { 0, 0, 0 }, 1 };
    Particle p3 { { 0, -0.5, -c }, { 0, 0, 0 }, 1 };

    double epsilon = 3.14159;
    double sigma = 1;

    std::array<double, 3> p1F { 0, 0, epsilon * 24 * 4 * c };
    std::array<double, 3> p2F { 0, epsilon * 24 * 1.5, epsilon * 24 * -2 * c };
    std::array<double, 3> p3F { 0, epsilon * 24 * -1.5, epsilon * 24 * -2 * c };
    std::array<std::array<double, 3>, 3> expectedFs = { p1F, p2F, p3F };

    LennardJonesSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        {},
        epsilon,
        sigma);

    particles.particles = { p1, p2, p3 };

    force_lennard_jones(sim);

    unsigned pCount = 0;
    for (auto p : particles) {
        for (unsigned i = 0; i < 3; i++) {
            EXPECT_NEAR(p.getF().at(i), expectedFs.at(pCount).at(i), PRESICION)
                << "Particle " << pCount << " Dimension " << i << ": Expected "
                << expectedFs.at(pCount).at(i) << " but got: " << p.getF().at(i);
        }
        ++pCount;
    }
}
