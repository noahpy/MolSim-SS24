
#include "io/fileReader/emptyReader.h"
#include "io/fileWriter/VTKWriter.h"
#include "models/Particle.h"
#include "models/molecules/Membrane.h"
#include "physics/boundaryConditions/BoundaryConfig.h"
#include "physics/forceCal/forceCal.h"
#include "physics/locationCal/locationCal.h"
#include "physics/strategy.h"
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
    }
};

// Test intra molecular force calculations for linear molecules against hand calculated values
TEST_F(calcForceMembrane, calcForceMembrane)
{
    double r0 = 2;
    double r = 1;
    double k = 20;

    particles = std::vector<Particle> {};
    ParticleContainer container { particles };
    Membrane mem { z, 2, 1, 1, r, 1, z, 0, 3, 1, r0, k };
    mem.generateMolecule(container, 562);

    // Check if the number of particles are correct
    EXPECT_EQ(container.particles.size(), 2);

    CellGrid grid { { -5, -5, -5 }, { 10, 10, 10 }, 5 };
    grid.addParticlesFromContainer(container);

    mem.calculateIntraMolecularForces(grid);

    // This is simply the harmonic potential
    std::array<double, 3> expectedForce =
        k / r * (r - r0) * (container.particles[1].getX() - container.particles[0].getX());

    for (int i = 0; i < 3; i++) {
        // Newtons third law
        EXPECT_NEAR(container.particles[0].getF().at(i), expectedForce.at(i), PRESICION);
        EXPECT_NEAR(container.particles[1].getF().at(i), -expectedForce.at(i), PRESICION);
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
    mem.initLJParams(1, 0.1); // small sigma to ignore repulsive forces
    mem.generateMolecule(container, 562);

    // Check if the number of particles are correct
    EXPECT_EQ(container.particles.size(), 4);

    CellGrid grid { { -5, -5, -5 }, { 10, 10, 10 }, 5 };
    grid.addParticlesFromContainer(container);

    mem.calculateIntraMolecularForces(grid);

    // lambda function to do harmonic potential
    auto harmonicPotential = [](Particle& p1, Particle& p2, double r0, double k) {
        return k / ArrayUtils::L2Norm(p2.getX() - p1.getX()) *
               (ArrayUtils::L2Norm(p2.getX() - p1.getX()) - r0) * (p2.getX() - p1.getX());
    };

    // Generate expected forces
    std::vector<std::array<double, 3>> expectedForces(container.particles.size(), { 0, 0, 0 });
    for (int i = 0; i < container.particles.size(); i++)
        for (int j = 0; j < container.particles.size(); j++)
            if (i != j) {
                // The r0 to use depends on if the particles are aligned diagonally
                double actualR0 =
                    (container.particles[i].getX()[0] == container.particles[j].getX()[0] ||
                     container.particles[i].getX()[1] == container.particles[j].getX()[1])
                        ? r0
                        : std::sqrt(2) * r0;
                expectedForces[i] =
                    expectedForces[i] +
                    harmonicPotential(container.particles[i], container.particles[j], actualR0, k);
            }

    // Check if the forces match
    for (int i = 0; i < container.particles.size(); i++)
        for (int j = 0; j < 3; j++)
            EXPECT_NEAR(container.particles[i].getF().at(j), expectedForces[i].at(j), PRESICION);
}

// Test intra molecular forces including LJ forces for membrane particles
// Exact same as above, only see if the forces are different -> thus we know the forces were changed
// and the LJ Test makes sure the calculation is correct
TEST_F(calcForceMembrane, calcForceMembraneLJ)
{
    double r0 = 2;
    double r = 1;
    double k = 20;

    particles = std::vector<Particle> {};
    ParticleContainer container { particles };
    Membrane mem { z, 2, 2, 1, r, 1, z, 0, 3, 1, r0, k };
    mem.initLJParams(1, 2); // big sigma to include repulsive forces
    mem.generateMolecule(container, 562);

    // Check if the number of particles are correct
    EXPECT_EQ(container.particles.size(), 4);

    CellGrid grid { { -5, -5, -5 }, { 10, 10, 10 }, 5 };
    grid.addParticlesFromContainer(container);

    mem.calculateIntraMolecularForces(grid);

    // lambda function to do harmonic potential
    auto harmonicPotential = [](Particle& p1, Particle& p2, double r0, double k) {
        return k / ArrayUtils::L2Norm(p2.getX() - p1.getX()) *
               (ArrayUtils::L2Norm(p2.getX() - p1.getX()) - r0) * (p2.getX() - p1.getX());
    };

    // Generate expected forces
    std::vector<std::array<double, 3>> expectedForces(container.particles.size(), { 0, 0, 0 });
    for (int i = 0; i < container.particles.size(); i++)
        for (int j = 0; j < container.particles.size(); j++)
            if (i != j) {
                // The r0 to use depends on if the particles are aligned diagonally
                double actualR0 =
                    (container.particles[i].getX()[0] == container.particles[j].getX()[0] ||
                     container.particles[i].getX()[1] == container.particles[j].getX()[1])
                        ? r0
                        : std::sqrt(2) * r0;
                expectedForces[i] =
                    expectedForces[i] +
                    harmonicPotential(container.particles[i], container.particles[j], actualR0, k);
            }

    // Check if the forces don't match -> as LJ was added
    for (int i = 0; i < container.particles.size(); i++)
            EXPECT_TRUE(ArrayUtils::L2Norm(container.particles[i].getF() - expectedForces[i]) > PRESICION);
}
