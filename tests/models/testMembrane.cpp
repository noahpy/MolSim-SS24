
#include "models/Particle.h"
#include "models/ParticleContainer.h"
#include "models/molecules/Membrane.h"
#include "utils/ArrayUtils.h"
#include "gtest/gtest.h"

std::array<double, 3> z = { 0, 0, 0 };

// Test if the molecule id has been set correctly
TEST(MembraneTests, initMembraneTests)
{
    Membrane m { z, 20, 20, 1, 1, 1, z, 0, 3, 1, 1, 20 };

    std::vector<Particle> particles {};
    ParticleContainer container(particles);

    m.generateMolecule(container, 562);

    for (auto& p : container.particles) {
        EXPECT_EQ(p.getMoleculeId(), 562);
    }
}

// Check if the neighbors are set correctly
TEST(MembraneTests, membraneMapTests)
{
    int width = 34;
    int height = 23;
    int depth = 1;
    Membrane m { z, width, height, depth, 1, 1, z, 0, 3, 1, 1, 20 };

    std::vector<Particle> particles {};
    ParticleContainer container(particles);

    m.generateMolecule(container, 562);

    // Check if the number of particles and neighbors are correct
    EXPECT_EQ(container.particles.size(), width * height * depth);
    EXPECT_EQ(m.getDiagNeighbors().size(), width * height * depth);
    EXPECT_EQ(m.getDirectNeighbors().size(), width * height * depth);

    std::array<double, 3> directTop = { 0, 1, 0 };
    std::array<double, 3> directRight = { 1, 0, 0 };

    std::array<double, 3> diagTopRight = { 1, 1, 0 };
    std::array<double, 3> diagBottomRight = { 1, -1, 0 };

    // A neighbor's position must be reconstructable from the particle's position
    // Note the != works as XOR here i.e. only one of the two shifts can be true
    for (auto& p : container.particles) {
        auto ref = std::reference_wrapper<Particle>(p);
        for (auto& neighbor : m.getDirectNeighbors().at(ref)) {
            bool neighborIsValid = (neighbor.get().getX() == p.getX() + directTop) !=
                                   (neighbor.get().getX() == p.getX() + directRight);
            EXPECT_TRUE(neighborIsValid);
        }
        for (auto& neighbor : m.getDiagNeighbors().at(ref)) {
            bool neighborIsValid = (neighbor.get().getX() == p.getX() + diagTopRight) !=
                                   (neighbor.get().getX() == p.getX() + diagBottomRight);
            EXPECT_TRUE(neighborIsValid);
        }
    }
}

// Test if the number of neighbors is correct
TEST(MembraneTests, membraneNeighborCountTests)
{
    int width = 34;
    int height = 23;
    int depth = 1;
    Membrane m { z, width, height, depth, 1, 1, z, 0, 3, 1, 1, 20 };

    std::vector<Particle> particles {};
    ParticleContainer container(particles);

    m.generateMolecule(container, 562);

    // lambda function to check if particle is at the top layer
    auto isTopLayer = [](Particle& p, int height, double spacing) {
        return p.getX()[1] > (static_cast<double>(height - 1) - 0.5f) * spacing;
    };
    // lambda function to check if particle is at right layer
    auto isRightLayer = [](Particle& p, int width, double spacing) {
        return p.getX()[0] > (static_cast<double>(width - 1) - 0.5f) * spacing;
    };
    // lambda function to check if particle is at the bottom layer
    auto isBottomLayer = [](Particle& p, double spacing) { return p.getX()[1] < 0.5f * spacing; };

    // Check if the number of neighbors is correct
    for (auto& p : container.particles) {
        auto ref = std::reference_wrapper<Particle>(p);
        int expectedDirectNeighbors =
            2 - (isTopLayer(p, height, 1) ? 1 : 0) - (isRightLayer(p, width, 1) ? 1 : 0);
        int expectedDiagNeighbors =
            2 - (isRightLayer(p, width, 1)
                     ? 2
                     : (isTopLayer(p, height, 1) || isBottomLayer(p, 1) ? 1 : 0));

        EXPECT_EQ(m.getDirectNeighbors().at(ref).size(), expectedDirectNeighbors);
        EXPECT_EQ(m.getDiagNeighbors().at(ref).size(), expectedDiagNeighbors);
    }
}