
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/Particle.h"
#include "physics/boundaryConditions/BoundaryConfig.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include "simulation/MixedLJSimulation.h"
#include "simulation/baseSimulation.h"
#include <cmath>
#include <gtest/gtest.h>

class calcMixedForceLJ : public ::testing::Test {
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

    calcMixedForceLJ()
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

TEST_F(calcMixedForceLJ, calcForceLJUnNormed)
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

    MixedLJSimulation sim(
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

    force_mixed_LJ_gravity_lc(sim);

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
