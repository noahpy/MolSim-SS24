
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/Particle.h"
#include "models/molecules/Membrane.h"
#include "physics/boundaryConditions/BoundaryConfig.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
#include "physics/thermostat/ThermostatFactory.h"
#include "physics/velocityCal/velocityCal.h"
#include "simulation/MembraneSimulation.h"
#include "simulation/baseSimulation.h"
#include "utils/ArrayUtils.h"
#include <cmath>
#include <gtest/gtest.h>

class calcForceMembrane : public ::testing::Test {
protected:
    double start_time;
    double end_time;
    double delta_t;
    double PRESICION = 1e-8;
    std::unique_ptr<EmptyFileReader> fileReader;
    std::unique_ptr<outputWriter::VTKWriter> writer;
    PhysicsStrategy strat;
    ParticleContainer particles;
    std::array<double, 3> domainOrigin = { -5, -5, -5 };
    std::array<double, 3> domainSize = { 10, 10, 10 };
    const double cutoff = 2.5;
    std::array<double, 3> membraneOrigin = { 0, 0, 0 };
    int numParticlesWidth = 3;
    int numParticlesHeight = 1;
    double k = 1;
    double r_0 = 1;
    double spacing = 2;
    std::array<double, 3> z = { 0, 0, 0 };

    calcForceMembrane()
        : start_time(0)
        , end_time(1000)
        , delta_t(0.014)
        , fileReader(std::make_unique<EmptyFileReader>(""))
        , writer(std::make_unique<outputWriter::VTKWriter>())
        , strat { location_stroemer_verlet, velocity_stroemer_verlet, force_lennard_jones }
        , particles { {} }
    {
        spdlog::set_level(spdlog::level::off);
    }
};

// Test intra molecular force calculations for linear molecules against hand calculated values
TEST_F(calcForceMembrane, calcForceMembraneLinear)
{
    double r0 = 2;
    double r = 1;
    double k = 20;

    ParticleContainer container { {} };
    Membrane mem { z, 2, 1, 1, r, 1, z, 0, 3, 1, r0, k };
    std::unique_ptr<Molecule> ptr = std::make_unique<Membrane>(mem);
    std::vector<std::unique_ptr<Molecule>> memVec {};
    memVec.push_back(std::move(ptr));

    Analyzer analyzer({ 1, 0, 4 }, "");
    std::unique_ptr<Analyzer> analyzerPtr = std::make_unique<Analyzer>(analyzer);

    MembraneSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        {},
        { { 1, { 1, 1 } } },
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
        std::move(analyzerPtr),
        0,
        thermostatFactory(ThermostatType::CLASSICAL, 0, 0, 0, 0),
        std::move(memVec));

    // Check if the number of particles are correct
    EXPECT_EQ(sim.container.particles.size(), 2);

    sim.getMolecules()[0]->calculateIntraMolecularForces(sim);

    // This is simply the harmonic potential
    std::array<double, 3> expectedForce =
        k / r * (r - r0) * (sim.container.particles[1].getX() - sim.container.particles[0].getX());

    for (int i = 0; i < 3; i++) {
        // Newtons third law
        EXPECT_NEAR(sim.container.particles[0].getF().at(i), expectedForce.at(i), PRESICION);
        EXPECT_NEAR(sim.container.particles[1].getF().at(i), -expectedForce.at(i), PRESICION);
    }
}

// Test intra molecular force calculations for diagonal molecules against hand calculated values
TEST_F(calcForceMembrane, calcForceMembraneDiagonal)
{
    double r0 = 2;
    double r = 1;
    double k = 20;

    particles = std::vector<Particle> {};
    ParticleContainer container { particles };
    Membrane mem { z, 2, 2, 1, r, 1, z, 0, 3, 1, r0, k };
    std::unique_ptr<Molecule> ptr = std::make_unique<Membrane>(mem);
    std::vector<std::unique_ptr<Molecule>> memVec {};
    memVec.push_back(std::move(ptr));

    Analyzer analyzer({ 1, 0, 4 }, "");
    std::unique_ptr<Analyzer> analyzerPtr = std::make_unique<Analyzer>(analyzer);

    MembraneSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        {},
        { { 1, { 1, 0.1 } } },
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
        std::move(analyzerPtr),
        0,
        thermostatFactory(ThermostatType::CLASSICAL, 0, 0, 0, 0),
        std::move(memVec));

    sim.getMolecules()[0]->calculateIntraMolecularForces(sim);

    // Check if the number of particles are correct
    EXPECT_EQ(sim.container.particles.size(), 4);

    // lambda function to do harmonic potential
    auto harmonicPotential = [](Particle& p1, Particle& p2, double r0, double k) {
        return k / ArrayUtils::L2Norm(p2.getX() - p1.getX()) *
               (ArrayUtils::L2Norm(p2.getX() - p1.getX()) - r0) * (p2.getX() - p1.getX());
    };

    // Generate expected forces
    std::vector<std::array<double, 3>> expectedForces(sim.container.particles.size(), { 0, 0, 0 });
    for (int i = 0; i < sim.container.particles.size(); i++)
        for (int j = 0; j < sim.container.particles.size(); j++)
            if (i != j) {
                // The r0 to use depends on if the particles are aligned diagonally
                double actualR0 =
                    (sim.container.particles[i].getX()[0] == sim.container.particles[j].getX()[0] ||
                     sim.container.particles[i].getX()[1] == sim.container.particles[j].getX()[1])
                        ? r0
                        : std::sqrt(2) * r0;
                expectedForces[i] =
                    expectedForces[i] +
                    harmonicPotential(sim.container.particles[i], sim.container.particles[j], actualR0, k);
            }

    // Check if the forces match
    for (int i = 0; i < sim.container.particles.size(); i++)
        for (int j = 0; j < 3; j++)
            EXPECT_NEAR(sim.container.particles[i].getF().at(j), expectedForces[i].at(j), PRESICION);
}

// Test intra molecular forces and leaving LJ forces for membrane particles that are neighbors
// Exact same as above, only change sigma to make LJ calc appear
TEST_F(calcForceMembrane, calcForceMembraneLJNeighbors)
{
    double r0 = 2;
    double r = 1;
    double k = 20;

    particles = std::vector<Particle> {};
    ParticleContainer container { particles };
    Membrane mem { z, 2, 2, 1, r, 1, z, 0, 3, 1, r0, k };
    std::unique_ptr<Molecule> ptr = std::make_unique<Membrane>(mem);
    std::vector<std::unique_ptr<Molecule>> memVec {};
    memVec.push_back(std::move(ptr));

    Analyzer analyzer({ 1, 0, 4 }, "");
    std::unique_ptr<Analyzer> analyzerPtr = std::make_unique<Analyzer>(analyzer);

    MembraneSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        {},
        { { 1, { 1, 2 } } },
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
        std::move(analyzerPtr),
        0,
        thermostatFactory(ThermostatType::CLASSICAL, 0, 0, 0, 0),
        std::move(memVec));

    sim.getMolecules()[0]->calculateIntraMolecularForces(sim);

    // Check if the number of particles are correct
    EXPECT_EQ(sim.container.particles.size(), 4);

    // lambda function to do harmonic potential
    auto harmonicPotential = [](Particle& p1, Particle& p2, double r0, double k) {
        return k / ArrayUtils::L2Norm(p2.getX() - p1.getX()) *
               (ArrayUtils::L2Norm(p2.getX() - p1.getX()) - r0) * (p2.getX() - p1.getX());
    };

    // Generate expected forces
    std::vector<std::array<double, 3>> expectedForces(sim.container.particles.size(), { 0, 0, 0 });
    for (int i = 0; i < sim.container.particles.size(); i++)
        for (int j = 0; j < sim.container.particles.size(); j++)
            if (i != j) {
                // The r0 to use depends on if the particles are aligned diagonally
                double actualR0 =
                    (sim.container.particles[i].getX()[0] == sim.container.particles[j].getX()[0] ||
                     sim.container.particles[i].getX()[1] == sim.container.particles[j].getX()[1])
                        ? r0
                        : std::sqrt(2) * r0;
                expectedForces[i] =
                    expectedForces[i] +
                    harmonicPotential(sim.container.particles[i], sim.container.particles[j], actualR0, k);
            }

    // Check if the forces match -> as LJ was not added because direct neighbor
    for (int i = 0; i < sim.container.particles.size(); i++)
        EXPECT_FALSE(
            ArrayUtils::L2Norm(sim.container.particles[i].getF() - expectedForces[i]) > PRESICION);
}

// Test intra molecular forces and LJ forces for membrane particles that are not neighbors
TEST_F(calcForceMembrane, calcForceMembraneLJNonNeighbors)
{
    double r0 = 2;
    double r = 1;
    double k = 20;

    particles = std::vector<Particle> {};
    ParticleContainer container { particles };
    Membrane mem { z, 4, 4, 1, r, 1, z, 0, 3, 1, r0, k };
    std::unique_ptr<Molecule> ptr = std::make_unique<Membrane>(mem);
    std::vector<std::unique_ptr<Molecule>> memVec {};
    memVec.push_back(std::move(ptr));

    Analyzer analyzer({ 1, 0, 4 }, "");
    std::unique_ptr<Analyzer> analyzerPtr = std::make_unique<Analyzer>(analyzer);

    MembraneSimulation sim(
        start_time,
        delta_t,
        end_time,
        particles,
        strat,
        std::move(writer),
        std::move(fileReader),
        {},
        { { 1, { 1, 7 } } },
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
        std::move(analyzerPtr),
        0,
        thermostatFactory(ThermostatType::CLASSICAL, 0, 0, 0, 0),
        std::move(memVec));

    sim.getMolecules()[0]->calculateIntraMolecularForces(sim);

    // Check if the number of particles are correct
    EXPECT_EQ(sim.container.particles.size(), 16);

    // lambda function to do harmonic potential
    auto harmonicPotential = [](Particle& p1, Particle& p2, double r0, double k) {
        return k / ArrayUtils::L2Norm(p2.getX() - p1.getX()) *
               (ArrayUtils::L2Norm(p2.getX() - p1.getX()) - r0) * (p2.getX() - p1.getX());
    };

    // Generate expected forces
    std::vector<std::array<double, 3>> expectedForces(sim.container.particles.size(), { 0, 0, 0 });
    for (int i = 0; i < sim.container.particles.size(); i++)
        for (int j = 0; j < sim.container.particles.size(); j++)
            if (i != j) {
                // The r0 to use depends on if the particles are aligned diagonally
                double actualR0 =
                    (sim.container.particles[i].getX()[0] == sim.container.particles[j].getX()[0] ||
                     sim.container.particles[i].getX()[1] == sim.container.particles[j].getX()[1])
                        ? r0
                        : std::sqrt(2) * r0;
                expectedForces[i] =
                    expectedForces[i] +
                    harmonicPotential(sim.container.particles[i], sim.container.particles[j], actualR0, k);
            }

    // Check if the forces do not match -> Sigma is so big that at least one non-neighbor is close enough to act a force upon a particle
    // This force must be LJ force
    for (int i = 0; i < sim.container.particles.size(); i++)
        EXPECT_TRUE(
            ArrayUtils::L2Norm(sim.container.particles[i].getF() - expectedForces[i]) > PRESICION);
}
