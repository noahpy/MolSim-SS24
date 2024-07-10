
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
        EXPECT_EQ(p.getMembraneId() != -1, true);
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
        EXPECT_EQ(p.getMembraneId() != -1, true);
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


    std::vector<int> neighbors = membrane2.getNeighbors(container.particles[0]);
    std::vector check{1, 4, 3, -1, -1, -1, -1 , -1};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors[id] == check[id], true);
    }

    std::vector<int> neighbors2 = membrane2.getNeighbors(container.particles[1]);
    std::vector check2{2, 5, 4, 3, 0, -1, -1 , -1};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors2[id] == check2[id], true);
    }

    std::vector<int> neighbors3 = membrane2.getNeighbors(container.particles[2]);
    std::vector check3{-1, -1, 5, 4, 1, -1, -1 , -1};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors3[id] == check3[id], true);
    }

    std::vector<int> neighbors4 = membrane2.getNeighbors(container.particles[3]);
    std::vector check4{4, -1, -1, -1, -1, -1, 0 , 1};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors4[id] == check4[id], true);
    }

    std::vector<int> neighbors5 = membrane2.getNeighbors(container.particles[4]);
    std::vector check5{5, -1, -1, -1, 3, 0, 1 , 2};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors5[id] == check5[id], true);
    }

    std::vector<int> neighbors6 = membrane2.getNeighbors(container.particles[5]);
    std::vector check6{-1, -1, -1, -1, 4, 1, 2 , -1};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors6[id] == check6[id], true);
    }
}

TEST(MembraneTests, membraneMapTests2)
{
    Particle p1 = {z, z, 1, 0};
    Particle p2 = {{2, 0, 0}, z, 1, 0};
    Particle p3 = {{4, 0, 0}, z, 1, 0};
    std::vector<Particle> particles = {p1, p2, p3};

    ParticleContainer container(particles);
    Membrane membrane({0,0,0}, 3, 1);
    membrane.initMembrane(container, 2);

    EXPECT_EQ(membrane.isNeighbor(container.particles[0], container.particles[1]), true);
    EXPECT_EQ(membrane.isNeighbor(container.particles[1], container.particles[2]), true);
    EXPECT_EQ(membrane.isNeighbor(container.particles[1], container.particles[0]), true);
    EXPECT_EQ(membrane.isNeighbor(container.particles[2], container.particles[1]), true);
    EXPECT_EQ(membrane.isNeighbor(container.particles[0], container.particles[2]), false);

    Particle p4 = {{0, 2, 0}, z, 1, 0};
    Particle p5 = {{2, 2, 0}, z, 1, 0};
    Particle p6 = {{4, 2, 0}, z, 1, 0};

    container.addParticle(p4);
    container.addParticle(p5);
    container.addParticle(p6);

    Membrane membrane2({0,0,0}, 3, 2);
    membrane2.initMembrane(container, 2);

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


    std::vector<int> neighbors = membrane2.getNeighbors(container.particles[0]);
    std::vector check{1, 4, 3, -1, -1, -1, -1 , -1};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors[id] == check[id], true);
    }

    std::vector<int> neighbors2 = membrane2.getNeighbors(container.particles[1]);
    std::vector check2{2, 5, 4, 3, 0, -1, -1 , -1};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors2[id] == check2[id], true);
    }

    std::vector<int> neighbors3 = membrane2.getNeighbors(container.particles[2]);
    std::vector check3{-1, -1, 5, 4, 1, -1, -1 , -1};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors3[id] == check3[id], true);
    }

    std::vector<int> neighbors4 = membrane2.getNeighbors(container.particles[3]);
    std::vector check4{4, -1, -1, -1, -1, -1, 0 , 1};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors4[id] == check4[id], true);
    }

    std::vector<int> neighbors5 = membrane2.getNeighbors(container.particles[4]);
    std::vector check5{5, -1, -1, -1, 3, 0, 1 , 2};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors5[id] == check5[id], true);
    }

    std::vector<int> neighbors6 = membrane2.getNeighbors(container.particles[5]);
    std::vector check6{-1, -1, -1, -1, 4, 1, 2 , -1};
    for (int id = 0; id < 8; ++id) {
        EXPECT_EQ(neighbors6[id] == check6[id], true);
    }
}