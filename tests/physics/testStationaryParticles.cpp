
#include "analytics/Analyzer.h"
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/velocityCal/velocityCal.h"
#include <gtest/gtest.h>
#include "models/generators/ParticleGenerator.h"
#include "physics/thermostat/ThermostatFactory.h"

class StationaryTester : public ::testing::Test {
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
    std::map<unsigned , bool> stationaryParticleTypes = {{1, true}};
    ParticleContainer particles {};
    Particle p1 { { 1, 1, 7 }, { 3, 0, 4 }, 1, 3 };
    Particle p2 { { 7, 1, 7 }, { 9, 0, 0 }, 2, 3 };
    Particle p3 { { 1, 2, 7 }, { 0, 0, 76 }, 3, 3 };
    size_t expectedStationary;

    StationaryTester()
        : start_time(0)
        , end_time(0.05)
        , delta_t(0.0005)
        , fileReader(std::make_unique<EmptyFileReader>(""))
        , writer(std::make_unique<outputWriter::VTKWriter>())
        , strat { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones }
        , expectedStationary(0)
    {
        spdlog::set_level(spdlog::level::off);

        CuboidParticleCluster cCluster ( {1,1,1}, 3, 3, 3, 0.5, 1, {0,0,0}, 0, 3, stationaryParticleTypes, 1);
        SphereParticleCluster sCluster ( {7,7,7}, 2, 3, 1, 1, {0,0,0}, 0, 3, stationaryParticleTypes, 1);

        expectedStationary += cCluster.getTotalNumberOfParticles();
        expectedStationary += sCluster.getTotalNumberOfParticles();

        ParticleGenerator generator(particles);
        generator.registerCluster(std::make_unique<CuboidParticleCluster>(cCluster));
        generator.registerCluster(std::make_unique<SphereParticleCluster>(sCluster));
        generator.generateClusters();

        particles.addParticle(p1);
        particles.addParticle(p2);
        particles.addParticle(p3);
    }
};

// Test if all stationary particles have received the correct flag
TEST_F(StationaryTester, testStationaryFlag)
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

    sim.runSim();

    size_t cStationary = 0;
    size_t cNotStationary = 0;
    for (auto& particle : particles.particles) {
        if (particle.getType() == 1) {
            // Stationary particles should have the flag NOT set
            ASSERT_FALSE(particle.getIsNotStationary());
            cStationary++;
        } else {
            // Non-stationary particles should have the flag set
            ASSERT_TRUE(particle.getIsNotStationary());
            cNotStationary++;
        }
    }

    // There should be 3 non-stationary particles
    ASSERT_EQ(cNotStationary, 3);
    // There should be #Cuboid + #Sphere stationary particles
    ASSERT_EQ(cStationary, expectedStationary);
}

// Test if the stationary particles are not moved and have zero velocity
TEST_F(StationaryTester, testStationaryMovement)
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

    std::vector<std::array<double, 3>> originalPositions;
    originalPositions.reserve(particles.particles.size());
    for (auto& particle : particles.particles) {
        originalPositions.push_back(particle.getX());
    }

    sim.runSim();

    // Check if the stationary particles have velocity 0
    for (auto& particle : particles.particles) {
        if (particle.getType() == 1) {
            for (size_t i = 0; i < 3; i++) {
                ASSERT_EQ(particle.getV()[i], 0);
            }
        }
    }

    // Check if the stationary particles have not moved
    for (size_t i = 0; i < particles.particles.size(); i++) {
        if (particles.particles[i].getType() != 1) {
            continue;
        }
        for (size_t j = 0; j < 3; j++) {
            ASSERT_EQ(particles.particles[i].getX()[j], originalPositions[i][j]);
        }
    }
}
