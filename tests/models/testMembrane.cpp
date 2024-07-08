
#include "models/Particle.h"
#include "models/ParticleContainer.h"
#include "models/membrane/Membrane.h"
#include "gtest/gtest.h"
#include <spdlog/spdlog.h>

std::array<double, 3> z = {0, 0, 0};

TEST(MembraneTests, initMembraneTests)
{
    Particle p1 = {z, z, 1, 0};
    Particle p2 = {{1, 0, 0}, z, 1, 0};
    Particle p3 = {{2, 0, 0}, z, 1, 0};
    std::vector<Particle> particles = {p1, p2, p3};

    ParticleContainer container(particles);
    Membrane membrane({0,0,0}, 3, 1);
    membrane.initMembrane(container, 1);

    for (auto& p : container.particles) {
        EXPECT_EQ(p.getMolecular(), true);
    }

    Particle p4 = {{0, 1, 0}, z, 1, 0};
    Particle p5 = {{1, 1, 0}, z, 1, 0};
    Particle p6 = {{2, 1, 0}, z, 1, 0};

    container.addParticle(p4);
    container.addParticle(p5);
    container.addParticle(p6);

    Membrane membrane2({0,0,0}, 3, 2);
    membrane2.initMembrane(container, 1);

    for (auto& p : container.particles) {
        EXPECT_EQ(p.getMolecular(), true);
    }
}

TEST(MembraneTests, membraneMapTests)
{
    Particle p1 = {z, z, 1, 0};
    Particle p2 = {{1, 0, 0}, z, 1, 0};
    Particle p3 = {{2, 0, 0}, z, 1, 0};
    std::vector<Particle> particles = {p1, p2, p3};

    ParticleContainer container(particles);
    Membrane membrane({0,0,0}, 3, 1);
    membrane.initMembrane(container, 1);

    EXPECT_EQ(membrane.isNeighbor(container.particles[0], container.particles[1]), true);
    EXPECT_EQ(membrane.isNeighbor(container.particles[1], container.particles[2]), true);
    EXPECT_EQ(membrane.isNeighbor(container.particles[1], container.particles[0]), true);
    EXPECT_EQ(membrane.isNeighbor(container.particles[2], container.particles[1]), true);
    EXPECT_EQ(membrane.isNeighbor(container.particles[0], container.particles[2]), false);

    Particle p4 = {{0, 1, 0}, z, 1, 0};
    Particle p5 = {{1, 1, 0}, z, 1, 0};
    Particle p6 = {{2, 1, 0}, z, 1, 0};

    container.addParticle(p4);
    container.addParticle(p5);
    container.addParticle(p6);

    Membrane membrane2({0,0,0}, 3, 2);
    membrane2.initMembrane(container, 1);

    EXPECT_EQ(membrane2.isDiagonalNeighbor(container.particles[0], container.particles[4]), true);
    EXPECT_EQ(membrane2.isNeighbor(container.particles[0], container.particles[3]), true);
    EXPECT_EQ(membrane2.isDiagonalNeighbor(container.particles[1], container.particles[3]), true);
    EXPECT_EQ(membrane2.isNeighbor(container.particles[1], container.particles[4]), true);
    EXPECT_EQ(membrane2.isDiagonalNeighbor(container.particles[1], container.particles[5]), true);
    EXPECT_EQ(membrane2.isNeighbor(container.particles[2], container.particles[5]), true);
    EXPECT_EQ(membrane2.isDiagonalNeighbor(container.particles[2], container.particles[4]), true);
    EXPECT_EQ(membrane2.isDiagonalNeighbor(container.particles[4], container.particles[0]), true);
    EXPECT_EQ(membrane2.isDiagonalNeighbor(container.particles[4], container.particles[2]), true);
    EXPECT_EQ(membrane2.isNeighbor(container.particles[4], container.particles[1]), true);

    EXPECT_EQ(membrane2.isCalcNeighbor(container.particles[1], container.particles[2]), true);
    EXPECT_EQ(membrane2.isCalcNeighbor(container.particles[1], container.particles[5]), true);
    EXPECT_EQ(membrane2.isCalcNeighbor(container.particles[1], container.particles[4]), true);
    EXPECT_EQ(membrane2.isCalcNeighbor(container.particles[1], container.particles[3]), true);

    std::vector<int> neighbors = membrane2.getNeighbors(container.particles[1]);
    std::vector check{2, 5, 4, 3, 0, -1, -1 , -1};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors[id] == check[id], true);
    }
}