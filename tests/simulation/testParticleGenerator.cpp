
#include "simulation/ParticleGenerator.h"
#include "utils/ArrayUtils.h"
#include <gtest/gtest.h>

class ParticleGeneratorTest : public ::testing::Test {
protected:
    std::array<double, 3> origin = { 1, 2, 3 };
    int numParticlesWidth = 13;
    int numParticlesHeight = 7;
    int numParticlesDepth = 9;
    double spacing = 1.0;
    double mass = 1.0;
    std::array<double, 3> initialVelocity = { 2, 3, 4 };
    double meanVelocity = 1.0;
    size_t dimensions = 3;
    ParticleContainer container;

    ParticleGeneratorTest()
        : origin(std::array<double, 3> { 1, 2, 3 })
        , numParticlesWidth(13)
        , numParticlesHeight(7)
        , numParticlesDepth(9)
        , spacing(1.0)
        , mass(1.0)
        , initialVelocity(std::array<double, 3> { 2, 3, 4 })
        , meanVelocity(1.0)
        , dimensions(3)
        , container(ParticleContainer { std::vector<Particle> {} })
    {
    }

    void SetUp() override
    {
        std::vector<Particle> particles = {};
        this->container = ParticleContainer { particles };

        ParticleGenerator generator { this->container };
        generator.generateCuboid(
            origin,
            numParticlesWidth,
            numParticlesHeight,
            numParticlesDepth,
            spacing,
            mass,
            initialVelocity,
            meanVelocity,
            dimensions);
    }
};

// Test if all positions of particles generated in a cuboid are unique
TEST_F(ParticleGeneratorTest, UniquePositionsInCuboid)
{
    for (auto it = container.beginPairs(); it != container.endPairs(); ++it) {
        std::pair<Particle&, Particle&> pt_pair = *it;

        std::array<double, 3> pos1 = pt_pair.first.getX(), pos2 = pt_pair.second.getX();
        double dist = ArrayUtils::L2Norm(pos1 - pos2);

        EXPECT_GT(dist, 0);
    }
}

// Test if all masses are the same
TEST_F(ParticleGeneratorTest, SameMass)
{
    for (auto& p : container) {
        EXPECT_EQ(p.getM(), mass);
    }
}

// Test if qube is spanned correctly i.e. all particles are within the qube and the qube is not too
// large
TEST_F(ParticleGeneratorTest, CorrectCuboid)
{
    std::array<double, 3> max = { 0, 0, 0 };
    for (auto& p : container) {
        std::array<double, 3> pos = p.getX();
        EXPECT_GE(pos[0], origin[0]);
        EXPECT_GE(pos[1], origin[1]);
        EXPECT_GE(pos[2], origin[2]);
        EXPECT_LE(pos[0], origin[0] + numParticlesWidth * spacing);
        EXPECT_LE(pos[1], origin[1] + numParticlesHeight * spacing);
        EXPECT_LE(pos[2], origin[2] + numParticlesDepth * spacing);

        // Find max of each dimension
        for (int i = 0; i < 3; i++) {
            if (pos[i] > max[i]) {
                max[i] = pos[i];
            }
        }
    }

    EXPECT_NEAR(max[0], origin[0] + (numParticlesWidth - 1) * spacing, 1e-6);
    EXPECT_NEAR(max[1], origin[1] + (numParticlesHeight - 1) * spacing, 1e-6);
    EXPECT_NEAR(max[2], origin[2] + (numParticlesDepth - 1) * spacing, 1e-6);

    EXPECT_EQ(
        container.getContainer().size(),
        numParticlesWidth * numParticlesHeight * numParticlesDepth);
}