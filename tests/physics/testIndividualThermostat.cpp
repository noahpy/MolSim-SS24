
#include "analytics/Analyzer.h"
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/generators/ParticleGenerator.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/thermostat/IndividualThermostat.h"
#include "physics/thermostat/ThermostatFactory.h"
#include "physics/velocityCal/velocityCal.h"
#include "utils/ArrayUtils.h"
#include "utils/Params.h"
#include <gtest/gtest.h>

class IndividualThermostatTester : public ::testing::Test {
protected:
    double start_time;
    double end_time;
    double delta_t;
    std::unique_ptr<EmptyFileReader> fileReader;
    std::unique_ptr<outputWriter::VTKWriter> writer;
    PhysicsStrategy strat;
    std::array<double, 3> domainOrigin = { 0, 0, 0 };
    std::array<double, 3> domainSize = { 10, 10, 10 };
    const double cutoff = 2.5;
    std::map<unsigned, std::pair<double, double>> LJParams { { 3, { 1, 2 } }, { 1, { 2, 3 } } };
    std::map<unsigned, bool> stationaryParticleTypes = { { 1, true } };
    Particle p1 { { 1, 5, 1 }, { 3, 0, 4 }, 1, 3 };
    Particle p2 { { 7, 5, 1 }, { 9, 0, 0 }, 2, 3 };
    Particle p3 { { 1, 5, 7 }, { 0, 0, 76 }, 3, 3 };
    ParticleContainer particles = std::vector<Particle> { p1, p2, p3 };

    IndividualThermostatTester()
        : start_time(0)
        , end_time(0.05)
        , delta_t(0.0005)
        , fileReader(std::make_unique<EmptyFileReader>(""))
        , writer(std::make_unique<outputWriter::VTKWriter>())
        , strat { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones }
    {
        spdlog::set_level(spdlog::level::off);

        CuboidParticleCluster cCluster(
            { 1, 1, 1 }, 3, 3, 3, 0.5, 1, { 0, 0, 0 }, 0, 3, stationaryParticleTypes, 1);

        ParticleGenerator generator(particles);
        generator.registerCluster(std::make_unique<CuboidParticleCluster>(cCluster));
        generator.generateClusters();

        particles.addParticle(p1);
        particles.addParticle(p2);
        particles.addParticle(p3);
    }
};

// Test if total kinetic energy is calculated correctly (ignoring stationary particles + *2 factor)
// and also test if get mean velocity is calculated correctly
TEST_F(IndividualThermostatTester, testCorrectTotalKineticEnergy)
{
    std::array<size_t, 3> bins = { 0, 0, 0 };
    std::unique_ptr<Analyzer> analyzerPtr = std::make_unique<Analyzer>(bins, "");

    MixedLJSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        stationaryParticleTypes,
        LJParams,
        domainOrigin,
        domainSize,
        cutoff,
        BoundaryConfig(
            BoundaryType::SOFT_REFLECTIVE,
            BoundaryType::SOFT_REFLECTIVE,
            BoundaryType::SOFT_REFLECTIVE,
            BoundaryType::SOFT_REFLECTIVE,
            BoundaryType::SOFT_REFLECTIVE,
            BoundaryType::SOFT_REFLECTIVE),
        std::move(analyzerPtr),
        0,
        thermostatFactory(ThermostatType::NONE, 0, 0, 0, 0),
        10000,
        10,
        10000);

    IndividualThermostat thermostat(40, 40, 40, 3);
    thermostat.initialize(sim); // to get correct results
    double totalKineticEnergy = thermostat.getTotalKineticEnergy(sim);

    // Calculate expected total kinetic energy
    std::array<double, 3> meanVelocity = p1.getV() + p2.getV() + p3.getV();
    meanVelocity = (1.0 / 3.0) * meanVelocity;
    // Test if the mean velocity is calculated correctly
    for (size_t i=0; i<3; i++)
        EXPECT_NEAR(meanVelocity[i], thermostat.getMeanVelocity(sim)[i], 1e-8);

    double expectedTotalKineticEnergy =
        p1.getM() * ArrayUtils::DotProduct(p1.getV() - meanVelocity) +
        p2.getM() * ArrayUtils::DotProduct(p2.getV() - meanVelocity) +
        p3.getM() * ArrayUtils::DotProduct(p3.getV() - meanVelocity);
    // expectedTotalKineticEnergy /= 2; not required, as the temperature divides by 2 as well

    EXPECT_NEAR(totalKineticEnergy, expectedTotalKineticEnergy, 1e-8);
    // The kinetic energy should be greater than 0, as the velocity is greater than 0
    EXPECT_GT(totalKineticEnergy, 0);
    // There should be other, stationary particles as well
    EXPECT_GT(sim.container.getContainer().size(), 3);
}

// Test if updateT only modifies non-stationary particles and does the updates correctly
TEST_F(IndividualThermostatTester, testUpdateT)
{
    std::array<size_t, 3> bins = { 0, 0, 0 };
    std::unique_ptr<Analyzer> analyzerPtr = std::make_unique<Analyzer>(bins, "");

    MixedLJSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        stationaryParticleTypes,
        LJParams,
        domainOrigin,
        domainSize,
        cutoff,
        BoundaryConfig(
            BoundaryType::SOFT_REFLECTIVE,
            BoundaryType::SOFT_REFLECTIVE,
            BoundaryType::SOFT_REFLECTIVE,
            BoundaryType::SOFT_REFLECTIVE,
            BoundaryType::SOFT_REFLECTIVE,
            BoundaryType::SOFT_REFLECTIVE),
        std::move(analyzerPtr),
        0,
        thermostatFactory(ThermostatType::NONE, 0, 0, 0, 0),
        10000,
        10,
        10000);

    IndividualThermostat thermostat(1037, 1020, 40, 3);
    thermostat.initialize(sim); // to get correct results

    // tests in the test case before, we assume it is valid
    double T_current = thermostat.getTotalKineticEnergy(sim) / (3 * 3);
    double beta = sqrt(1020/T_current);
    // Test if beta is calculated correctly
    EXPECT_NEAR(beta, thermostat.getBeta(T_current), 1e-8);

    // This is also tested, and we assume it to be correct
    std::array<double, 3> meanVelocity = thermostat.getMeanVelocity(sim);

    std::array<double, 3> expectedP1Velocity = meanVelocity + beta * (p1.getV() - meanVelocity);
    std::array<double, 3> expectedP2Velocity = meanVelocity + beta * (p2.getV() - meanVelocity);
    std::array<double, 3> expectedP3Velocity = meanVelocity + beta * (p3.getV() - meanVelocity);

    thermostat.updateT(sim);
    // Test if the velocities are updated correctly
    for (size_t i=0; i<3; i++)
    {
        // The particles are added in the back of the container. Ugly but works for tests
        EXPECT_NEAR(sim.container.particles[sim.container.particles.size()-3].getV()[i], expectedP1Velocity[i], 1e-8);
        EXPECT_NEAR(sim.container.particles[sim.container.particles.size()-2].getV()[i], expectedP2Velocity[i], 1e-8);
        EXPECT_NEAR(sim.container.particles[sim.container.particles.size()-1].getV()[i], expectedP3Velocity[i], 1e-8);
    }
}